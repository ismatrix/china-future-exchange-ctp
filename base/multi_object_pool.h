/**
 *@file		multi_object_pool.h
 *
 *@brief	对象池分组
 *			设计原则：1、不依赖第三方库 2、支持任何对象 3、支持对象分组
 *
 *@author	jimmymo
 *@date		2012-02-14
 *@version	1.0
 *$Id: multi_object_pool.h,v 1.0 2012/02/14 10:09:05 jimmymo Exp $
 */


#ifndef __MULTI_OBJECT_POOL_H__
#define __MULTI_OBJECT_POOL_H__

#include <pthread.h>

#include <map>
#include <string>
#include <assert.h>

#include "object_pool.h"

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * struct __DefaultMultiPoolConstructor
 */
template<typename TObject, typename TObjectPool = ObjectPool<TObject> >
struct __DefaultMultiPoolConstructor
{
    /**
     * Overloaded operator()
     * 
     * @param [in] obj	the objectpool of multiobjectpool
     * @return
     *    - 0.
     */
    TObjectPool* operator () ( string opname )
    {
        return new TObjectPool();
    }
};

//////////////////////////////////////////////////////////////////////////////////////////
/**
 *@name		MultiObjectPool
 *@brief	
 */
template< typename TObject, typename ConstructorType = __DefaultMultiPoolConstructor<TObject>,
	typename TObjectPool = ObjectPool<TObject> >
class MultiObjectPool 
{
	public:
		struct stObjectOfGroup {
			string csName;
			TObject * cpoTObject;
		};

	public:
		/**
		 *@brief	Default constructor
		 */
		MultiObjectPool( );

		/**
		 *@brief	Destructor
		 */
		virtual ~MultiObjectPool( );

		int Initialize( ConstructorType aConstructorType = ConstructorType() );

		TObject * GetObject( string opname );

		void FreeObject( TObject * cpoTObject );

		void ReleaseObject( TObject * cpoTObject );

	protected:
		void AddObjectPool( string opname );

		void RemoveObjectPool( string opname );

	private:
		/**
		 *@brief	Copy constructor
		 */
		//MultiObjectPool( const MultiObjectPool& );

		/**
		 *@brief	Assignment operator
		 */
		//MultiObjectPool& operator=( const MultiObjectPool& );

	private:
		ConstructorType coConstructorType;

		map<string,TObjectPool *> coConnectionGroup;
		//map<int,stObjectOfGroup> coCheckOutConnection; // 64位机器编译报错，error: cast from 'MyObject*' to 'int' loses precision
		map<size_t,stObjectOfGroup> coCheckOutConnection; // size_t兼容64位机器，长度可以容纳指针

		pthread_mutex_t ctGroupMutex;
		pthread_mutex_t ctCheckOutMutex;
};


/////////////////////////////////////////////////////////////////////////////////////////
//implement

//	Default constructor
	template< typename TObject, typename ConstructorType, typename TObjectPool >
	MultiObjectPool<TObject, ConstructorType, TObjectPool>::MultiObjectPool( )
: coCheckOutConnection( ), coConnectionGroup( )
{
	TX_LOG_CALL( "MultiObjectPool::MultiObjectPool( )..." );

	pthread_mutex_init( &ctGroupMutex, NULL );
	pthread_mutex_init( &ctCheckOutMutex, NULL );

	TX_LOG_RETN( "MultiObjectPool::MultiObjectPool(...)" );
}

//	Destructor
	template< typename TObject, typename ConstructorType, typename TObjectPool >
MultiObjectPool<TObject, ConstructorType, TObjectPool>::~MultiObjectPool( )
{
	TX_LOG_CALL( "MultiObjectPool::~MultiObjectPool( )..." );

	pthread_mutex_destroy( &ctGroupMutex );
	pthread_mutex_destroy( &ctCheckOutMutex );

	TX_LOG_RETN( "MultiObjectPool::~MultiObjectPool(...)" );
}

template< typename TObject, typename ConstructorType, typename TObjectPool >
int MultiObjectPool<TObject, ConstructorType, TObjectPool>::Initialize( ConstructorType aConstructorType )
{
	coConstructorType = aConstructorType;

	return 0;
}

	template< typename TObject, typename ConstructorType, typename TObjectPool >
TObject * MultiObjectPool<TObject, ConstructorType, TObjectPool>::GetObject( string opname)
{
	TX_LOG_CALL( "MultiObjectPool::GetObject( opname=%s )...", opname.c_str() );

	TObject * lpoTObject = NULL;

	pthread_mutex_lock( &ctGroupMutex );

	typename map<string,TObjectPool *>::iterator iter;

	iter = coConnectionGroup.find( opname );

	if ( iter != coConnectionGroup.end() )
	{
		TObjectPool * lpoObjectPool = iter->second;
		pthread_mutex_unlock( &ctGroupMutex );
		// 获取对象时，有可能会卡住（比如无法顺利建立连接对象），因此必须先释放锁，以免影响到其它线程的正常执行
		lpoTObject = lpoObjectPool->GetObject( );
	}
	else
	{
		pthread_mutex_unlock( &ctGroupMutex );

		AddObjectPool( opname );

		{
			pthread_mutex_lock( &ctGroupMutex );

			iter = coConnectionGroup.find( opname );

			//assert( iter != coConnectionGroup.end() );

			TObjectPool * lpoObjectPool = NULL;

			if ( iter != coConnectionGroup.end() )
			{
				lpoObjectPool = iter->second;
				pthread_mutex_unlock( &ctGroupMutex );
				// 获取对象时，有可能会卡住（比如无法顺利建立连接对象），因此必须先释放锁，以免影响到其它线程的正常执行
				lpoTObject = lpoObjectPool->GetObject( );
			}
			else
			{
				pthread_mutex_unlock( &ctGroupMutex );
			}
		}
	}

	if ( lpoTObject != NULL )
	{
		pthread_mutex_lock( &ctCheckOutMutex );
		//stObjectOfGroup loConnectionOfGroup( opname, lpoTObject );
		stObjectOfGroup loConnectionOfGroup;
		loConnectionOfGroup.csName = opname;
		loConnectionOfGroup.cpoTObject = lpoTObject;
		//printf( "&loConnectionOfGroup=0x%x\n", (int)&loConnectionOfGroup );
		coCheckOutConnection[ (size_t)lpoTObject ] = loConnectionOfGroup;
		pthread_mutex_unlock( &ctCheckOutMutex );
	}
	else
	{
		// error
	}

	TX_LOG_RETN( "MultiObjectPool::GetObject(...)=0x%x", lpoTObject );

	return lpoTObject;
}

	template< typename TObject, typename ConstructorType, typename TObjectPool >
void MultiObjectPool<TObject, ConstructorType, TObjectPool>::FreeObject( TObject * apoTObject )
{
	TX_LOG_CALL( "MultiObjectPool::FreeObject( apoTObject=0x%x )...", apoTObject );

	pthread_mutex_lock( &ctCheckOutMutex );

	typename map<size_t,stObjectOfGroup>::iterator iter_conn = coCheckOutConnection.find( (size_t)apoTObject );

	if ( iter_conn != coCheckOutConnection.end() )
	{
		string lsName = iter_conn->second.csName;
		TObject * lpoTObject = iter_conn->second.cpoTObject;
		//printf( "lsName=%s,apoTObject=0x%x\n", lsName.c_str(), (int)lpoTObject );
		assert( lpoTObject == apoTObject );

		coCheckOutConnection.erase( iter_conn );

		pthread_mutex_unlock( &ctCheckOutMutex );

		{
			pthread_mutex_lock( &ctGroupMutex );

			typename map<string,TObjectPool *>::iterator iter = coConnectionGroup.find( lsName );

			if ( iter != coConnectionGroup.end() )
			{
				TObjectPool * lpoObjectPool = iter->second;
				// 释放对象时，有可能会卡住（比如无法顺利断开连接对象），因此必须先释放锁，以免影响到其它线程的正常执行
				pthread_mutex_unlock( &ctGroupMutex );
				lpoObjectPool->FreeObject( lpoTObject );
			}
			else
			{
				pthread_mutex_unlock( &ctGroupMutex );
			}
		}
	}
	else
	{
		pthread_mutex_unlock( &ctCheckOutMutex );
	}

	TX_LOG_RETN( "MultiObjectPool::FreeObject(...)" );
}

	template< typename TObject, typename ConstructorType, typename TObjectPool >
void MultiObjectPool<TObject, ConstructorType, TObjectPool>::ReleaseObject( TObject * apoTObject )
{
	TX_LOG_CALL( "MultiObjectPool::ReleaseObject( apoTObject=0x%x )...", apoTObject );

	pthread_mutex_lock( &ctCheckOutMutex );

	typename map<size_t,stObjectOfGroup>::iterator iter_conn = coCheckOutConnection.find( (size_t)apoTObject );

	if ( iter_conn != coCheckOutConnection.end() )
	{
		string lsName = iter_conn->second.csName;
		TObject * lpoTObject = iter_conn->second.cpoTObject;
		assert( lpoTObject == apoTObject );

		coCheckOutConnection.erase( iter_conn );

		pthread_mutex_unlock( &ctCheckOutMutex );

		{
			pthread_mutex_lock( &ctGroupMutex );

			typename map<string,TObjectPool *>::iterator iter_pool = coConnectionGroup.find( lsName );

			if ( iter_pool != coConnectionGroup.end() )
			{
				iter_pool->second->ReleaseObject( lpoTObject );
			}
			else
			{
			}

			pthread_mutex_unlock( &ctGroupMutex );
		}
	}
	else
	{
		pthread_mutex_unlock( &ctCheckOutMutex );
	}

	TX_LOG_RETN( "MultiObjectPool::ReleaseObject(...)" );
}

	template< typename TObject, typename ConstructorType, typename TObjectPool >
void MultiObjectPool<TObject, ConstructorType, TObjectPool>::AddObjectPool( string opname )
{
	TX_LOG_CALL( "MultiObjectPool::AddObjectPool( opname=%s )...", opname.c_str() );

	TObjectPool * lpoConnectionPool = coConstructorType( opname );

	if ( lpoConnectionPool != NULL )
	{
		pthread_mutex_lock( &ctGroupMutex );
		coConnectionGroup.insert( typename map<string,TObjectPool *>::value_type( opname, lpoConnectionPool ) );
		pthread_mutex_unlock( &ctGroupMutex );
	}
	else
	{
		// error
	}

	TX_LOG_RETN( "MultiObjectPool::AddObjectPool(...)" );
}

	template< typename TObject, typename ConstructorType, typename TObjectPool >
void MultiObjectPool<TObject, ConstructorType, TObjectPool>::RemoveObjectPool( string opname)
{
	TX_LOG_CALL( "MultiObjectPool::RemoveObjectPool( opname=%s )...", opname.c_str() );

	pthread_mutex_lock( &ctGroupMutex );

	typename map<int,TObjectPool *>::iterator iter_pool = coConnectionGroup.find( opname );

	if ( iter_pool != coConnectionGroup.end() )
	{
		delete( iter_pool->second );
		coConnectionGroup.erase( iter_pool );
	}
	else
	{
	}

	pthread_mutex_unlock( &ctGroupMutex );

	TX_LOG_RETN( "MultiObjectPool::RemoveObjectPool(...)" );
}

#endif //__MULTI_OBJECT_POOL_H__
