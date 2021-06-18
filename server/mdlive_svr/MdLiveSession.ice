#ifndef		__SUBCRIBRMd_ICE__
#define		__SUBCRIBRMd_ICE__

#include <Ice/Identity.ice>
#include <ice/Common.ice>


module MdLive
{
	interface MdSessionCallBack
	{
		/*
		* onTick 		: 每次收到tick数据触发，推送给订阅的客户端
		* TradingDay 	: 交易日
		* InstrumentID 	: 合约名
		* Ticker		: 一条ticker数据
		* Return 		: 返回值0
		*/
		int  onTick(string TradingDay,string InstrumentID, CM::Ticker ticker); 
		
		/*
		* onBar 		: 分k收盘触发 推送给订阅的客户端
		* TradingDay 	: 交易日
		* InstrumentID 	: 合约名
		* bar			: 分k数据
		* Return		: 返回值0
		*/
		int  onBar(string TradingDay,string InstrumentID , CM::Bar bar, int period );
		
		/*
		* onDay 		: 日k收盘触发 推送给订阅的客户端
		* TradingDay 	: 交易日
		* InstrumentID 	: 合约名
		* day			: 日k数据
		* Return		: 返回值0
		*/
		int  onDay(string TradingDay,string InstrumentID , CM::DayBar day );
	
		/*
		* onDepthMarketData : 收到的原始快照数据 推送给订阅的客户端
		* mdata				: 原始的市场数据
		*/
		void  onDepthMarketData(CM::DepthMarketData mdata );
	};
	

	interface MdSession 
	{
		/*
		* setCallBack: 注册客户端
		* cb		: 回调函数句柄
		* Return	: 返回值0成功 
		*/
		int setCallBack(Ice::Identity ident);

		/*
		* heartBeat	: 心跳 维持长连接
		*/
		int   heartBeat();	//客户端调用心跳

		/*
		* subscribeMd: 订阅
		* insid		: 合约名
		* level		: T/M1/D  ticker或者分k
		* return	: 0成功
		* example	: subscribeMd('IF1601','T'); / subscribeMd('IF1601','M1');
		*/
		int subscribeMd(string insid, string level);
		
		/*
		* unSubscribeMd: 退订
		* insid		: 合约名
		* level		: T/M/D/A  ticker或者分k 日k all
		* return	: 0成功
		* example	: unSubscribeMd('IF1601','T'); / unSubscribeMd('IF1601','M1');
		*/
		int unSubscribeMd(string insid, string level);

		/*
		*	sendBar		: 外部接口 发送Bar给mdlive
		*	TradingDay	: 交易日
		*	InstrumentID: 合约名
		*	bar			: 分k数据
		*/
		int sendBar(string TradingDay, string InstrumentID, CM::Bar bar );

		/*
		*	sendDayBar	: 外部接口 发送DayBar给mdlive
		*	TradingDay	: 交易日
		*	InstrumentID: 合约名
		*	bar			: 分k数据
		*/
		int sendDayBar(string TradingDay, string InstrumentID, CM::DayBar day );

		/*
		*	sendTick	: 外部接口 发送Tick给mdlive
		*	TradingDay	: 交易日
		*	InstrumentID: 合约名
		*	Tick		: tick数据
		*/
		int sendTicker(string TradingDay, string InstrumentID, CM::Ticker ticker );


		//server debug tools, not thread safe
		string  QuerySession();				//查询server端的所有session
		string  QuerySubCurrent();			//查询server端当前订阅
		string  QueryTicker(string insid);	//查询server内存中ticker
		string  QueryKline(string insid);	//查询server内存中的Kline数据
		string  QuerySnap(string insid);	//查询所有的快照信息
	};
};

#endif
