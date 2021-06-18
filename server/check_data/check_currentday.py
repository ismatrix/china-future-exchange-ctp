#!/usr/bin/python
# -*- coding: utf-8 -*-

#1. 获取当天数据 保存文件
#2. 与mongo中数据对比
#3. 输出对应不上的数据到邮件

import time
import pymongo
import urllib2
import json
import demjson
import string

#  http://vip.stock.finance.sina.com.cn/quotes_service/view/js/qihuohangqing.js?cc  这里获取交易所和合约定义信息

#这里拼写连接 抓去数据 新浪的数据准确定不够
#  http://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php/Market_Center.getHQFuturesData?page=2&num=500&sort=position&asc=0&node=zly_qh&base=futures

#下面是和讯行情连接 但是他们没有结算价格
#225 大连
#http://webftcn.hermes.hexun.com/shf/sortlist?block=431&number=1000&title=14&commodityid=0&direction=0&start=0&column=code,name,price,volume,lastClose,open,high,low,openInterest

#192 郑州
#http://webftcn.hermes.hexun.com/shf/sortlist?block=432&number=1000&title=14&commodityid=0&direction=0&start=0&column=code,name,price,volume,lastClose,open,high,low,openInterest

#上海 217
#http://webftcn.hermes.hexun.com/shf/sortlist?block=430&number=1000&title=14&commodityid=0&direction=0&start=0&column=code,name,price,volume,lastClose,open,high,low,openInterest

#中金 36
#http://webcffex.hermes.hexun.com/cffex/sortlist?block=770&number=1000&title=14&commodityid=0&direction=0&start=0&column=code,name,price,volume,lastClose,open,high,low,openInterest


exchange=[u'czce', 'dce', 'shfe', 'cffex']

def get_instrumentid() :
	url = 'http://vip.stock.finance.sina.com.cn/quotes_service/view/js/qihuohangqing.js?cc'
	request = urllib2.Request(url)
	response = urllib2.urlopen(request)
	buf = response.read().decode('gbk').encode('utf8')
	#buf中获取合约id
	pos1 = buf.find('ARRFUTURESNODES = {')
	pos2 = buf.find('S_SL_CF_FU_DIG2 = function')
	strjson = buf[18 : pos2-pos1].replace(';', '').replace('\'郑州商品交易所\',', '').replace('\'大连商品交易所\',', '').replace('\'上海期货交易所\',', '').replace('\'中国金融期货交易所\',', '')
	res=demjson.decode(strjson)
	l=[]
	for ex in exchange :
		for one in res[ex] :
			l.append(one[1])
	return l
		
def get_cffex_instrument(exchange) :
	url = 'http://vip.stock.finance.sina.com.cn/quotes_service/view/js/qihuohangqing.js?cc'
	request = urllib2.Request(url)
	response = urllib2.urlopen(request)
	buf = response.read().decode('gbk').encode('utf8')
	#buf中获取合约id
	pos1 = buf.find('ARRFUTURESNODES = {')
	pos2 = buf.find('S_SL_CF_FU_DIG2 = function')
	strjson = buf[18 : pos2-pos1].replace(';', '').replace('\'郑州商品交易所\',', '').replace('\'大连商品交易所\',', '').replace('\'上海期货交易所\',', '').replace('\'中国金融期货交易所\',', '')
	res=demjson.decode(strjson)
	l = []
	for one in res[exchange]:
		response = urllib2.urlopen("http://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php/Market_Center.getNameList?page=1&num=5&sort=position&asc=0&node=" + one[1])
		buf = response.read().decode('gbk').encode('utf8')
		res=demjson.decode(buf)
		for i in range(0, len(res)) :
			l.append(res[i]["symbol"])
	return l
		
def get_cffex_data():
	l = get_cffex_instrument("cffex")
	url = "http://hq.sinajs.cn/rn=32ee30c8&list="
	for i in range(0, len(l)) :
		if(i == 0):
			url += "CFF_" + l[i]
		else:
			url += ",CFF_" + l[i]
	print url
	response = urllib2.urlopen(url)
	print "............."
	buf = response.read().decode('gbk').encode('utf8')
	print buf


def get_daybar() :
	client = pymongo.MongoClient('127.0.0.1', 27017)
	db = client['smartwin']
	ins = db["DAYBAR"]
	tradingday = time.strftime("%Y%m%d", time.localtime())
	l=[]
	res = ins.find({'tradingday': tradingday})
	for one in res :
		l.append(one)
	print("DAYBAR size=", len(l))
	return l

def get_sina_data(instruments) :
	l=[]
	print instruments
	for instrument in instruments :
		#num=1000  node填错 可以把数据全部拉回来
		url = 'http://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php/Market_Center.getHQFuturesData?page=1&num=1000&sort=position&asc=0&node=i' +  instrument  + '&base=futures'
		request = urllib2.Request(url)
		response = urllib2.urlopen(request)
		strjson = response.read().decode('gbk').encode('utf8')
		l = demjson.decode(strjson)
		print(len(l), url)
		return l

def write_sina_data(sina_daybars) :
	tradingday = time.strftime("%Y%m%d", time.localtime())
	f=open(tradingday + '_sina.txt', 'wt')
	for one in sina_daybars :
		f.write(json.dumps(one))
	f.close()

def get_productid(instrument) :
	for i in range(0, len(instrument)) :
		if(not ((instrument[i] >= '0' and instrument[i] <= '9') or  (instrument[i] >= 'A' and instrument[i] <= 'Z') or (instrument[i] >= 'a' and instrument[i] <= 'z'))) :
			return ""
		if(instrument[i] >='0'  and instrument[i] <= '9') :
			return instrument[0:i]
	return ""

#把合约中年份没有1的补齐
def update_instrument(instrument) :
	productid = get_productid(instrument)
	back=''
	if(productid != '') :
		back = instrument[len(productid):]
	if(len(back) == 3) :
		back = '1' + back
		#print productid, back
	return productid+back
		

def check_field(local_daybar, sina_daybar, tag1, tag2) :
	if(string.atof(sina_daybar[tag2]) - string.atof(local_daybar[tag1]) > 0.001) :
		tradingday = time.strftime("%Y%m%d", time.localtime())
		f=open(tradingday + '_sina_check.txt', 'awt')
		f.write(local_daybar['instrument'] + ' ' + tag1 + ' local=' + str(local_daybar[tag1]) + ' sina=' + str(sina_daybar[tag2]) + '\n')
		f.close()

def check_daybar(local_daybar, sina_daybar):
	check_field(local_daybar, sina_daybar, 'open', 'open')
	check_field(local_daybar, sina_daybar, 'close', 'close')
	check_field(local_daybar, sina_daybar, 'high', 'high')
	check_field(local_daybar, sina_daybar, 'low', 'low')
	check_field(local_daybar, sina_daybar, 'settlement', 'settlement')
	check_field(local_daybar, sina_daybar, 'presettlement', 'prevsettlement')
	check_field(local_daybar, sina_daybar, 'volume', 'volume')
	check_field(local_daybar, sina_daybar, 'openinterest', 'position')

if __name__ == '__main__' :
	#get_cffex_data();
	instruments = get_instrumentid()
	local_daybars = get_daybar()
	sina_daybars = get_sina_data(instruments)
	write_sina_data(sina_daybars)
	print("local_daybars.size=", len(local_daybars))
	print("sina_daybars.size=", len(sina_daybars))
	for one in sina_daybars :
		flag = False
		for item in local_daybars :
			instrument = update_instrument(item['instrument'])
			if(one['symbol'].upper() == instrument.upper()) :
				check_daybar(item, one)
				flag = True
		productid = get_productid(one['symbol'])
		if(flag == False and  productid+'0' != one['symbol']) :
			print 'sina', one['symbol'], productid, one['open'], one['close'], one['high'], one['low'], one['settlement'], one['prevsettlement'], one['volume'], one['position']









