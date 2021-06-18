#########################################################################
# File Name: update_all_histroy_instrument_db.sh
# Created Time: Fri 08 Jul 2016 03:52:53 PM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash


cd /home/ops/production/mdtools/

instrumentids=`ls /mnt/hqdata | grep -v IDX | grep -v 0001`
for instrumentid in $instrumentids
do
	productid=`./mdtools -index -query-p $instrumentid`
	volumemultiple=`mongo  smartwin --quiet  -eval "db.PRODUCT.find({'productid':'$productid'}, {'volumemultiple':1}).forEach(function(item){ print(item.volumemultiple);})" | head -1`
	exchangeid=`mongo  smartwin --quiet  -eval "db.PRODUCT.find({'productid':'$productid'}, {'exchangeid':1}).forEach(function(item){ print(item.exchangeid);})" |head -1`
	productclass=`mongo  smartwin --quiet  -eval "db.PRODUCT.find({'productid':'$productid'}, {'productclass':1}).forEach(function(item){ print(item.productclass);})" |head -1`
	istrading=`mongo  smartwin --quiet  -eval "db.INSTRUMENT.find({'instrumentid':'$instrumentid'}, {'istrading':1}).forEach(function(item){ print(item.istrading);})" |head -1`

	if [ "x"$istrading == "x1" ];then
		continue;
	fi

	#echo "$instrumentid  $productid $volumemultiple $exchangeid $istrading"


	echo mongo smartwin  --quiet --eval "db.INSTRUMENT.update({'instrumentid': '$instrumentid'}, {'instrumentid': '$instrumentid', 'exchangeid': '$exchangeid', 'productid':'$productid', 'productclass':'$productclass', 'volumemultiple': $volumemultiple, 'istrading': 0}, true)"
	mongo smartwin  --quiet --eval "db.INSTRUMENT.update({'instrumentid': '$instrumentid'}, {'instrumentid': '$instrumentid', 'exchangeid': '$exchangeid', 'productid':'$productid', 'productclass':'$productclass', 'volumemultiple': $volumemultiple, 'istrading': 0}, true)"

done




















