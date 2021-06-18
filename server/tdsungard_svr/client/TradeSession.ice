#ifndef		__TRADE_ICE__
#define		__TRADE_ICE__

#include <Ice/Identity.ice>
#include <ice/Common.ice>


module Trade
{
	interface TdSessionCallBack
	{
        /*
		*	onDone ： 推送成交持仓账户信息
		*	In：
		*		fundid	: 基金id
		*		done	: 成交信息
		*		account	: 账户信息
		*		position: 持仓信息列表
		*	Out:
		*		0 	成功
		*		<0	失败
        */
		int onDone(string fundid,CM::Done done, CM::Account account, CM::PositionList position);
		
        /*
		*	onOrder ： 委托下单信息推送
		*	In：
		*		fundid	: 基金id
		*		order	: 委托信息
		*	Out:
		*		0 	成功
		*		<0	失败
        */
		int onOrder(string fundid, CM::Order order);
	
		/*
		*	onOrder ： 委托下单信息推送
		*	In：
		*		fundid	: 基金id
		*		order	: 委托信息
		*	Out:
		*		0 	成功
		*		<0	失败
        */
		int onTradingday(string fundid,  string tradingday);
	
	};
	

	interface TdSession 
	{
		/*
		* 	setCallBack: 注册客户端
		*	In :
		* 		ident		: 客户端初始---ident.ident.name = IceUtil::generateUUID() ; ident.category = "";
		* 	Out : 
		*		0  	成功
		*		<0	失败
		*/
		int setCallBack(Ice::Identity ident);
		
		
		/*
		*	heartBeat	: 心跳 维持长连接 断线重连
		*	Out	:
		*		0 连接正常
		*		!0 表示连接断开
		*/
		int   heartBeat();	//客户端调用心跳

		
		/*
		*	Description	:	查询对应信息 本地计算
		*	In		:
		*		fundid : 基金ID
		*		from   : 数据来源 默认来自本地， 1来自交易所
		*	Out :
		*		对应结构
		*/
		CM::AccountList		queryAccount();
		CM::Account			queryAccountOne(string fundid);
        CM::PositionList	queryPosition(string fundid);
        CM::OrderList		queryOrder(string fundid);
        CM::DoneList		queryDone(string fundid);
        
		
		/*
		*	Description	:	直接查询交易所返回数据 
		*	In		:
		*		fundid : 基金ID
		*		from   : 数据格式 默认本地格式， 1交易所原油格式
		*	Out :
		*		对应json
		*/
		string jsonQueryAccount(int from);
		string jsonQueryAccountOne(string fundid, int from);
    	string jsonQueryPosition(string fundid, int from);
    	string jsonQueryOrder(string fundid, int from);
    	string jsonQueryDone(string fundid, int from);
       
		/*
		 *	doOrder	:	下单
		 *	In		:
		 *		do : 参见结构定义
		 *	Out	:
		 *		返回>=0成功 其他失败
		 *
		 */
		int doOrder(CM::DoOrder do);
 
		/*
		 *	cancleOrder	:	撤单
		 *	In		:
		 *		privateno: 撤单私有号
		 *	Out	:
		 *		返回>=0成功 其他失败
		 *
		 */
		int cancleOrder(string fundid, string sessionid, string instrumentid, string privateno, string orderno);
		
		/*
		 *	updatePassword	:	修改密码
		 *	In		:
		 *		oldpwd	: 老密码
		 *		newpwd	：新密码
		 *	Out		:
		 *		返回0成功， 否则失败
		 */
		int updatePassword(string oldpwd, string newpwd);

		/*
		*	subscribe	: 订阅
		*	In	:
		*		moduleName	: 客户端名字
		*		fundid		: 基金ID
		*	Out	:
		*		0	成功 
		*		<0	失败
		*	example		: subscribe('PositionControl','800528'); 
		*/
		int subscribe(string moduleName,string fundid);
		
		/*
		*	unSubscribe: 退订
		*	In :
		*		fundid		: 基金名字
		*	Out :
		*		0	成功
		*		<0	失败
		*	example	: unSubscribe('800528');
		*/
		int unSubscribe(string fundid);


		/*
		*	getTradingday 获取server的tradingday
		*	In :
		*	Out :
		*	Return : tradingday
		*/
		string getTradingday();
		
		//server debug tools  ----没有用到---
		string  QuerySession();				//查询server端的所有session
		string  QuerySubCurrent();			//查询server端当前订阅
	};
};

#endif
