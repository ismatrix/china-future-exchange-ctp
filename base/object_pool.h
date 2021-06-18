/**
 *@file		object_pool.h
 *
 *@brief	对象池
 *			设计原则：1、不依赖第三方库 2、支持任何对象 3、支持对象分组
 *
 *@author	jimmymo
 *@date		2012-02-14
 *@version	1.0
 *$Id: object_pool.h,v 1.0 2012/02/14 10:09:05 jimmymo Exp $
 */

#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <string.h>

#include <map>
#include <deque>

using namespace std;

#define SIZE_IP_ADDR			32
#define DEFAULT_POOL_SIZE	1024
#define DEFAULT_BUCKET 128
#define DEFAULT_RESERVE_TIMEOUT 300

enum {
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_STAT,
	LOG_LEVEL_CALL,
	LOG_LEVEL_RET,
	LOG_LEVEL_TRACE
};

#define TX_HOOK_LOG_OUTPUT(buf) cout << "[" << __FILE__ << ":" << __LINE__ << "]" << buf;

/////////////////////////////////////////////////////
template<typename TObject>
int op_printf(char const *fmt, ...)
{
	char buf[4096];

	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	TX_HOOK_LOG_OUTPUT(buf);

	return 0;
}


#define TX_LOG( level, format, ... ) op_printf<int>( format, ##__VA_ARGS__ )
#define TX_LOG_ERROR( format, ... ) TX_LOG( LOG_LEVEL_ERROR, "ERROR: " format "\n", ##__VA_ARGS__ )
#define TX_LOG_WARN( format, ... ) TX_LOG( LOG_LEVEL_WARN, "WARN: " format "\n", ##__VA_ARGS__ )
#define TX_LOG_INFO( format, ... ) TX_LOG( LOG_LEVEL_INFO, "INFO: " format "\n", ##__VA_ARGS__ )
#define TX_LOG_STAT( format, ... ) TX_LOG( LOG_LEVEL_STAT, "STAT: " format "\n", ##__VA_ARGS__ )
#define TX_LOG_CALL( format, ... ) TX_LOG( LOG_LEVEL_CALL, "CALL: " format "\n", ##__VA_ARGS__ )
#define TX_LOG_RETN( format, ... ) TX_LOG( LOG_LEVEL_RET, "RETN: " format "\n", ##__VA_ARGS__ )
#define TX_LOG_TRACE( format, ... ) TX_LOG( LOG_LEVEL_TRACE, "TRACE: " format "\n", ##__VA_ARGS__ )

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * struct __DefaultPoolConstructor
 */
template<typename TObject>
struct __DefaultPoolConstructor
{
    /**
     * Overloaded operator()
     * 
     * @param [in] obj	the object of objectpool
     * @return
     *    - 0.
     */
    TObject* operator () ( )
    {
        return new TObject();
    }
};

//////////////////////////////////////////////////////////////////////////////////////////
/**
 *@name		ObjectPool
 *@brief	
 */
template< typename TObject, typename ConstructorType = __DefaultPoolConstructor<TObject> >
class ObjectPool 
{
	private:
		struct ObjectNode{
			time_t	ctCreateTime;
			time_t	ctVisitTime;
			TObject*	cpObject;
		};

	public:
	/**
	 *@brief	Default constructor
		 */
		ObjectPool( );

		/**
		 *@brief	Default constructor
		 *@param	aiReserveTimeout
		 *连接保留超时(Connection Reserve Timeout): 设为 30 秒 
		 *在保留缓冲池连接的调用超时之前的秒数(0 - 32 位正整数)。如果设置为 -1，则马上超时。
		 *The number of seconds (between -1 and a positive 32-bit integer) after which a call to reserve a connection from the pool will timeout. When set to 0, a call will never timeout. When set to -1, a call will timeout immediately
		 */
		ObjectPool( int iPoolSize, int iMaxObject, int iReserveTimeout = 0 );

		/**
		 *@brief	Destructor
		 */
		virtual ~ObjectPool( );

		/**
		 *@brief Initialize
		 *arg: aiInterval : if need to auto reflush connection, 
		 *									the connection will be used no more than <aiFlushInterval> seconds.
		 */
		int Initialize( int aiPoolSize = DEFAULT_POOL_SIZE, int aiMaxConn = DEFAULT_POOL_SIZE, int aiReserveTimeout = 0, ConstructorType aConstructorType = ConstructorType());

		/**
		 *@brief Get a connection from the pool or create one
		 */
		virtual TObject * GetObject( );

		/**
		 *@brief Free the connection back to the pool
		 */
		virtual void FreeObject( TObject * apoTConn );

		/**
		 *@brief Release one connection
		 */
		virtual void ReleaseObject( TObject * apoConn );

		int FreeSize( );
		
		void PrintStatistic( void );

	protected:
		/**
		 *@brief Hook function to new connection
		 *			derived class must redefine how to new connection
		 */
		//virtual TObject * CreateObjectHook( ) = 0;
		//virtual void ReleaseObjectHook( TObject * apoTConn )
		//{
		//	TX_LOG_CALL( "ObjectPool::%s( apoTConn=0%x ) ", __FUNCTION__, apoTConn );

		//	if( apoTConn != NULL ){
		//		delete apoTConn;
		//	}

		//	TX_LOG_RETN( "ObjectPool::%s() ", __FUNCTION__ );
		//	
		//	return;
		//}

		void IncreaseUsing( void )
		{
			pthread_mutex_lock( &ctLock );
			ciUsingCount++;
			pthread_mutex_unlock( &ctLock );
		}

		void DecreaseUsing( void )
		{
			pthread_mutex_lock( &ctLock );
			ciUsingCount--;
			pthread_mutex_unlock( &ctLock );
		}

		/**
		 *@brief Release all connections
		 */
		virtual void ReleaseAll( );

	private:
		/**
		 *@brief	Copy constructor
		 */
		//ObjectPool( const ObjectPool& );

		/**
		 *@brief	Assignment operator
		 */
		//ObjectPool& operator=( const ObjectPool& );

		/**
		 *@brief
		 */
		virtual TObject * CreateObject( );

		void SaveObject( ObjectNode &aoNode ){
			pthread_mutex_lock( &ctFreeMutex );
			coFreeConnections.push_back(aoNode);
			pthread_mutex_unlock( &ctFreeMutex );
			DecreaseUsing();
			PrintStatistic();
		}

	private:
		int ciCachePoolSize;
		int ciMaxConnection;
		int ciUsingCount;
		int ciReserveTimeout;

		ConstructorType coConstructorType;

		pthread_mutex_t ctLock;
		pthread_mutex_t ctFreeMutex;
		pthread_mutex_t	ctCheckoutMutex;
		deque<ObjectNode> coFreeConnections;
		//map< int, ObjectNode > coCheckoutConnection; // 64位机器编译报错，error: cast from 'MyObject*' to 'int' loses precision
		map< size_t, ObjectNode > coCheckoutConnection; // size_t兼容64位机器，长度可以容纳指针
};


/////////////////////////////////////////////////////////////////////////////////////////
//implement
template< typename TObject, typename ConstructorType >
ObjectPool<TObject, ConstructorType>::ObjectPool( )
	: coFreeConnections( ),
	coCheckoutConnection( )
{
	TX_LOG_CALL( "ObjectPool::ObjectPool( )" );

	ciCachePoolSize = DEFAULT_POOL_SIZE;
	ciMaxConnection = DEFAULT_POOL_SIZE;
	ciUsingCount = 0;
	ciReserveTimeout = 0;

	pthread_mutex_init( &ctLock, NULL ); 
	pthread_mutex_init( &ctFreeMutex, NULL ); 
	pthread_mutex_init( &ctCheckoutMutex, NULL );

	TX_LOG_RETN( "ObjectPool::ObjectPool(...)" );
}

template< typename TObject, typename ConstructorType >
ObjectPool<TObject, ConstructorType>::ObjectPool( int aiPoolSize, int aiMaxConn,
		int aiReserveTimeout )
	: coFreeConnections( ),
	coCheckoutConnection( ) 
{
	TX_LOG_CALL( "ObjectPool::ObjectPool(aiPoolSize=%d,aiMaxConn=%d,"
			" aiReserveTimeout=%d )",
			aiPoolSize, aiMaxConn, aiReserveTimeout );

	ciCachePoolSize = aiPoolSize;
	ciMaxConnection = aiMaxConn;
	ciUsingCount = 0;
	ciReserveTimeout = aiReserveTimeout;
	
	pthread_mutex_init( &ctLock, NULL ); 
	pthread_mutex_init( &ctFreeMutex, NULL ); 
	pthread_mutex_init( &ctCheckoutMutex, NULL );

	TX_LOG_RETN( "ObjectPool::ObjectPool(...)" );
}

template< typename TObject, typename ConstructorType >
ObjectPool<TObject, ConstructorType>::~ObjectPool( )
{
	TX_LOG_CALL( "ObjectPool::~ObjectPool" );
	
	ReleaseAll( );
	pthread_mutex_destroy( &ctLock );
	pthread_mutex_destroy( &ctFreeMutex );
	pthread_mutex_destroy( &ctCheckoutMutex );
	
	TX_LOG_RETN( "ObjectPool::~ObjectPool(...)" );
}

template< typename TObject, typename ConstructorType >
int ObjectPool<TObject, ConstructorType>::Initialize( int aiPoolSize, int aiMaxConn, 
		int aiReserveTimeout, ConstructorType aConstructorType )
{
	TX_LOG_CALL( "ObjectPool::Initialize(aiPoolSize=%d,aiMaxConn=%d,"
			"aiReserveTimeout=%d )", 
			aiPoolSize, aiMaxConn, aiReserveTimeout );

	ciCachePoolSize = aiPoolSize;
	ciMaxConnection = aiMaxConn;
	ciReserveTimeout = aiReserveTimeout;
	coConstructorType = aConstructorType;

	TX_LOG_RETN( "ObjectPool::Initialize() = 0 " );

	return 0;
}

template< typename TObject, typename ConstructorType >
TObject * ObjectPool<TObject, ConstructorType>::GetObject( )
{
	TX_LOG_CALL( "ObjectPool::GetObject( )..." );

	time_t ltNow = time( NULL );
	struct ObjectNode loNode;

	while( true ){

		memset( &loNode, 0, sizeof( ObjectNode ) );

		if ( FreeSize( ) > 0 ) 
		{
			pthread_mutex_lock( &ctFreeMutex );
			// double check
			if (coFreeConnections.size() > 0)
			{
				loNode = coFreeConnections.front();
				coFreeConnections.pop_front();
				pthread_mutex_unlock( &ctFreeMutex );

			}
			else
			{
				pthread_mutex_unlock( &ctFreeMutex );
				continue;
			}

			TX_LOG_INFO( "INFO: Reuse the connection( 0x%x ).", loNode.cpObject );
			IncreaseUsing();
		}
		else if ( ( FreeSize( ) + ciUsingCount ) >= ciMaxConnection) {
			pthread_mutex_unlock( &ctFreeMutex );
			TX_LOG_ERROR( "Concurrent connection reaches maximum limit %d", ciMaxConnection);
			break;
		}
		else {
			pthread_mutex_unlock( &ctFreeMutex );
			/* try to establish a new connection */
			loNode.cpObject = CreateObject( );
			loNode.ctCreateTime  = ltNow;
			loNode.ctVisitTime = ltNow;
			/* connection maybe broken */
			if ( loNode.cpObject == NULL )
			{
				break;
			}
		}

		if( loNode.cpObject != NULL ) {

			if( ciReserveTimeout > 0 ) {
				pthread_mutex_lock( &ctCheckoutMutex );
				time_t ltLastVisitTime = loNode.ctVisitTime;
				loNode.ctVisitTime = ltNow;
				coCheckoutConnection.insert( typename map<size_t, ObjectNode >::value_type(
							(size_t)loNode.cpObject, loNode ) );
				pthread_mutex_unlock( &ctCheckoutMutex );

				/*	
				if( ciReserveTimeout < ltNow - ltLastVisitTime ){
					TX_LOG_INFO( "INFO: Connection( 0x%x ) used %d seconds. release it.", 
							loNode.cpObject, ltNow - ltLastVisitTime);
					ReleaseObject( loNode.cpObject );
					continue;
				}
			*/
			}
		}

		if( loNode.cpObject != NULL ){
			break;
		}
	}

	TX_LOG_RETN( "ObjectPool::GetObject(...)=0x%x", loNode.cpObject );

	return loNode.cpObject;
}

	template< typename TObject, typename ConstructorType >
TObject * ObjectPool<TObject, ConstructorType>::CreateObject( )
{
	TX_LOG_CALL( "ObjectPool::CreateObject( )...");

	TObject * lpoTObject = coConstructorType();

	if ( lpoTObject != NULL ) {
		IncreaseUsing();
	}

	PrintStatistic();

	TX_LOG_RETN( "ObjectPool::CreateObject( )=0x%x", lpoTObject );

	return lpoTObject;
}

	template< typename TObject, typename ConstructorType >
void ObjectPool<TObject, ConstructorType>::FreeObject( TObject * apoTConn )
{
	TX_LOG_CALL( "ObjectPool::%s( apoTConn = 0x%x )",
			__FUNCTION__, apoTConn );

	if ( apoTConn == NULL ) {
		return;
	}

	ObjectNode loNode;
	memset( &loNode, 0, sizeof( ObjectNode ) );
	loNode.cpObject = apoTConn;

	time_t	ltNow = time( NULL );
	typename map<size_t, ObjectNode>::iterator iter;

	if( ciReserveTimeout > 0 ){
		//timeout if it have been used more than ciReserveTimeout seconds.
		pthread_mutex_lock( &ctCheckoutMutex );

		if( ( iter = coCheckoutConnection.find( (size_t)apoTConn ) ) != coCheckoutConnection.end()) {

			loNode.ctCreateTime = iter->second.ctCreateTime;
			loNode.ctVisitTime = iter->second.ctVisitTime;
			loNode.cpObject = iter->second.cpObject;

			if ( FreeSize( ) < ciCachePoolSize ) {

				TX_LOG_TRACE( "Connection(0x%x) been used %d seconds, cache it.", 
						loNode.cpObject, ltNow - loNode.ctVisitTime);

				coCheckoutConnection.erase( iter );
				pthread_mutex_unlock( &ctCheckoutMutex );
				SaveObject( loNode );
			} 
			else {
				TX_LOG_TRACE( "Connection(0x%x) been used %d seconds, release it.",
						loNode.cpObject, ltNow - loNode.ctVisitTime);

				pthread_mutex_unlock( &ctCheckoutMutex );
				ReleaseObject( apoTConn );
			}
		}
		else
		{
			//unkown object
			TX_LOG_TRACE( "Connection(0x%x) unkown object, release it.", apoTConn );
			pthread_mutex_unlock( &ctCheckoutMutex );
			ReleaseObject( apoTConn );
		}
	}
	else if( ciReserveTimeout == -1){
		//timeout immediately
		TX_LOG_TRACE( "Connection(0x%x) release immediately.", apoTConn );
		ReleaseObject( apoTConn );
	}
	else{
		//Never timeout.
		if( FreeSize( ) <= ciCachePoolSize ){
			TX_LOG_TRACE( "Connection(0x%x) allways cache it..", apoTConn );
			SaveObject( loNode );
		}
		else{
			TX_LOG_TRACE( "CachePoolSize is full, release it. Connection( 0x%x )", apoTConn );
			ReleaseObject( apoTConn );
		}
	}

	TX_LOG_RETN( "ObjectPool::FreeObject(...)" );
}

	template< typename TObject, typename ConstructorType >
void ObjectPool<TObject, ConstructorType>::ReleaseAll( )
{
	TX_LOG_CALL( "ObjectPool::ReleaseAll( )..." );

	struct ObjectNode loNode;
	memset( &loNode, 0, sizeof( loNode ) );

	/* release free connections */
	pthread_mutex_lock( &ctFreeMutex );
	while ( coFreeConnections.size() > 0 ) {
		loNode = coFreeConnections.front();
		coFreeConnections.pop_front();

		if( loNode.cpObject != NULL ){
			delete loNode.cpObject;
		}
	}
	pthread_mutex_unlock( &ctFreeMutex );

	/* release check out connections */
	//FIXME

	PrintStatistic();

	TX_LOG_RETN( "ObjectPool::ReleaseAll(...)" );
}

	template< typename TObject, typename ConstructorType >
void ObjectPool<TObject, ConstructorType>::ReleaseObject( TObject * apoConn )
{
	TX_LOG_CALL( "ObjectPool::ReleaseObject( )..." );

	if( ciReserveTimeout > 0 ){

		pthread_mutex_lock( &ctCheckoutMutex );
		typename map<size_t,ObjectNode>::iterator iter = coCheckoutConnection.find( (size_t)apoConn );  
		if ( iter != coCheckoutConnection.end() ) {

			coCheckoutConnection.erase( iter );
		}
		else{
			TX_LOG_WARN( "WARN: Can't find the connection from the checkout list." );
		}
		pthread_mutex_unlock( &ctCheckoutMutex );

	}

	if ( apoConn != NULL )
	{
		delete apoConn;
	}
	DecreaseUsing();
	PrintStatistic();

	TX_LOG_RETN( "ObjectPool::ReleaseObject(...)" );
}

	template< typename TObject, typename ConstructorType >
int ObjectPool<TObject, ConstructorType>::FreeSize( void )
{
	int liSize;
	
	pthread_mutex_lock( &ctFreeMutex );
	liSize = coFreeConnections.size();
	pthread_mutex_unlock( &ctFreeMutex );

	return liSize;
}

	template< typename TObject, typename ConstructorType >
void ObjectPool<TObject, ConstructorType>::PrintStatistic( void )
{
	TX_LOG_STAT( "Total connections = %d, Cache connection = %d, Using connection = %d",
			FreeSize() + ciUsingCount, FreeSize(), ciUsingCount );
}

#endif//__OBJECT_POOL_H__
