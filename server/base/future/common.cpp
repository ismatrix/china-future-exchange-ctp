/*************************************************************************
    > File Name: common.cpp
    > Created Time: Fri 17 Jun 2016 02:56:41 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include "common.h"

bool		g_debug = false;
CFile*		g_file = NULL;
Conf		g_conf;
map<string, string> g_keyValye;





//读取mongo合约信息来订阅行情
int  QueryMongoInstrument(Conf& conf, set<string>& idlist)
{
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("INSTRUMENT");

	Json::Value q, in, a;
    a.append("1");
    //a.append("2");
    in["$in"] = a;

	q["productclass"] =  in;		//只处理为1的
	
    q["istrading"] =  1;		//只处理为1的

	if(db.Query(q.toStyledString()) < 0)
	{
		LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		return -1;;
	}

	while(db.Next())
	{
		try
		{
			char* result = db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))
			{

				    LOG_ERROR("reader.parse error. result:" << result);
				    continue;
			}   
			string insid = val["instrumentid"].asString();
			idlist.insert(insid);
			LOG_DEBUG("insert instrumentid:" << insid);
			
			InstrumentData* ist  = GetInstrumentData(insid, true); //创建一个合约位置
			get_CThostFtdcInstrumentField_Struct(val, &ist->instrucment);        //填充合约静态信息
		}
		catch(Json::Exception& e)
		{
			LOG_ERROR(e.what());
		}
		catch(...)
		{
			LOG_ERROR("db.Next() fail.");
		}
	}
	return 0;
}

//查询product
int QueryMongoProduct(list<string>& products)
{
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("PRODUCT");
	if(db.Query("{}") < 0)
	{
		LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		return -1;;
	}

	while(db.Next())
	{
		try
		{
			char* result = db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))
			{
				    LOG_ERROR("reader.parse error. result:" << result);
				    continue;
			}   
			string productid = val["productid"].asString();
			products.push_back(productid);
			
			LOG_DEBUG("insert productid=" << productid << " productname=" << val["productname"].asString());
		}
		catch(Json::Exception& e)
		{
			LOG_ERROR(e.what());
		}
		catch(...)
		{
			LOG_ERROR("db.Next() fail.");
		}
	}
	return 0;
}

int InitConf(Conf& conf)
{
	//默认的日志
	int level = 3, size = 1024000000, num=10;
	conf.get<int>("LOG", "level", level, "3");
	conf.get<int>("LOG", "size", size, "102400000");
	conf.get<int>("LOG", "num", num, "10");
	string name = "";
	conf.get<string>("LOG", "name", name, __FILE__);
	LOG_INIT(name.c_str(), level, size, num);

	//精灵进程和core
	uint32_t core = 0, daemon = 0;
	core = conf.get<uint32_t>("global", "core", core, "0");
	if(core != 0) set_limit_core(core);
	daemon = conf.get<uint32_t>("global", "daemon", daemon, "0");
	if(daemon == 1) set_daemon();

	//默认的mongo
	CMongodb& db = CMongodb::Instance();
	string uri, dbname, collection;
	conf.get<string>("MONGO", "uri", uri, "mongodb://127.0.0.1");
	conf.get<string>("MONGO", "db", dbname, "smartwin");
	conf.get<string>("MONGO", "collection", collection, "DAYBAR");
	db.Init(uri.c_str(), dbname.c_str(), collection.c_str());
	
	string debug = "";
	conf.get<string>("global", "debug", debug, "false");
	if(debug == "true") g_debug = true; 
	
	return 0;
}

void InitMongo(CMongodb& db)
{
	string uri, dbname, collection;
	g_conf.get<string>("MONGO", "uri", uri, "mongodb://127.0.0.1");
	g_conf.get<string>("MONGO", "db", dbname, "smartwin");
	g_conf.get<string>("MONGO", "collection", collection, "DAYBAR");
	db.Init(uri.c_str(), dbname.c_str(), collection.c_str());
}

int InitFile(Conf& conf, CFile* & f)
{
	string path;
	conf.get<string>("global", "path", path, "./");
	f = new CFile(path.c_str(), ".dat");
	return 0;
}


int InitFifo(Conf& conf)
{
	int fifo_size = 20480000;
	conf.get<int>("FIFO", "size", fifo_size, "20480000");//20M
	md_fifo_init(fifo_size);
	return 0;
}



//加载数据库交易数据

int GetAccountInfo(Conf& conf)
{
	string fundid; 
	conf.get<string>("ACCOUNT", "fundid", fundid, "");
	g_keyValye["fundid"] = fundid;
	if(fundid == "")
	{
		LOG_ERROR("fundid error in ini file.");
		exit(-1);
	}
	CMongodb db = CMongodb::Instance();
	db.ChangeCollection("FUND");

	Json::Value q;
	q["fundid"] =  fundid;		
	if(db.Query(q.toStyledString()) < 0)
	{
		LOG_ERROR("mongo query fail. err:" << db.GetLastErr());
		exit(-1);
	}
	while(db.Next())
	{
		try
		{
			char* result = db.GetResult();
			Json::Reader reader;
			Json::Value val;
			if(!reader.parse(result, val))
			{
			    LOG_ERROR("reader.parse error. result:" << result);
			    exit(-1);
			}   

			g_keyValye["password"]		= val["trading"]["password"].asString();
			g_keyValye["account"]		= val["trading"]["account"].asString();
			g_keyValye["brokerid"]		= val["trading"]["brokerid"].asString();
			g_keyValye["service.ip"]		= val["service"]["ip"].asString();
			g_keyValye["service.port"]	= val["service"]["port"].asString();
			g_keyValye["service.key"]		= val["service"]["key"].asString();
			g_keyValye["cellid"]		= val["trading"]["cellid"].asString();	
			
			Json::Value ser = val["trading"]["server"];
			g_keyValye["number"] = toa(ser.size());
			for(uint32_t i=0; i<ser.size(); i++)
			{
				stringstream ss;
				ss << "server" << i;
				g_keyValye[ss.str()] = ser[i].asString();
			}
		}
		catch(Json::Exception& e)
		{
			LOG_ERROR(e.what());
			exit(-1);
		}
		catch(...)
		{
			LOG_ERROR("db.Next() fail.");
			exit(-1);
		}
	}
	
	typeof(g_keyValye.begin()) itr = g_keyValye.begin();
	for(; itr!=g_keyValye.end(); itr++)
	{
		LOG_INFO(itr->first << " = " << itr->second);
	}
	
	LOG_INFO("account=" << g_keyValye["account"] << " brokerid=" << g_keyValye["brokerid"] << " password=" << g_keyValye["password"] << " service.ip=" << g_keyValye["service.ip"] << " service.key=" << g_keyValye["service.key"] << " server0=" << g_keyValye["server0"]);

	if(g_keyValye["password"] == "") exit(-1);
	if(g_keyValye["service.ip"] == "") exit(-1);
	if(g_keyValye["server0"] == "") exit(-1);
	return 0;
}






