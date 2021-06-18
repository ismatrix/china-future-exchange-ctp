/////////////////////////////////////////////////////////////////////////
///@system 金仕达新一代资管系统
///@company SunGard China
///@file kfts_trade_type.h
///@brief 定义了金仕达资管系统数据类型
///@history
///20141001	Kingstar IRDG		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef __KFTS_TRADE_TYPE_H__
#define __KFTS_TRADE_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

	/** \defgroup Group11 1.1 数据长度定义 */
	/** @{ */
	/// API版本长度
#define API_VERSION_LEN         20
	/// 信息长度
#define MSG_LEN                 256
	/// 客户号长度
#define CUST_NO_LEN             16
	/// 密码长度
#define PWD_LEN                 129
	/// 合约代码长度
#define INST_ID_LEN            20 
	/// 合约名称长度
#define INST_NAME_LEN            16
	/// 组合合约代码长度
#define INST_COMBID_LEN      81
	/// 备注长度
#define NOTE_LEN                256
	/// 柜台实例号长度
#define INSTANCE_ID_LEN         32
	/// 客户名称长度
#define CUST_NAME_LEN           16
	/// ETF名称长度
#define ETF_NAME_LEN            20
	/// 网卡地址长度
#define MAC_ADDR_LEN            23
	/// 股东账号长度
#define HOLDERNO_LEN			17
	/** @} */

	/** \defgroup Group12 1.2 数据类型定义 */
	/** @{ */
	/// 布尔类型
#ifndef BOOL
	typedef int BOOL;
#endif

	/// 真值
#ifndef TRUE
#define TRUE	1
#endif
	/// 假值
#ifndef FALSE
#define FALSE	0
#endif

	/// API版本类型
	typedef char            TApiVersionType[API_VERSION_LEN];
	/// 错误代码类型
	typedef int             TErrorIDType;
	/// 错误信息类型
	typedef char            TErrorMsgType[MSG_LEN];
	/// 客户号类型
	typedef char            TCustNoType[CUST_NO_LEN];
	/// 客户登录密码类型
	typedef char            TCustPwdType[PWD_LEN];
	/// 终端类型类型
	typedef int             TTerminalTypeType;
	/// SPX交易员检验方式
	typedef int				TSPXCheckUserType;
	/// 备注类型
	typedef char            TNoteType[NOTE_LEN];
	/// 柜台实例号类型
	typedef char            TInstanceIDType[INSTANCE_ID_LEN];
	/// 网卡地址类型
	typedef char            TMacType[MAC_ADDR_LEN];    
	/// 会话号类型
	typedef int             TSessionIDType;
	/// 连接会话号类型
	typedef int             TConnSessionIDType;
	/// 客户姓名类型
	typedef char            TCustNameType[CUST_NAME_LEN];
	/// 交易日期类型
	typedef int             TDateType;
	/// 私有号类型
	typedef int             TPrivateNoType;
	/// 资产单元编号类型
	typedef int             TCellIDType;
	/// 投资组合编号类型
	typedef int             TPortfolioIDType;
	/// 资产单元/投资组合标志类型，0-资产单元,1-投资组合
	typedef int             TAccountTypeType;
	/// 市场代码类型，具体值含义见数据字典-市场代码
	typedef char            TMarketCodeType;
	/// 股东代码类型
	typedef char			THoldernoType[HOLDERNO_LEN];
	/// 合约代码类型
	typedef char            TInstrumentIDType[INST_ID_LEN];
	/// 开平标志类型，具体值含义见数据字典-开平标志类型
	typedef char            TOffsetFlagType;
	/// 买卖类型类型，具体值含义见数据字典-买卖类型
	typedef char            TDirectionType;
	/// 市价单类型类型，具体值含义见数据字典-市价单类型
	typedef char            TMarketOrderFlagType;
	/// 委托价格类型
	typedef double          TPriceType;
	/// 数量类型
	typedef int				TVolumeType;
	/// 委托数量类型
	typedef double          TLargeVolumeType;
	/// 委托属性类型
	typedef int             TOrderPropType;
	/// 策略编号/批号类型
	typedef int             TBatchNoType;
	/// 委托流水号类型
	typedef int             TOrderNoType;
	/// 委托状态类型，具体值含义见数据字典-委托状态
	typedef short           TOrderStatusType;
	/// 委托时间类型
	typedef int             TTimeType;
	/// 成交次数类型
	typedef int             TDoneCountType;    
	/// 成交编号类型
	typedef int             TDoneNoType;    
	/// 货币类型类型，具体值含义见数据字典-货币类型
	typedef char            TCurrencyIDType;
	/// 资金数量类型
	typedef double          TAmountType;
	/// 查询账户资金类型
	typedef int             TQueryAccountProp;
	/// 查询持仓类型
	typedef int             TQueryPositionProp;    
	/// 查询委托属性类型
	typedef int             TQueryOrderPropType;    
	/// 合约名称类型
	typedef char            TInstrumentNameType[INST_NAME_LEN];
	/// 合约状态类型
	typedef short           TSecStatusType;
	/// 品种类型，具体值含义见数据字典-品种类型
	typedef int             TProductTypeType;
	/// ETF简称类型
	typedef char            TEtfNameType[ETF_NAME_LEN];
	/// 单位类型
	typedef int             TUnitType;
	/// 比例类型
	typedef double          TRatioType;
	/// 发布单位净参考值标志类型
	typedef char            TPublishIOPVFlag;
	/// ETF状态类型，具体值含义见数据字典-ETF交易状态
	typedef char            TEtfStatusType;
	/// 篮子股票个数类型
	typedef int             TRecordNumType;
	/// 现金替代标志类型，具体值含义见数据字典-现金替代标志
	typedef char            TCashReplFlagType;
	/// 费用代码替代类型
	typedef int             TFeeCodeType;
	/// 费率计算方式替代类型
	typedef int             TFeeCalcType;
	/// 费率值替代类型
	typedef double          TFeeValueType;
	/// 费率分笔计算标志
	typedef char            TFeeDispartFlag;
	/// 转账类型标志
	typedef char            TTransFundFlag[2];
	/// 强平标记
	typedef int				TForceFlag;
	/// 成交属性,具体值含义见数据字典
	typedef char			TDoneProp;
	/// 指令编号
	typedef int				TInstructID;
	/// 资金账户类型
	typedef int				TAccType;
	/// 投机套保标志
	typedef char			THedgeFlag;
	/// 持仓多空类型
	typedef char			TPosiDirectionType;
	/// 组合策略代码,具体值含义见数据字典
	typedef char			TStrategyCode[11];
	/// 组合类型,具体值含义见数据字典
	typedef char			TCombType;
	/// 委托单类型,具体值含义见数据字典
	typedef char			TFutOrderType;
	/// 组合合约代码类型
	typedef char            TCombinInstrIDType[INST_COMBID_LEN];
	/// 组合合约类型,具体值含义见数据字典
	typedef int			TFutCombFlag;
	/// 合约状态类型,具体值含义见数据字典
	typedef char			TContractStatus;
	/// 可交易状态,具体值含义见数据字典
	typedef char			TContTradeStatus;
	/// 合约索引
	typedef int				TContractIndex;
	/// 委托策略名称
	typedef char			TStrategyName[41];
	/// 组合单腿数
	typedef int				TLegsCnt;
	/// 组合单单腿数量比例
	typedef int				TLegRate;
	/// 组合单单腿计算符号
	typedef int				TLegCalSign;
	/// 下单策略号
	typedef int				TPolicyNo;
	/// 交易类型
	typedef char			TTradeType;
	/// 执行类型
	typedef char			TStrikeModeType;
	/// 期权类型
	typedef char			TOptionsTypeType;
	/// 操作日期类型
	typedef char             TOperDateType[9];
	/// 操作时间类型
	typedef char             TOperTimeType[9];
	/// 锁定方向类型
	typedef char             TLockType;
	/// 合同号
	typedef char             TContractType[20];
	/// 成交号
	typedef char             TTradeIDType[32];
	/// 报价类型
	typedef char             TQuoteType;
	/// 客户端名称和版本号
	typedef char             TTerminalNameType[30];	
	/// 资管系统名称
	typedef char             TSystemNameType[22]; 
	/// trader_id 类型
	typedef int              TTRADER_ID_TYPE; 
	/// 前置机连接id类型
	typedef char             TFRONT_KEY_TYPE[12];
	/// 授权码
	typedef char             TLicenseCodeType[256];
	/// 域编号
	typedef int              TDomainNameType;
	/// 交易所系统委托流水号
	typedef char             TMarketOrderIDType[32];
	/** @} */

	/**
	* \page page_dict 数据字典
	* \par 1. 市场代码(证券/期货/黄金/外汇)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'1'</td><td>上海A股</td></tr>
	* <tr><td>'2'</td><td>深圳A股</td></tr>
	* <tr><td>'3'</td><td>中金所</td></tr>
	* <tr><td>'4'</td><td>上期所</td></tr>
	* <tr><td>'5'</td><td>郑商所</td></tr>
	* <tr><td>'6'</td><td>大商所</td></tr>
	* <tr><td>'7'</td><td>黄金交易所</td></tr>
	* <tr><td>'8'</td><td>外汇交易中心</td></tr>
	* </table>
	* @see TMarketCodeType
	* \par 2. 货币代码(证券/期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'1'</td><td>人民币</td></tr>
	* <tr><td>'2'</td><td>美元</td></tr>
	* <tr><td>'3'</td><td>港币</td></tr>
	* </table>
	* @see TCurrencyIDType
	* \par 3. 买卖方向(证券/期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>买入</td></tr>
	* <tr><td>'1'</td><td>卖出</td></tr>
	* <tr><td>'2'</td><td>申购</td></tr>
	* <tr><td>'3'</td><td>赎回</td></tr>
	* <tr><td>'4'</td><td>质押入库</td></tr>
	* <tr><td>'5'</td><td>质押出库</td></tr>
	* <tr><td>'6'</td><td>转托管</td></tr>
	* <tr><td>'7'</td><td>合并</td></tr>
	* <tr><td>'8'</td><td>分拆</td></tr>
	* <tr><td>'A'</td><td>非公开买转让</td></tr>
	* <tr><td>'B'</td><td>非公开卖转让</td></tr>
	* </table>
	* @see TDirectionType
	* \par 4. 开平标志类型(证券/期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>开仓</td></tr>
	* <tr><td>'1'</td><td>平仓</td></tr>
	* <tr><td>'2'</td><td>平今</td></tr>
	* </table>
	* @see TOffsetFlagType
	* \par 5. 市价单类型(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>股票二级市场限价买卖</td></tr>
	* <tr><td>'1'</td><td>最优五档并撤销委托</td></tr>
	* <tr><td>'2'</td><td>最优五档并转限价(深交所不支持),</td></tr>
	* <tr><td>'3'</td><td>对方最优价格(上交所不支持)</td></tr>
	* <tr><td>'4'</td><td>本方最优价格(上交所不支持)</td></tr>
	* <tr><td>'5'</td><td>即时成交剩余撤销(上交所不支持)</td></tr>
	* <tr><td>'6'</td><td>全额成交或撤销 (上交所不支持)</td></tr>
	* </table>
	* @see TMarketOrderFlagType
	* \par 6. 委托状态(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>0</td><td>未报</td></tr>
	* <tr><td>1</td><td>正报</td></tr>
	* <tr><td>2</td><td>已报</td></tr>
	* <tr><td>3</td><td>已报待撤</td></tr>
	* <tr><td>4</td><td>部成待撤</td></tr>
	* <tr><td>5</td><td>部撤</td></tr>
	* <tr><td>6</td><td>场外撤单</td></tr>
	* <tr><td>7</td><td>部成</td></tr>
	* <tr><td>8</td><td>已成</td></tr>
	* <tr><td>9</td><td>废单</td></tr>
	* <tr><td>10</td><td>场内撤单</td></tr>
	* <tr><td>11</td><td>部撤未成</td></tr>
	* <tr><td>12</td><td>部撤待撤</td></tr>
	* </table>
	* @see TOrderStatusType
	* \par 7. ETF交易状态(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>0</td><td>不允许进行申购和赎回</td></tr>
	* <tr><td>1</td><td>允许申购和赎回</td></tr>
	* <tr><td>2</td><td>允许申购、不允许赎回</td></tr>
	* <tr><td>3</td><td>不允许申购、允许赎回每日开始交易后不允许改变</td></tr>
	* </table>
	* @see TEtfStatusType
	* \par 8. 现金替代标志(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>0</td><td>禁止现金替代（必须有股票）</td></tr>
	* <tr><td>1</td><td>可以进行现金替代（先用股票，股票不足的话用现金替代）</td></tr>
	* <tr><td>2</td><td>必须用现金替代</td></tr>
	* <tr><td>3</td><td>跨市场退补现金替代</td></tr>
	* <tr><td>4</td><td>跨市场必须现金替代</td></tr>
	* <tr><td>5</td><td>非沪深退补现金替代</td></tr>
	* <tr><td>6</td><td>非沪深必须现金替代</td></tr>
	* </table>
	* @see TCashReplFlagType
	* \par 9. 品种类型(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>9</td><td>国债</td></tr>
	* <tr><td>10</td><td>地方债</td></tr>
	* <tr><td>11</td><td>公司债</td></tr>
	* <tr><td>12</td><td>企业债</td></tr>
	* <tr><td>13</td><td>分离交易的可转换公司债</td></tr>
	* <tr><td>14</td><td>可转债</td></tr>
	* <tr><td>15</td><td>资产证券化</td></tr>
	* <tr><td>23</td><td>国债质押</td></tr>
	* <tr><td>24</td><td>地方债质押</td></tr>
	* <tr><td>25</td><td>公司债质押</td></tr>
	* <tr><td>26</td><td>企业债质押</td></tr>
	* <tr><td>27</td><td>可分离转债质押</td></tr>
	* <tr><td>30</td><td>账户式回购</td></tr>
	* <tr><td>42</td><td>主板股票交易</td></tr>
	* <tr><td>43</td><td>中小板股票交易</td></tr>
	* <tr><td>44</td><td>创业板股票交易</td></tr>
	* <tr><td>56</td><td>ETF基金交易</td></tr>
	* <tr><td>57</td><td>ETF基金申赎</td></tr>
	* <tr><td>71</td><td>私募债</td></tr>
	* </table>
	* @see TProductTypeType
	* \par 10. 费用代码(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>1</td><td>佣金</td></tr>
	* <tr><td>2</td><td>过户费</td></tr>
	* <tr><td>3</td><td>印花税</td></tr>
	* <tr><td>4</td><td>交易规费</td></tr>
	* <tr><td>5</td><td>结算费</td></tr>
	* <tr><td>6</td><td>其他费用</td></tr>
	* </table>
	* @see TFeeCodeType
	* \par 11. 费率计算方式(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>1</td><td>成交金额</td></tr>
	* <tr><td>2</td><td>成交面值</td></tr>
	* <tr><td>3</td><td>申报笔数</td></tr>
	* <tr><td>4</td><td>成交笔数</td></tr>
	* <tr><td>5</td><td>成交手数</td></tr>
	* </table>
	* @see TFeeCalcType
	* \par 12. 费率分笔计算标志(证券)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>0</td><td>不分笔</td></tr>
	* <tr><td>1</td><td>分笔</td></tr>
	* </table>
	* @see TFeeDispartFlag
	* \par 13. 定单类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>限价</td></tr>
	* <tr><td>'1'</td><td>市价</td></tr>
	* <tr><td>'2'</td><td>最优价</td></tr>
	* <tr><td>'3'</td><td>市价止损</td></tr>
	* <tr><td>'4'</td><td>市价止赢</td></tr>
	* <tr><td>'5'</td><td>限价止损</td></tr>
	* <tr><td>'6'</td><td>限价止赢</td></tr>
	* <tr><td>'7'</td><td>最优价(对手方最优）</td></tr>
	* <tr><td>'8'</td><td>市价最优五档</td></tr>
	* <tr><td>'K'</td><td>市价转剩余限价</td></tr>
	* </table>
	* @see TMarketOrderFlagType
	* \par 14. 投机套保标志(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>投机/非备兑</td></tr>
	* <tr><td>'1'</td><td>保值/备兑</td></tr>
	* <tr><td>'2'</td><td>套利</td></tr>
	* </table>
	* @see THedgingFlag
	* \par 15. 组合类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>普通组合</td></tr>
	* <tr><td>'1'</td><td>互换组合</td></tr>
	* </table>
	* @see TCombType
	* \par 16. 组合策略代码(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>SP</td><td>期货跨期</td></tr>
	* <tr><td>SPC</td><td>期货跨品种</td></tr>
	* <tr><td>SPX</td><td>期货压榨</td></tr>
	* <tr><td>SPD</td><td>期货跨期</td></tr>
	* <tr><td>IPS</td><td>期货跨品种</td></tr>
	* </table>
	* @see TStrategyCode
	* \par 17. 委托单类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>A</td><td>普通委托</td></tr>
	* <tr><td>B</td><td>批量委托</td></tr>
	* <tr><td>D</td><td>预埋普通委托</td></tr>
	* <tr><td>E</td><td>预埋批量委托</td></tr>
	* <tr><td>J</td><td>组合委托</td></tr>
	* <tr><td>K</td><td>组合批量委托</td></tr>
	* <tr><td>L</td><td>预埋组合委托</td></tr>
	* <tr><td>M</td><td>预埋组合批量委托</td></tr>
	* <tr><td>1</td><td>行权委托</td></tr>
	* </table>
	* @see TFutOrderType
	* \par 18. 成交属性(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>GFD当日有效</td></tr>
	* <tr><td>'1'</td><td>FOK全成或全撤</td></tr>
	* <tr><td>'2'</td><td>FAK剩余即撤销</td></tr>
	* <tr><td>'3'</td><td>IOC即时成交剩余自动撤销</td></tr>
	* <tr><td>'4'</td><td>JFOK剩余即撤销</td></tr>
	* <tr><td>'5'</td><td>剩余转限价</td></tr>
	* </table>
	* @see TDoneProp
	* \par 19. 委托状态(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>0</td><td>未报</td></tr>
	* <tr><td>1</td><td>正报</td></tr>
	* <tr><td>2</td><td>已报</td></tr>
	* <tr><td>3</td><td>已报待撤</td></tr>
	* <tr><td>4</td><td>部成待撤</td></tr>
	* <tr><td>5</td><td>部撤</td></tr>
	* <tr><td>6</td><td>场外撤单</td></tr>
	* <tr><td>7</td><td>部成</td></tr>
	* <tr><td>8</td><td>已成</td></tr>
	* <tr><td>9</td><td>废单</td></tr>
	* </table>
	* @see TOrderStatusType
	* \par 20. 合约状态类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>正常</td></tr>
	* <tr><td>'1'</td><td>只可平仓</td></tr>
	* <tr><td>'2'</td><td>停用</td></tr>
	* </table>
	* @see TContractStatus
	* \par 21. 交易类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>证券</td></tr>
	* <tr><td>'1'</td><td>期货</td></tr>
	* <tr><td>'2'</td><td>期货期权</td></tr>
	* <tr><td>'3'</td><td>个股期权</td></tr>
	* </table>
	* @see TTradeType
	* \par 22. 执行类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'0'</td><td>美式</td></tr>
	* <tr><td>'1'</td><td>欧式</td></tr>
	* </table>
	* @see TStrikeModeType
	* \par 23. 期权类型(期货)
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'C'</td><td>看涨</td></tr>
	* <tr><td>'P'</td><td>看跌</td></tr>
	* </table>
	* @see TOptionsTypeType
	* \par 24. 锁定解锁类型
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'3'</td><td>锁定</td></tr>
	* <tr><td>'4'</td><td>解锁</td></tr>
	* </table>
	* @see TLockType
	* \par 25. 报价类型
	* <table>
	* <tr><td>代码</td><td>意义</td></tr>
	* <tr><td>'1'</td><td>可交易的报价</td></tr>
	* </table>
	* @see TQuoteType
	*/

#ifdef __cplusplus
}
#endif

#endif //__KFTS_TRADE_TYPE_H__
