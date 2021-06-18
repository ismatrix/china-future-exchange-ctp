/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 慧网基金
///@file CTPTradeDataProcesscpph
///@brief ctp查询工具 包含查询 下单撤单
///@history 
///20160326    dreamyzhang        创建该文件
/////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <getopt.h>

#include "ut_str.h"
#include "conf.h"
#include "util.h"
#include "mongoc_json.h"
#include "encrypt/encrypt_password.h"
#include "CTPTradeDataProcess.h"



void load_config()
{
	Conf conf;
	if(!conf.load("./update_instrment_tools.ini"))
	{
		printf("load update_instrment_tools.ini fail.\n");	
		exit(-1);
	}

	int level = 3, size = 1024000000, num=10;
	conf.get<int>("LOG", "level", level, "3");
	conf.get<int>("LOG", "size", size, "102400000");
	conf.get<int>("LOG", "num", num, "10");
	string name = "";
	conf.get<string>("LOG", "name", name, __FILE__);
	LOG_INIT(name.c_str(), level, size, num);
	
	CMongodb& db = CMongodb::Instance();
	string uri, dbname, collection;
	conf.get<string>("MONGO", "uri", uri, "mongodb://127.0.0.1");
	conf.get<string>("MONGO", "db", dbname, "smartwin");
	conf.get<string>("MONGO", "collection", collection, "INSTRUMENT");
	db.Init(uri.c_str(), dbname.c_str(), collection.c_str());                          
}

void process_I(string brokerid, string account, string password, string address, string authcode, string UserProductInfo, string appid)
{
	//set_daemon();	//设置守护进程
	
	uint32_t tradingday = 0;
	while(true)
	{
		CTPTradeDataProcess ctp(brokerid, account, password, authcode, UserProductInfo, appid);
		ctp.set_front(address.c_str());
		ctp.Init(TERT_QUICK);

		int c = 0, ret1=0, ret2=0;
		while(!ctp.get_InitFlag() && c++ < 5) msleep(1000);	

		if(!ctp.get_InitFlag()) goto L1;

		if(tradingday == ctp.tradingday) goto L1;
		
		ret1 = ctp.ReqQryInstrument();
		if(ctp.WaitReq(ret1, 60000) < 0)
		{
			printf("ReqQryInstrument  fail.ret1=%d\n", ret1);
			goto L1;
		}
		ret2 = ctp.ReqQryProduct();	
		if(ctp.WaitReq(ret2, 60000) < 0)
		{
			printf("ReqQryProduct fail.ret2=%d\n", ret2);
			goto L1;
		}

		printf("change tradingday=%u -------> ctp.tradingday=%u\n", tradingday, ctp.tradingday);
		tradingday = ctp.tradingday;
	    exit(0);

	L1:	ctp.Release();
		sleep(180);
	}
}


				
void usage_ctp()
{
    puts("usage: short options\n");
    puts("     -h       ------>     --help           ");
    puts("     -b       ------>     --brokerid       ");
    puts("     -u       ------>     --user           ");
    puts("     -p       ------>     --pwd            ");
    puts("     -a       ------>     --address like tcp://180.168.146.187:1000");
    puts("     -i       ------>     --instrument     ");
    puts("     -P       ------>     --product        ");
    puts("     -o       ------>     --order          ");
    puts("     -t       ------>     --trader         ");
    puts("     -A       ------>     --account        ");
    puts("     -s       ------>     --position       ");
    puts("     -D       ------>     --position_detail");
    puts("     -S       ------>     --settlement     ");
    puts("     -d       ------>     --doorder        ");
    puts("     -c       ------>     --cancleorder    \n");
    puts("     -I       ------>     --daomon_update_instrument    \n");
   
	puts("long options:\n");
	puts("     --help               print help info");   
	puts("     --brokerid           ");
	puts("     --user               account of ctp");   
	puts("     --pwd                password of ctp");   
	puts("     --address            address for connection");   
	puts("     --instrument         query info from ctp");
	puts("     --product            query info from ctp");
	puts("     --order              query info from ctp");   
	puts("     --trader             query info from ctp");   
	puts("     --account            query info from ctp");
	puts("     --position           query info from ctp");
	puts("     --position_detail    query info from ctp");
	puts("     --settlement         confirm the settlement forevery day");
	puts("     --doorder            do order");
	puts("     --cancleorder        cancle order");
	
	puts("run as daomon progress\n");
	puts("     --daomon_update_instrument\n");

	puts("update_instrment_tools.ini in current direction need by --daomon_update_instrument and --tdlive\n");
	puts("    [MONGO]");
	puts("    uri=mongodb://127.0.0.1");
	puts("    db=smartwin");
	puts("    collection=INSTRUMENT\n");
	
	puts("example : ./update_instrment_tools --ctp -b 9999 -u 068074 -p 123456  -a tcp://180.168.146.187:10000 -i -P -o   ");
	puts("example : ./update_instrment_tools --ctp -b 9999 -u 068074 -p 123456  -a tcp://180.168.146.187:10000 --daomon_update_instrument\n");
}

//直接请求ctp查询
void process_ctp(int argc, char* argv[], int argv_index)
{
	if(argc < argv_index + 4)
	{
		usage_ctp();
		exit(-1);
	}
	string brokerid="", account="", password="", address="", authcode="", UserProductInfo="", appid="";
	static bool ctpinit = false;
	CTPTradeDataProcess* ctp = NULL;
	
	char* const short_options = (char*)"hb:u:p:a:iPotAsDSd:c:IC:U:m:";
	struct option long_options[] = {
		{  "help"		    , 0, NULL, 'h' },
		{  "brokerid"		, 1, NULL, 'b' },
		{  "user"			, 1, NULL, 'u' },
		{  "pwd"			, 1, NULL, 'p' },
		{  "adress"			, 1, NULL, 'a' },
		{  "authcode"		, 1, NULL, 'C' },
		{  "productinfo"	, 1, NULL, 'U' },
		{  "appid"			, 1, NULL, 'm' },
		{  "instrument"		, 0, NULL, 'i' },
		{  "product"		, 0, NULL, 'P' },
		{  "order"			, 0, NULL, 'o' },
		{  "trader"			, 0, NULL, 't' },
		{  "account"		, 0, NULL, 'A' },
		{  "position"		, 0, NULL, 's' },
		{  "position_detail", 0, NULL, 'D' },
		{  "settlement"		, 0, NULL, 'S' },
		{  "doorder"		, 1, NULL, 'd' },
		{  "cancleorder"	, 1, NULL, 'c' },
		{  "daomon_update_instrument"	, 0, NULL, 'I' },
		{  0, 0, 0, 0},
	};

#define CHECK_CTP(option) if(!ctpinit) { printf("error option:%s, brokerid:%s, account:%s, password:%s, address:%s, authcode:%s, userinfo:%s, appid:%s\n", option, brokerid.c_str(), account.c_str(), password.c_str(), address.c_str(), authcode.c_str(), UserProductInfo.c_str(), appid.c_str()); exit(-1);} 

	int index, oc, ret=0;
	while((oc = getopt_long(argc, argv, short_options, long_options, &index)) != -1)
	{
		switch(oc)
		{
			case 'h':
			case 0:
				usage_ctp();break;
			case 'b':
				brokerid = optarg; break;
			case 'u':
				account = optarg; break;
			case 'p':
				password = optarg; break;
			case 'a':
				address = optarg; break;
			case 'C':
				authcode = optarg; break;
			case 'U':
				UserProductInfo = optarg; break;
            case 'm':
                appid = optarg; break;
			case 'i':
				CHECK_CTP("ReqQryInstrument")	
				ret = ctp->ReqQryInstrument(); break;
			case 'P':
				CHECK_CTP("ReqQryProduct")
				ret = ctp->ReqQryProduct(); break;
			case 'o':
				CHECK_CTP("ReqQryOrder")
				ret = ctp->ReqQryOrder(); break;
			case 't':
				CHECK_CTP("ReqQryTrade")
				ret = ctp->ReqQryTrade(); break;
			case 'A':
				CHECK_CTP("ReqQryTradingAccount")
				ret = ctp->ReqQryTradingAccount(); break;
			case 's':
				CHECK_CTP("ReqQryInvestorPosition")	
				ret = ctp->ReqQryInvestorPosition();break;
			case 'D':
				CHECK_CTP("ReqQryInvestorPositionDetail")
				ret = ctp->ReqQryInvestorPositionDetail();
			case 'S':
				CHECK_CTP("ReqSettlementInfoConfirm")
				ret = ctp->ReqSettlementInfoConfirm(); break;
			case 'd':
				CHECK_CTP("DoOrder")
				//ret = ctp->DoOrder(CThostFtdcInputOrderField& order);
				break;
			case 'c':
				CHECK_CTP("DoCancleOrder")
				//ret = ctp->DoCancleOrder(CThostFtdcInputOrderActionField& ordaction);
				break;
			case 'I':
				if(ctp != NULL) ctp->Release();
				process_I(brokerid, account, password, address, authcode, UserProductInfo, appid);
				break;
			case '?':
				printf("arguments  %c  error!\n", (char)optopt); break;
			case ':': 
				printf("no options  param!\n"); break;
			default:
			{
				usage_ctp();
			}
		}
		
		if(ret != 0)
		{
			if(ctp->WaitReq(ret, 100000) < 0) 
				cout << "option=" << (char)oc << "  ret=" << ret  << " fail.\n";
			ret = 0;
		}

		if(!ctpinit && !brokerid.empty() && !account.empty() && !password.empty() && !address.empty() && !authcode.empty() && !appid.empty())
		{
			ctpinit = true;
			ctp = new CTPTradeDataProcess(brokerid, account, password, authcode, UserProductInfo, appid);
			ctp->set_front(address.c_str());
			ctp->Init(TERT_QUICK);
		
			int c = 0;
			while(!ctp->get_InitFlag() && c++ < 5) msleep(10000);	
			
			if(!ctp->get_InitFlag()) 
			{
				cout << "ctp init fail.";
				return ;
			}

		}
	}
}


void usage_main()
{
    puts("usage: \n");
    puts("     -h            help info          ");
	puts("     --ctp         ctp options        ");   
	puts("     -pwd_enc      pwd encode         ");
	puts("     -pwd_dec      pwd decode         ");
	puts("example : ./update_instrment_tools --ctp ...");
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		usage_main();
		return -1;
	}
	load_config();
	
	char* const short_options = (char*)"he:d:";
	struct option long_options[] = {
		{  "help"		    , 0, NULL, 'h' },
		{  "ctp"			, 0, NULL, 0 },
		{  "pwd_enc"		, 1, NULL, 'e'},
		{  "pwd_dec"		, 1, NULL, 'd'},
		{  0, 0, 0, 0},
	};

	string password;
	int index, oc;
	while((oc = getopt_long(argc, argv, short_options, long_options, &index)) != -1)
	{
		switch(oc)
		{
			case 'h':
				usage_main();break;
			case 0:
			{
				const char* l = long_options[index].name;
				if(strncmp(l, "ctp", 3) == 0)
				{
					process_ctp(argc, argv, optind); return 0;
				}
				else
				{
					usage_main();
				}
				break;
			}
			case 'e':
			{
				password = optarg;
				cout  << "password = " << password <<  "  encrypt = " << encrypt_account_password((const uint8_t*)password.c_str(), password.size()) << endl;
				break;
			}
			case 'd':
			{
				password = optarg;
				cout  << "string = " << password <<  "  decrypt = " << decrypt_account_password((const uint8_t*)password.c_str(), password.size()) << endl;
				break;
			}
			case '?':
				printf("arguments  %c  error!\n", (char)optopt); break;
			case ':': 
				printf("no options  param!\n"); break;
			default:
			{
				usage_main();
			}
		}
	}

    return 0;
}








