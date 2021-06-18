/*************************************************************************
    > File Name: StJsonStruct.cpp
    > Created Time: Tue 31 May 2016 04:36:45 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include "StJsonStruct.h"

Json::Value& get_STAccountInfo(Json::Value& root, STAccountInfo* p)
{
	if(p == NULL) return root;
	//root["tradingday"]			=	tradingday;          	
	//root["requestid"]           =	requestid;          
	root["cell_id"]             =	toa(p->cell_id);          
	root["portfolio_id"]        =	p->portfolio_id;     
	root["account_type"]        =	p->account_type;     
	root["currency_type"]       =	ctos(p->currency_type);    
	root["acc_type"]            =	p->acc_type;		
	root["deposite"]            =	p->deposite;         
	root["available"]           =	p->available;        
	root["buyable"]             =	p->buyable;          
	root["buy_frz_amt"]         =	p->buy_frz_amt;      
	root["buy_done_amt"]        =	p->buy_done_amt;     
	root["sell_done_amt"]       =	p->sell_done_amt;    
	root["dealfrozcap"]         =	p->dealfrozcap;      
	root["abnormal_frzn_amt"]   =	p->abnormal_frzn_amt;
	root["manual_unfrzn_amt"]   =	p->manual_unfrzn_amt;
	root["margin"]              =	p->margin;           
	root["outcap"]              =	p->outcap;           
	root["incap"]               =	p->incap;            
	root["realprofit"]          =	p->realprofit;       
	root["forbid_asset"]        =	p->forbid_asset;     
	root["dthis_bal"]           =	p->dthis_bal;		
	root["manual_frzn_amt"]     =	p->manual_frzn_amt;
	root["royaltyin"]           =	p->RoyaltyIn;        
	root["royaltyout"]          =	p->RoyaltyOut;		
	root["royaltyfrozen"]	    =	p->RoyaltyFrozen;	
	//string ss = fast_writer.write(root);
	return root;
}

STAccountInfo* get_STAccountInfo_Struct(Json::Value& root, STAccountInfo* p)
{
	if(p == NULL) return NULL;
	if(!root["cell_id"].isNull())            p->cell_id            =  atoll(root["cell_id"].asString().c_str());
	if(!root["portfolio_id"].isNull())       p->portfolio_id       =  root["portfolio_id"].asInt();
	if(!root["account_type"].isNull())       p->account_type       =  root["account_type"].asInt();
	if(!root["currency_type"].isNull())      p->currency_type      =  root["currency_type"].asString()[0];
	if(!root["acc_type"].isNull())           p->acc_type		   =  root["acc_type"].asInt();
	if(!root["deposite"].isNull())           p->deposite           =  root["deposite"].asDouble();
	if(!root["available"].isNull())          p->available          =  root["available"].asDouble();
	if(!root["buyable"].isNull())            p->buyable            =  root["buyable"].asDouble();
	if(!root["buy_frz_amt"].isNull())        p->buy_frz_amt        =  root["buy_frz_amt"].asDouble();
	if(!root["buy_done_amt"].isNull())       p->buy_done_amt       =  root["buy_done_amt"].asDouble();
	if(!root["sell_done_amt"].isNull())      p->sell_done_amt      =  root["sell_done_amt"].asDouble();
	if(!root["dealfrozcap"].isNull())        p->dealfrozcap        =  root["dealfrozcap"].asDouble();
	if(!root["abnormal_frzn_amt"].isNull())  p->abnormal_frzn_amt  =  root["abnormal_frzn_amt"].asDouble();
	if(!root["manual_unfrzn_amt"].isNull())  p->manual_unfrzn_amt  =  root["manual_unfrzn_amt"].asDouble();
	if(!root["margin"].isNull())             p->margin             =  root["margin"].asDouble();
	if(!root["outcap"].isNull())             p->outcap             =  root["outcap"].asDouble();
	if(!root["incap"].isNull())              p->incap              =  root["incap"].asDouble();
	if(!root["realprofit"].isNull())         p->realprofit         =  root["realprofit"].asDouble();
	if(!root["forbid_asset"].isNull())       p->forbid_asset       =  root["forbid_asset"].asDouble();
	if(!root["dthis_bal"].isNull())          p->dthis_bal		   =  root["dthis_bal"].asDouble();
	if(!root["manual_frzn_amt"].isNull())    p->manual_frzn_amt	   =  root["manual_frzn_amt"].asDouble();
	if(!root["RoyaltyIn"].isNull())          p->RoyaltyIn          =  root["royaltyin"].asDouble();
	if(!root["RoyaltyOut"].isNull())         p->RoyaltyOut		   =  root["royaltyout"].asDouble();
	if(!root["RoyaltyFrozen"].isNull())      p->RoyaltyFrozen 	   =  root["royaltyfrozen"].asDouble();

	return p;
}
Json::Value& get_STPositionInfo(Json::Value& root, STPositionInfo* p)
{
	if(p == NULL) return root;
	root["cell_id"]					=	toa(p->cell_id);      
	root["portfolio_id"]			=	p->portfolio_id;      
	root["account_type"]            =	p->account_type;      
	root["market_code"]             =	ctos(p->market_code);       
	root["sec_code"]                =	p->sec_code;          
	root["hedgingflag"]             =	ctos(p->hedgingflag);       
	root["posidirection"]           =	ctos(p->PosiDirection);     
	root["pre_total_vol"]           =	p->pre_total_vol;     
	root["pre_remain_vol"]          =	p->pre_remain_vol;    
	root["total_vol"]               =	p->total_vol;         
	root["avail_vol"]               =	p->avail_vol;         
	root["buy_vol"]                 =	p->buy_vol;           
	root["sell_vol"]                =	p->sell_vol;          
	root["total_cost"]              =	p->total_cost;        
	root["avg_price"]               =	p->avg_price;         
	root["commission"]              =	p->commission;        
	root["opencommission"]          =	p->OpenCommission;    
	root["closecommission"]         =	p->CloseCommission;   
	root["realized_profit"]         =	p->realized_profit;   
	root["sell_frz_vol"]            =	p->sell_frz_vol;      
	root["buy_done_amt"]            =	p->buy_done_amt;      
	root["sell_done_amt"]           =	p->sell_done_amt;     
	root["etf_frz_vol"]             =	p->etf_frz_vol;            
	root["etf_rtn_vol"]             =	p->etf_rtn_vol;            
	root["sell_frz_undone_vol"]     =	p->sell_frz_undone_vol;    
	root["etf_frz_undone_vol"]      =	p->etf_frz_undone_vol;     
	root["abnormal_frzn_vol"]       =	p->abnormal_frzn_vol;      
	root["manual_unfrzn_vol"]       =	p->manual_unfrzn_vol;      
	root["mortgage_frozen_vol"]     =	p->mortgage_frozen_vol;    
	root["etf_left_vol"]            =	p->etf_left_vol;           
	root["cur_buy_etf_left_vol"]    =	p->cur_buy_etf_left_vol;   
	root["curr_froz"]               =	p->curr_froz;				
	root["curr_unfroz"]             =	p->curr_unfroz;		
	root["detf_frozen_vol"]         =	p->detf_frozen_vol;    
	root["usemargin"]               =	p->usemargin;		    
	root["yd_usemargin"]            =	p->yd_usemargin;		
	root["total_margin"]            =	p->total_margin;		
	root["discount"]                =	p->discount;		    
	root["total_profit"]            =	p->total_profit;			
	root["tradetype"]               =	ctos(p->TradeType);				
	root["locked_vol"]              =	p->locked_vol;				
	root["avail_lock_vol"]          =	p->avail_lock_vol;			
	root["unlocked_vol"]            =	p->unlocked_vol;			
	root["avail_unlock_vol"]        =	p->avail_unlock_vol;		
	root["coverd_frozen_vol"]       =	p->coverd_frozen_vol;		
	root["margintradefrozenvolume"] =	p->MarginTradeFrozenVolume;
	return root;
}

STPositionInfo* get_STPositionInfo_Struct(Json::Value& root, STPositionInfo* p)
{
	if(p == NULL) return NULL;
	if(!root["cell_id"].isNull())              p->cell_id      = atoll(root["cell_id"].asString().c_str());
	if(!root["portfolio_id"].isNull())         p->portfolio_id = root["portfolio_id"].asInt();
	if(!root["account_type"].isNull())         p->account_type = root["account_type"].asInt();
	if(!root["market_code"].isNull())          p->market_code  = root["market_code"].asString()[0];
	if(!root["sec_code"].isNull())             memcpy(&p->sec_code,  root["sec_code"].asString().c_str(),  sizeof(p->sec_code                ));
	if(!root["hedgingflag"].isNull())          p->hedgingflag         =	root["hedgingflag"].asString()[0];
	if(!root["PosiDirection"].isNull())        p->PosiDirection       =	root["posidirection"].asString()[0];
	if(!root["pre_total_vol"].isNull())        p->pre_total_vol       =	root["pre_total_vol"].asDouble();
	if(!root["pre_remain_vol"].isNull())       p->pre_remain_vol      =	root["pre_remain_vol"].asDouble();
	if(!root["total_vol"].isNull())            p->total_vol           =	root["total_vol"].asDouble();
	if(!root["avail_vol"].isNull())            p->avail_vol           =	root["avail_vol"].asDouble();
	if(!root["buy_vol"].isNull())              p->buy_vol             =	root["buy_vol"].asDouble();
	if(!root["sell_vol"].isNull())             p->sell_vol            =	root["sell_vol"].asDouble();
	if(!root["total_cost"].isNull())           p->total_cost          =	root["total_cost"].asDouble();
	if(!root["avg_price"].isNull())            p->avg_price           =	root["avg_price"].asDouble();
	if(!root["commission"].isNull())           p->commission          =	root["commission"].asDouble();
	if(!root["opencommission"].isNull())       p->OpenCommission      =	root["opencommission"].asDouble();
	if(!root["closecommission"].isNull())      p->CloseCommission     =	root["closecommission"].asDouble();
	if(!root["realized_profit"].isNull())      p->realized_profit     =	root["realized_profit"].asDouble();
	if(!root["sell_frz_vol"].isNull())         p->sell_frz_vol        =	root["sell_frz_vol"].asDouble();
	if(!root["buy_done_amt"].isNull())         p->buy_done_amt        =	root["buy_done_amt"].asDouble();
	if(!root["sell_done_amt"].isNull())        p->sell_done_amt       =	root["sell_done_amt"].asDouble();
	if(!root["etf_frz_vol"].isNull())          p->etf_frz_vol         = root["etf_frz_vol"].asDouble();
	if(!root["etf_rtn_vol"].isNull())          p->etf_rtn_vol         = root["etf_rtn_vol"].asDouble();
	if(!root["sell_frz_undone_vol"].isNull())  p->sell_frz_undone_vol = root["sell_frz_undone_vol"].asDouble();
	if(!root["etf_frz_undone_vol"].isNull())   p->etf_frz_undone_vol  = root["etf_frz_undone_vol"].asDouble();
	if(!root["abnormal_frzn_vol"].isNull())    p->abnormal_frzn_vol   = root["abnormal_frzn_vol"].asDouble();
	if(!root["manual_unfrzn_vol"].isNull())    p->manual_unfrzn_vol   = root["manual_unfrzn_vol"].asDouble();
	if(!root["mortgage_frozen_vol"].isNull())  p->mortgage_frozen_vol = root["mortgage_frozen_vol"].asDouble();
	if(!root["etf_left_vol"].isNull())         p->etf_left_vol        = root["etf_left_vol"].asDouble();
	if(!root["cur_buy_etf_left_vol"].isNull()) p->cur_buy_etf_left_vol= root["cur_buy_etf_left_vol"].asDouble();
	if(!root["curr_froz"].isNull())            p->curr_froz			  = root["curr_froz"].asDouble();
	if(!root["curr_unfroz"].isNull())          p->curr_unfroz		  =	root["curr_unfroz"].asDouble();
	if(!root["detf_frozen_vol"].isNull())      p->detf_frozen_vol     =	root["detf_frozen_vol"].asDouble();
	if(!root["usemargin"].isNull())            p->usemargin		      =	root["usemargin"].asDouble();
	if(!root["yd_usemargin"].isNull())         p->yd_usemargin		  =	root["yd_usemargin"].asDouble();
	if(!root["total_margin"].isNull())         p->total_margin		  =	root["total_margin"].asDouble();
	if(!root["discount"].isNull())             p->discount		      =	root["discount"].asDouble();
	if(!root["total_profit"].isNull())         p->total_profit		  = root["total_profit"].asDouble();
	if(!root["tradetype"].isNull())            p->TradeType			  = root["tradetype"].asString()[0];
	if(!root["locked_vol"].isNull())           p->locked_vol		  = root["locked_vol"].asDouble();
	if(!root["avail_lock_vol"].isNull())       p->avail_lock_vol	  = root["avail_lock_vol"].asDouble();
	if(!root["unlocked_vol"].isNull())         p->unlocked_vol		  = root["unlocked_vol"].asDouble();
	if(!root["avail_unlock_vol"].isNull())     p->avail_unlock_vol	  = root["avail_unlock_vol"].asDouble();
	if(!root["coverd_frozen_vol"].isNull())    p->coverd_frozen_vol	  = root["coverd_frozen_vol"].asDouble();
	if(!root["margintradefrozenvolume"].isNull()) p->MarginTradeFrozenVolume = root["margintradefrozenvolume"].asDouble();
	return p;
}

Json::Value& get_STDoneInfo(Json::Value& root, STDoneInfo* p)
{
	if(p == NULL) return root;
	root["done_no"]				=	p->done_no;           
	root["trade_id"]            =	p->trade_id;			
	root["session_id"]          =	p->session_id;        
	root["private_no"]          =	p->private_no;        
	root["cell_id"]             =	toa(p->cell_id);           
	root["portfolio_id"]        =	p->portfolio_id;      
	root["market_code"]         =	ctos(p->market_code);       
	root["sec_code"]            =	p->sec_code;          
	root["batch_no"]            =	p->batch_no;          
	root["order_no"]            =	p->order_no;          
	root["done_price"]          =	p->done_price;        
	root["done_vol"]            =	p->done_vol;          
	root["done_amount"]         =	p->done_amount;       
	root["fund_change_amount"]  =	p->fund_change_amount;
	root["stock_vol_amount"]    =	p->stock_vol_amount;  
	char buf[32]={0};
	snprintf(buf, sizeof(buf), "%02d%02d%02d", p->done_time/10000, p->done_time/100%100, p->done_time%100);
	root["done_time"]           = buf;   
	root["done_date"]           =	toa(p->done_date);			
	root["margin"]              =	p->Margin;            
	root["bs"]                  =	ctos(p->bs);                
	root["offset_flag"]         =	ctos(p->offset_flag);       
	root["hedge_flag"]          =	ctos(p->hedge_flag);			
	root["trade_type"]          =	ctos(p->trade_type);		
	return root;
}

STDoneInfo* get_STDoneInfo_Struct(Json::Value& root, STDoneInfo* p)
{
	if(p == NULL) return NULL;
	if(!root["done_no"].isNull())            p->done_no            	=  root["done_no"].asInt();
	if(!root["trade_id"].isNull())           memcpy(&p->trade_id			,  root["trade_id"].asString().c_str()           ,	sizeof(p->trade_id			));
	if(!root["session_id"].isNull())         p->session_id         	=  root["session_id"].asInt();
	if(!root["private_no"].isNull())         p->private_no        	=  root["private_no"].asInt();
	if(!root["cell_id"].isNull())            p->cell_id           	=  atoll(root["cell_id"].asString().c_str());
	if(!root["portfolio_id"].isNull())       p->portfolio_id      	=  root["portfolio_id"].asInt();
	if(!root["market_code"].isNull())        p->market_code       	=  root["market_code"].asString()[0];
	if(!root["sec_code"].isNull())           memcpy(&p->sec_code          	,  root["sec_code"].asString().c_str()           ,	sizeof(p->sec_code          ));
	if(!root["batch_no"].isNull())           p->batch_no          	=  root["batch_no"].asInt();
	if(!root["order_no"].isNull())           p->order_no          	=  root["order_no"].asInt();
	if(!root["done_price"].isNull())         p->done_price        	=  root["done_price"].asDouble();
	if(!root["done_vol"].isNull())           p->done_vol          	=  root["done_vol"].asDouble();
	if(!root["done_amount"].isNull())        p->done_amount       	=  root["done_amount"].asDouble();
	if(!root["fund_change_amount"].isNull()) p->fund_change_amount	=  root["fund_change_amount"].asDouble();
	if(!root["stock_vol_amount"].isNull())   p->stock_vol_amount  	=  root["stock_vol_amount"].asDouble();
	if(!root["done_time"].isNull())          p->done_time         	=  atoll(root["done_time"].asString().c_str());
	if(!root["done_date"].isNull())          p->done_date			=  atoll(root["done_date"].asString().c_str());
	if(!root["Margin"].isNull())             p->Margin            	=  root["margin"].asDouble();
	if(!root["bs"].isNull())                 p->bs                	=  root["bs"].asString()[0];
	if(!root["offset_flag"].isNull())        p->offset_flag       	=  root["offset_flag"].asString()[0];
	if(!root["hedge_flag"].isNull())         p->hedge_flag			=  root["hedge_flag"].asString()[0];
	if(!root["trade_type"].isNull())         p->trade_type		 	=  root["trade_type"].asString()[0];

	return p;
}

Json::Value& get_STOrderInfo(Json::Value& root, STOrderInfo* p)
{
	if(p == NULL) return root;
	root["session_id"]          =	p->session_id;          
	root["batch_no"]            =	p->batch_no;            
	root["order_no"]            =	p->order_no;            
	root["order_status"]        =	p->order_status;        
	root["frozen_amount"]       =	p->frozen_amount;       
	root["frozen_vol"]          =	p->frozen_vol;          
	root["done_amount"]         =	p->done_amount;         
	root["done_vol"]            =	p->done_vol;            
	root["cancel_vol"]          =	p->cancel_vol;          
	root["order_date"]          =	p->order_date;			
	root["order_time"]          =	p->order_time;          
	root["cancel_time"]         =	p->cancel_time;         
	root["cancel_submit_time"]  =	p->cancel_submit_time;  
	root["error_no"]            =	p->error_no;            
	root["err_msg"]             =	p->err_msg;             
	root["cancel_error_no"]     =	p->cancel_error_no;     
	root["cancel_err_msg"]      =	p->cancel_err_msg;      
	root["order_submit_time"]   =	p->order_submit_time;   
	root["fee"]                 =	p->fee;                 
	root["dis_fee"]             =	p->dis_fee;			
	root["frozenroyalty"]       =	p->FrozenRoyalty;       
	root["quotelocalid"]        =	p->QuoteLocalID;	

	//Json::Value order;
	root["private_no"]		    =	p->order.private_no;         
	root["cell_id"]             =	toa(p->order.cell_id);            
	root["portfolio_id"]        =	p->order.portfolio_id;       
	root["market_code"]         =	ctos(p->order.market_code);        
	root["sec_code"]            =	p->order.sec_code;           
	root["offset_flag"]         =	ctos(p->order.offset_flag);        
	root["bs"]                  =	ctos(p->order.bs);                 
	root["market_order_flag"]   =	ctos(p->order.market_order_flag);  
	root["price"]               =	p->order.price;              
	root["order_vol"]           =	p->order.order_vol;          
	root["order_prop"]          =	p->order.order_prop;         
	root["doneprop"]            =	ctos(p->order.doneprop);           
	root["instructid"]          =	p->order.instructid;         
	root["ordertype"]           =	ctos(p->order.OrderType);			
	root["hedge_flag"]          =	ctos(p->order.hedge_flag);			
	root["policy_no"]           =	p->order.policy_no;			
	root["tradetype"]           =	ctos(p->order.TradeType);	

	
	return root;
}



STOrderInfo* get_STOrderInfo_Struct(Json::Value& root, STOrderInfo* p)
{
	if(p == NULL) return NULL;
	
	if(!root["session_id"].isNull())		 p->session_id              =	root["session_id"].asInt();      
	if(!root["batch_no"].isNull())           p->batch_no                =	root["batch_no"].asInt();        
	if(!root["order_no"].isNull())           p->order_no                =	root["order_no"].asInt();        
	if(!root["order_status"].isNull())       p->order_status            =	root["order_status"].asInt();    
	if(!root["frozen_amount"].isNull())      p->frozen_amount           =	root["frozen_amount"].asDouble();
	if(!root["frozen_vol"].isNull())         p->frozen_vol              =	root["frozen_vol"].asDouble();
	if(!root["done_amount"].isNull())        p->done_amount             =	root["done_amount"].asDouble();
	if(!root["done_vol"].isNull())           p->done_vol                =	root["done_vol"].asDouble();
	if(!root["cancel_vol"].isNull())         p->cancel_vol              =	root["cancel_vol"].asInt();
	if(!root["order_date"].isNull())         p->order_date              =	root["order_date"].asInt();
	if(!root["order_time"].isNull())         p->order_time              =	root["order_time"].asInt();
	if(!root["cancel_time"].isNull())        p->cancel_time             =	root["cancel_time"].asInt();
	if(!root["cancel_submit_time"].isNull()) p->cancel_submit_time      =	root["cancel_submit_time"].asInt();
	if(!root["error_no"].isNull())           p->error_no                =	root["error_no"].asInt();
	if(!root["err_msg"].isNull())            memcpy(&p->err_msg, root["err_msg"].asString().c_str(), sizeof(p->err_msg)); 
	if(!root["cancel_error_no"].isNull())    p->cancel_error_no         =	root["cancel_error_no"].asInt();
	if(!root["cancel_err_msg"].isNull())     memcpy(&p->cancel_err_msg,	root["cancel_err_msg"].asString().c_str(), sizeof(p->cancel_err_msg));  
	if(!root["order_submit_time"].isNull())  p->order_submit_time       =	root["order_submit_time"].asDouble();
	if(!root["fee"].isNull())                p->fee                     =	root["fee"].asDouble();
	if(!root["dis_fee"].isNull())            p->dis_fee                 =	root["dis_fee"].asDouble();
	if(!root["frozenroyalty"].isNull())      p->FrozenRoyalty           =	root["frozenroyalty"].asDouble();
	if(!root["quotelocalid"].isNull())       p->QuoteLocalID            =	root["quotelocalid"].asInt();    
																
	if(!root["private_no"].isNull())         p->order.private_no        =	root["private_no"].asInt();  
	if(!root["cell_id"].isNull())            p->order.cell_id           =	atoll(root["cell_id"].asString().c_str()); 
	if(!root["portfolio_id"].isNull())       p->order.portfolio_id      =	root["portfolio_id"].asInt();  
	if(!root["market_code"].isNull())        p->order.market_code      =	root["market_code"].asString()[0];
	if(!root["sec_code"].isNull())           memcpy(&p->order.sec_code,root["sec_code"].asString().c_str(), sizeof(p->order.sec_code));        
	if(!root["offset_flag"].isNull())        p->order.offset_flag      =	root["offset_flag"].asString()[0];
	if(!root["bs"].isNull())                 p->order.bs               =	root["bs"].asString()[0];
	if(!root["market_order_flag"].isNull())  p->order.market_order_flag=	root["market_order_flag"].asString()[0];
	if(!root["price"].isNull())              p->order.price             =	root["price"].asDouble(); 
	if(!root["order_vol"].isNull())          p->order.order_vol         =	root["order_vol"].asDouble(); 
	if(!root["order_prop"].isNull())         p->order.order_prop        =	root["order_prop"].asInt(); 
	if(!root["doneprop"].isNull())           p->order.doneprop			=	root["doneprop"].asString()[0];
	if(!root["instructid"].isNull())         p->order.instructid        =	root["instructid"].asInt();      
	if(!root["ordertype"].isNull())          p->order.OrderType			=	root["ordertype"].asString()[0];
	if(!root["hedge_flag"].isNull())         p->order.hedge_flag		=	root["hedge_flag"].asString()[0];
	if(!root["policy_no"].isNull())          p->order.policy_no         =	root["policy_no"].asInt();       
	if(!root["tradetype"].isNull())          p->order.TradeType			=	root["tradetype"].asString()[0];
	
	return p;
}

