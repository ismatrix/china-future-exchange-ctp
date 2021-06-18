/////////////////////////////////////////////////////////////////////////
///@system �����ļ�����
///@company ��������
///@file file.h
///@brief ��� ��kд�ļ�
///@history 
///20160326	dreamyzhang		�������ļ�
/////////////////////////////////////////////////////////////////////////
#ifndef __FILE_H__
#define __FILE_H__

#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fstream>
#include <iomanip>
#include <list>

#include "util.h"
#include "RollLog.h"
#include "../ice/Common.h"		//ice�ṹ�����ļ�

using namespace std;

//1. ÿ�촴��һ��ȫ��Ŀ¼
//2. �̶�ʱ����ߴﵽ�̶�����ȫ��д�ļ�һ�Ρ�
//3. ��ȡ��ʱ��Ʒ���¼�ȷ����ȡ��
//4. /20150526/IF1606/M1_IF1606.txt
//4. /20150526/IF1606/T_IF1606.txt
//Open("/data/", "20160526", "IF1606", "M1")
class CFile
{
	public:
		CFile(){};
		CFile(string prefix, string suffix=""){_prefix = prefix; _suffix = suffix;};
		int Open(string tradingday, string instrument, string module, string _suffix, int read=0);
		int Close();

		int WriteTxt(list< ::CM::Ticker>& ticker, string tradingday, string instrument);
		int WriteDat(list< ::CM::Ticker>& ticker, string tradingday, string instrument);
		int ReadTxt(list< ::CM::Ticker>& ticker, string tradingday, string instrument);
		int ReadDat(list< ::CM::Ticker>& ticker, string tradingday, string instrument);
		
		int WriteTxt(list< ::CM::Bar>& bar, string tradingday, string instrument);
		int WriteDat(list< ::CM::Bar>& bar, string tradingday, string instrument);
		int ReadTxt(list< ::CM::Bar>& bar, string tradingday, string instrument);
		int ReadDat(list< ::CM::Bar>& bar, string tradingday, string instrument);
	
		void set__prefix(string prefix){_prefix = prefix;}

		static int TransM1Txt(string infile, string outfile);
		static int TransM1Dat(string infile, string outfile);
		static int TransTTxt(string infile, string outfile);
		static int TransTDat(string infile, string outfile);

		static void create_dir(string path)
		{
			int32_t pos = path.find("/");
			while(pos != (int32_t)string::npos)
			{
				//LOG_DEBUG("ops:" << pos << " pos1:" << pos1);
				string mid = path.substr(0, pos+1);
				mkdir(mid.c_str(), 0755);
				//LOG_DEBUG("create_dir " << mid << " pos:" << pos);
			
				pos = path.find("/", pos+1);
			}
			mkdir(path.c_str(), 0755);
		}
	private:
		fstream fs;
		string _filename;
		
		string _prefix;
		string _suffix;
};

#endif



