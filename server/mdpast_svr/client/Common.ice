/////////////////////////////////////////////////////////////////////////
///@system 交易系统
///@company 慧网基金
///@file stdef.ice
///@description  交易公共结构定义
///@history
///20160326    dreamyzhang        创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __COMMON_ICE__
#define __COMMON_ICE__

module CM
{
	////////////////////////////////////行情 start//////////////////////////////////////////////////////////////
	struct Ticker
	{
		long		Timestamp;			/*ACTIONDAY+HH24MISS+.SSS;*/
		double		Price;				/*本次结算价              */
		int			Volume;				/*成交量                  */
		double		Turnover;			/*成交额                  */
		double		OpenInterest;		/*持仓量 未平仓合约       */
		double		TotalVolume;		/*成交量                  */
		double		TotalTurnover;		/*成交额                  */
		double		BidPrice1;			/*买一价                  */
		double		AskPrice1;			/*卖一价                  */
		int			BidVolume1;			/*买一量                  */
		int			AskVolume1;			/*卖一量                  */
	};

	struct Bar
	{
		long		Timestamp;			/*ActionDay+hh24mi00.000 */
		double		High;				/*最高价                 */
		double		Low;				/*最低价                 */
		double		Open;				/*开盘价                 */
		double		Close;				/*收盘价                 */
		int			Volume;				/*成交量                 */
		double		Turnover;			/*成交额                 */
		double		OpenInterest;		/*持仓量 未平仓合约      */
	};

	struct DayBar
	{
		long		Timestamp;			/*TradeDay+hh24mi00*/ 
		double		High;				/*最高价           */
		double		Low;				/*最低价           */
		double		Open;				/*开盘价           */
		double		Close;				/*收盘价           */
		double		Average;			/*日均价           */
		int			Volume;				/*成交量           */
		double		Turnover;			/*成交额           */
		double		Settlement;			/*结算价           */
		double		OpenInterest;		/*持仓量 未平仓合约*/
		double		PreSettlement;		/*上次结算价       */
		double		PreClose;			/*昨收盘           */
		double		PreoOpenInterest;	/*昨持仓量         */
		double		Price;				/*当前价           */
		double      UpperLimit;			/*涨停价           */
		double      LowerLimit;			/*跌停价           */
	};

	////////////////////////////////////行情 end//////////////////////////////////////////////////////////////
	
	
	////////////////////////////////////交易 start//////////////////////////////////////////////////////////////
    
    //账户信息结构体
    struct Account
    {                                                                                   
        string    tradingday      ;    /*交易日              ctp.TradingDay             */              
        string    fundid          ;    /*账户标识            ctp.AccountID/st.cell_id   */   
        string	  parentid		  ;	   /*父基金帐号			 默认0                      */
		double    prebalance      ;    /*昨日余额            ctp.PreBalance/st.dthis_bal*/      
        double    premargin       ;    /*上次占用的保证金    ctp.PreMargin/st.null      */    
        string    requestid       ;    /*接口请求标识                                   */
        double    balance         ;    /*期货结算准备金      ctp.Balance/st.deposite    */  
        double    available       ;    /*可用资金            ctp.Available/st.available */      
        double    margin          ;    /*当前保证金总额      ctp.CurrMargin/st.margin   */ 
        double    incap           ;    /*入金金额            ctp.Deposit/st.incap       */     
        double    outcap          ;    /*出金金额            ctp.Withdraw/st.outcap     */     
        double    commission	  ;    /*开仓手续费                                     */  
        double    closeprofit     ;    /*盯日平仓盈亏合计                               */  
        double    positionprofit  ;    /*盯日持仓盈亏合计                               */  
        string    updatedate      ;    /*信息更新日期                                   */
    };
    //持仓信息表
    struct Position
    {
        string    tradingday      ;    /*交易日              ctp.TradingDay                 */               
        string    fundid          ;    /*账户标识            ctp.InvestorID/st.cell_id      */     
        //string    brokerid      ;    /*期货经纪公司标识    ctp.BrokerID/st.cell_id        */ 
        string    requestid       ;    /*ctp接口请求标识                                    */
        string    instrumentid    ;    /*合约代码            ctp.InstrumentID/st.sec_code   */     
        string    direction       ;    /*多空方向            ctp.PosiDirection/st.bs        */    
        string    hedgeflag       ;    /*投机套保标志        ctp.HedgeFlag/st.hedge_flag    */    
        int       preposition     ;    /*昨日总持仓          ctp.YdPosition/st.pre_total_vol*/       
        int       preholdposition ;    /*昨日余仓                                           */    
        int       position        ;    /*今日总持仓          ctp.Position/st.avail_vol      */       
        int       openvolume      ;    /*今开仓量            ctp.OpenVolume/st.buy_vol      */       
        int       closevolume     ;    /*今平仓量            ctp.CloseVolume/st.sell_vol    */       
        double    openamount      ;    /*今开仓金额          ctp.OpenAmount/st.buy_done_amt */       
        double    closeamount     ;    /*今平仓金额          ctp.CloseAmount/st.sell_done_am*/       
        double    opencost        ;    /*今开仓均价          (ctp.OpenAmount/ctp.OpenVolume)*/               
        double    positioncost    ;    /*持仓成本            ctp.PositionCost/st.total_cost */       
        double    premargin       ;    /*昨天占用的保证金    ctp.PreMargin/st.yd_usemargin  */   
        double    margin          ;    /*当前占用的保证金    ctp.UseMargin/st.total_margin  */      
        double    opencommission  ;    /*开仓手续费-TThostFtdcMoneyType-double              */
        double    closecommission ;    /*平仓手续费-TThostFtdcMoneyType-double              */
        double    closeprofit     ;    /*盯日平仓盈亏                                       */
        double    positionprofit  ;    /*盯日持仓盈亏                                       */
        double    totalprofile    ;    /*累计实现盈亏                                       */
        string	  positiondate	  ;	   /*今仓昨仓 '1' '2' 默认今仓1                         */
		string    updatedate      ;    /*最新更新时间                                       */
    };
    
	//成交信息结构体
    struct Done
    {
        string    tradingday	;    /*交易日             ctp.TradingDay/系统维护       */
        string    fundid        ;    /*基金标识，         ctp.InvestorID/st.cell_id     */
        string    requestid		;    /*接口请求标识                                     */
        string    brokerid		;    /*期货经纪公司标识   ctp.BrokerID/st.cell_id       */
        string    orderid		;    /*委托报单编号       ctp.OrderSysID/st.order_no    */
        string    tradeid		;    /*成交编号           ctp.TradeID/st.trade_id       */
        string    exchangeid	;    /*交易所代码         ctp.ExchangeID/st.-1          */    
        string    instrumentid	;    /*合约代码           ctp.InstrumentID/st.sec_code  */     
        string    direction		;    /*买卖方向           ctp.Direction/st.bs           */
        string    offsetflag	;    /*开平标志           ctp.OffsetFlag/st.offset_flag */ 
        string    hedgeflag		;    /*投机套保标志       ctp.HedgeFlag/st.hedge_flag   */ 
        double    price			;    /*成交价格           ctp.Price/st.done_price       */  
        int       volume        ;    /*成交数量           ctp.Volume/st.done_vol        */
        string    tradedate     ;    /*成交时期yyyymmdd   ctp.TradeDate/st.done_date    */
        string    tradetime		;    /*成交时间hh24miss   ctp.TradeTime/st.done_time    */
        string    updatedate	;    /*纪录最新更新时间   time(NULL)                    */              

		string parentid			;	 /*父基金帐号                                       */
		string strategyid		;	 /*策略标识                                         */
		string userid			;	 /*操作人员标识                                     */
		string signalname		;	 /*"signal.name", //信号名称 参加codemap.signal.name*/
	};
    
	struct Order
    {
		string		frontid			;	/*只有ctp用到                             */
        string		sessionid		;	/*连接id 撤单的时候用到                   */
		string		privateno		;	/*ctp.OrderRef / st.private_no            */
		string      exchangeid		;	/*市场代码	ctp.ExchangID / st.MarketCode */
		string		orderid			;	/*委托流水号 ctp.OrderSysID / st.order_no */
		string		tradingday      ;	/*交易日                                  */
		string		fundid          ;	/*基金id                                  */
		string		brokerid		;	/*经纪                                    */
		string		requestid		;	/*请求id                                  */
		string		instrumentid	;	/*合约ID                                  */
		string		direction		;	/*多空                                    */
		string		offsetflag		;	/*开平标志                                */
		string		hedgeflag		;	/*套保标志                                */
		double		price			;	/*委托价格                                */
		int			volume			;	/*委托数量                                */
		string		ordertype		;	/*委托类型                                */
		string		orderstatus		;	/*报单状态                                */
		int			volumetraded	;	/*今成交数量                              */
		string		insertdatetime	;	/*报单时间 报给期货公司                   */
		string	    ordertime		;	/*委托时间  期货公司给交易所              */
		int			oerrno			;	/*下单 撤单失败 下单-1   撤单-2  0成功    */
		string		oerrmsg			;	/*错误信息                                */
		string		updatetime		;	/*最后修改时间                            */
	
		string parentid				;	/*父基金帐号                              */
		string strategyid			;	/*策略标识                                */
		string userid				;	/*操作人员标识                            */
		string signalname			;	/*"signal.name", 信号名称                 */
	};

	//上期只能限价单  金仕达可以市价 限价。。。
	struct DoOrder
	{
		string fundid;			/*基金帐号
		*/string exchangeid;	/*交易所代码  ["SHFE", "上海期货交易所" ], [ "CZCE", "郑州商品交易所" ], [ "DCE", "大连商品交易所" ], [ "CFFEX", "中国金融期货交易所"]
		*/string brokerid;		/*经纪
		*/string instrumentid;	/*合约
		*/string ordertype;		/*订单类型		[ "0", "市价" ], [ "1", "限价" ], [ "2", "最优价" ], [ "3", "对手方最优" ], [ "4", "市价最优5挡" ] 只有0 1金仕达  ctp只有1  上期的只有1
		*/string direction;		/*多空方向		[ "buy", "买入" ], [ "sell", "卖出" ]
		*/string offsetflag;	/*买卖标志		[ "open", "开仓" ], [ "close", "平仓" ]
		*/string hedgeflag;		/*套保			[ "speculation", "投机/非备兑" ], [ "hedge", "保值/备兑" ], [ "arbitrage", "套利" ]   我们只用到speculation
		*/double price;			/*委托价格
		*/int    volume;		/*委托数量
		*/string donetype;		/*成交类型		[ "0", "GFD当日有效" ], [ "1", "FOK限价全成或全撤" ], [ "2", "FAK限价立即成交剩余撤销" ], [ "3", "IOC立即成交剩余自动撤销" ]  0 2 3有效。 2=3
		*/

		string parentid		;	/*父基金帐号    */
		string strategyid	;	/*策略标识      */
		string userid		;   /*操作人员标识  */
		string signalname	;	/*"signal.name",*/

		string tradingday;
		string requestid;
	};

    sequence<Done>           DoneList;
    sequence<Account>        AccountList;
    sequence<Position>       PositionList;
    sequence<Order>          OrderList;
	////////////////////////////////////交易 end//////////////////////////////////////////////////////////////
};

#endif



















