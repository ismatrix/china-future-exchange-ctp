#########################################################################
# File Name: index.sh
# Created Time: Thu 30 Jun 2016 02:33:44 PM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash

export PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/lib:

print()
{
	echo "Usage: $0 {tradingday|product|products}"
	echo "     examp: $0 tradingday 20160601"
	echo "     examp: $0 product    ag"
	echo "     examp: $0 products  all"
	echo "     examp: $0 currentday  all"
	echo "     examp: $0 mtradingday 20160601"
	echo "     examp: $0 mproduct    ag"
	echo "     examp: $0 master  all"
	echo "     examp: $0 instrument_rank 20160601"
}

if [ $# -lt 2 ] ; then
	eval print
	exit -1
fi


eval_products() 
{
	products=`ls /mnt/hqdata/  | awk '{print "./mdtools -index -query-p " $1}' | bash | sort | uniq`

	for product in $products
	do
		rm  /mnt/hqdata/${product}IDX/*
		echo ./mdtools	-index -day-p    $product
		./mdtools	-index -day-p    $product
		
		echo ./mdtools	-index -minute-p    $product
		./mdtools	-index -minute-p    $product
	done
}

eval_masters()
{
	products=`ls /mnt/hqdata/  | awk '{print "./mdtools -index -query-p " $1}' | bash | sort | uniq`

	for product in $products
	do
		rm  /mnt/hqdata/${product}0001/*
		echo ./mdtools	-index -master-p    $product
		./mdtools	-index -master-p    $product
	done
}


cd /home/ops/production/mdtools
case "$1" in
	tradingday)
		find /mnt/hqdata/ -name "*" -type f | grep "IDX_"$2 | xargs rm
		./mdtools   -index -day-t $2
		./mdtools   -index -minute-t $2
		;;
	product)
		find /mnt/hqdata/ -name "*" -type f | grep $2"IDX_" | xargs rm
		./mdtools   -index -day-p $2
		./mdtools   -index -minute-p $2
		;;
	products)
		eval_products
		;;
	dayall)
		./mdtools -index -day all
		;;
	currentday)
		dt=`date "+%Y%m%d"`
		#指数
		find /mnt/hqdata/ -name "*" -type f | grep "IDX_"${dt} | xargs rm
		./mdtools   -index -day-t ${dt}
		./mdtools   -index -minute-t ${dt}

		#主连
		find /mnt/hqdata/ -name "*" -type f | grep "0001_"$dt | xargs rm
		./mdtools -index -master-t $dt
		;;
	currentday_rank)
		#更新合约表
		dt=`date "+%Y%m%d"`
		w=`date -d "$dt" +%w`
		if [ $w -ne 6 ] && [ $w -ne 0 ] ; then
			./mdtools  -instrument_rank $dt
		else
			echo "weekend, not do somgthing"
		fi
		;;
	mtradingday)
		find /mnt/hqdata/ -name "*" -type f | grep "0001_"$2 | xargs rm
		./mdtools -index -master-t $2
		;;
	mproduct)
		find /mnt/hqdata/ -name "*" -type f | grep $2"0001_" | xargs rm
		./mdtools -index -master-p $2	
		;;
	master)
		eval_masters
		;;
	instrument_rank)
		w=`date -d "$2" +%w`
		if [ $w -ne 6 ] && [ $w -ne 0 ] ; then
			./mdtools  -instrument_rank $2
		else
			echo "weekend, not do somgthing"
		fi
		;;
	*)
		print
esac




