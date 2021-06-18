#ifndef		__SUBCRIBRMd_ICE__
#define		__SUBCRIBRMd_ICE__

#include <Ice/Identity.ice>
#include <Common.ice>


module MdPast
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
        * period        : kline周期
		* Return		: 返回值0
		*/
		int  onBar(string TradingDay,string InstrumentID , CM::Bar bar, int period);
		
		/*
		* onDay 		: 日k收盘触发 推送给订阅的客户端
		* TradingDay 	: 交易日
		* InstrumentID 	: 合约名
		* day			: 日k数据
		* Return		: 返回值0
		*/
		int  onDay(string TradingDay,string InstrumentID , CM::DayBar day );
	};
	

	interface MdSession
	{
		/*
		 * queryData		: 查询ticker      
		 * DataType			: 数据类型 T M1----M120  DM
		 * InstrumentID		: 合约名
		 * starttime		: 开始时间 20160531
		 * endtime			: 结束时间 20160601
		 */
		string   queryData(Ice::Identity ident, string DataType, string InstrumentID, string starttime, string endtime);
	};
};

#endif
