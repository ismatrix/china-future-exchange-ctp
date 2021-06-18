/*************************************************************************
    > File Name: file.cpp
    > Created Time: Thu 16 Jun 2016 03:51:57 PM CST
    > Author: 
    > description: 
 ************************************************************************/

#include<iostream>

#include "file.h"

using namespace std;


//Open("/data/", "20160526", "IF1606", "M1")
int CFile::Open(string tradingday, string instrument, string module, string _suffix, int read)
{
	if(_filename != "")
	{
		_filename = "";
		fs.close();
		return -1;
	}

	string prefix = _prefix + "/" + instrument;
	if(access(prefix.c_str(), F_OK) != 0)
	{
		if(read == 1) 
		{
			LOG_INFO(prefix << " not exist.");
			return -1;
		}

		create_dir(prefix);
		LOG_DEBUG("create dir " << prefix);
	}

	_filename = prefix + "/" + module +instrument +  "_" + tradingday  + _suffix;
	LOG_INFO("open file " << _filename);
	if(read == 1)
		fs.open(_filename.c_str(), ios::in);
	else
		fs.open(_filename.c_str(), ios::app|ios::out|ios::in);

	if(!fs.is_open()) return -3;

	//设置小数输出 3位小数
	fs << setiosflags(ios::fixed) << setprecision(3);

	return 0;
}

int CFile::Close()
{
	_filename = "";
	fs.close();
	return 0;
}

int CFile::WriteTxt(list< ::CM::Ticker>& ticker, string tradingday, string instrument)
{
	Open(tradingday, instrument, "T_", ".txt");
	list< ::CM::Ticker>::iterator itr = ticker.begin();
	for(; itr!=ticker.end(); )
	{
		fs	<< itr->Timestamp     << " "      
			<< itr->Price         << " "
			<< itr->Volume        << " "
			<< itr->Turnover      << " "
			<< itr->OpenInterest  << " "
			<< itr->TotalVolume   << " "
			<< itr->TotalTurnover << " "
			<< itr->BidPrice1     << " "
			<< itr->AskPrice1     << " "
			<< itr->BidVolume1    << " "
			<< itr->AskVolume1    << "\n";  
	//	LOG_DEBUG("write ticker instrument=" << instrument << " itr->Volume:" << itr->Volume << " itr->Turnover:"<<itr->Turnover);
		ticker.erase(itr++);
	}
	Close();
	return 0;
}

int CFile::WriteDat(list< ::CM::Ticker>& ticker, string tradingday, string instrument)
{
	Open(tradingday, instrument, "T_",  ".dat");
	list< ::CM::Ticker>::iterator itr = ticker.begin();
	for(; itr!=ticker.end(); )
	{
		fs.write((const char*)&(*itr), sizeof(::CM::Ticker));
		ticker.erase(itr++);
	}
	Close();
	return 0;
}

int CFile::ReadTxt(list< ::CM::Ticker>& ticker, string tradingday, string instrument)
{
	int ret = Open(tradingday, instrument, "T_",  ".txt", 1);
	if(ret < 0) return ret;
	
	::CM::Ticker t;
	while(fs>>  t.Timestamp
			>>  t.Price
			>>  t.Volume
			>>  t.Turnover
			>>  t.OpenInterest
			>>  t.TotalVolume
			>>  t.TotalTurnover
			>>  t.BidPrice1
			>>  t.AskPrice1
			>>  t.BidVolume1
			>>  t.AskVolume1
	)
	{
		ticker.push_back(t);
	}
	Close();
	return 0;
}

int CFile::ReadDat(list< ::CM::Ticker>& ticker, string tradingday, string instrument)
{
	int ret = Open(tradingday, instrument, "T_",  ".txt", 1);
	if(ret < 0) return ret;

	::CM::Ticker t;
	while(fs.read((char*)&t, sizeof(::CM::Ticker)))
	{
		ticker.push_back(t);
	}
	return 0;
}

int CFile::WriteTxt(list< ::CM::Bar>& bar, string tradingday, string instrument)
{
	Open(tradingday, instrument, "M1_",  ".txt");
	list< ::CM::Bar>::iterator itr = bar.begin();
	for(; itr!=bar.end(); )
	{
		fs	<< itr->Timestamp << " "
			<< itr->High      << " "
			<< itr->Low       << " "
			<< itr->Open      << " "
			<< itr->Close     << " "
			<< itr->Volume    << " "  
			<< itr->Turnover  << " "
			<< itr->OpenInterest  << "\n";
		bar.erase(itr++);
	}
	Close();
	return 0;
}

int CFile::WriteDat(list< ::CM::Bar>& bar, string tradingday, string instrument)
{
	Open(tradingday, instrument,  "M1_", ".dat");
	list< ::CM::Bar>::iterator itr = bar.begin();
	for(; itr!=bar.end(); itr++)
	{
		fs.write((const char*)&(*itr), sizeof(::CM::Bar));
		bar.erase(itr++);
	}
	Close();
	return 0;
}

int CFile::ReadTxt(list< ::CM::Bar>& bar, string tradingday, string instrument)
{
	int ret = Open(tradingday, instrument, "M1_", ".txt", 1);
	if(ret < 0 ) return ret;

	::CM::Bar b;
	while(fs>> b.Timestamp
			>> b.High
			>> b.Low  
			>> b.Open   
			>> b.Close     
			>> b.Volume      
			>> b.Turnover
			>> b.OpenInterest)
	{
		bar.push_back(b);
	}
	Close();
	return 0;
}

int CFile::ReadDat(list< ::CM::Bar>& bar, string tradingday, string instrument)
{
	int ret = Open(tradingday, instrument, "M1_", ".dat", 1);
	if(ret< 0 ) return ret;

	::CM::Bar b;
	while(fs.read((char*)&b, sizeof(::CM::Bar)))
	{
		bar.push_back(b);
	}
	Close();
	return 0;
}


//读取1分k写到txt 写到data
int CFile::TransM1Txt(string infile, string outfile)
{
	fstream in, out;
	in.open(infile.c_str(), ios::app|ios::in);
	out.open(outfile.c_str(), ios::app|ios::out);
	
	::CM::Bar b;
	while(in>>  b.Timestamp 
			>>  b.High      
			>>  b.Low       
			>>  b.Open      
			>>  b.Close     
			>>  b.Volume    
			>>  b.Turnover 
			>>  b.OpenInterest
	)
	{
		out.write((char*)&b, sizeof(::CM::Bar));
	}
	in.close();
	out.close();
	return 0;
}

//读取1分k写到data 写到txt
int CFile::TransM1Dat(string infile, string outfile)
{
	
	fstream in, out;
	in.open(infile.c_str(), ios::app|ios::in);
	out.open(outfile.c_str(), ios::app|ios::out);
	
	out << setiosflags(ios::fixed) << setprecision(3);
	
	::CM::Bar b;
	while(in.read((char*)&b, sizeof(::CM::Bar)))
	{
		out << b.Timestamp  << " "      
			<< b.High       << " "
			<< b.Low        << " "
			<< b.Open       << " "
			<< b.Close      << " "
			<< b.Volume     << " "
			<< b.Turnover   << " " 
			<< b.OpenInterest  << "\n";
	}
	in.close();
	out.close();
	return 0;
}

//读取ticker data--->txt
int CFile::TransTTxt(string infile, string outfile)
{
	fstream in, out;
	in.open(infile.c_str(), ios::app|ios::in);
	out.open(outfile.c_str(), ios::app|ios::out);
	
	::CM::Ticker t;
	while(in>>  t.Timestamp
			>>  t.Price
			>>  t.Volume
			>>  t.Turnover
			>>  t.OpenInterest
			>>  t.TotalVolume
			>>  t.TotalTurnover
			>>  t.BidPrice1
			>>  t.AskPrice1
			>>  t.BidVolume1
			>>  t.AskVolume1
	)
	{
		out.write((char*)&t, sizeof(::CM::Ticker));
	}
	in.close();
	out.close();
	return 0;
}

//读取ticker txt--->data
int CFile::TransTDat(string infile, string outfile)
{
	fstream in, out;
	in.open(infile.c_str(), ios::app|ios::in);
	out.open(outfile.c_str(), ios::app|ios::out);
	
	out << setiosflags(ios::fixed) << setprecision(3);
	
	::CM::Ticker t;
	while(in.read((char*)&t, sizeof(::CM::Ticker)))
	{
		out << t.Timestamp     << " "      
			<< t.Price         << " "
			<< t.Volume        << " "
			<< t.Turnover      << " "
			<< t.OpenInterest  << " "
			<< t.TotalVolume   << " "
			<< t.TotalTurnover << " "
			<< t.BidPrice1     << " "
			<< t.AskPrice1     << " "
			<< t.BidVolume1    << " "
			<< t.AskVolume1    << "\n";  
	}
	in.close();
	out.close();
	return 0;
}

