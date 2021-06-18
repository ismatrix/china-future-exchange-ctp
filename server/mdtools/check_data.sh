#!/bin/bash


for file in `find ./hqdata/ -name "*" -type f| grep -v 000 | grep -v IDX | grep 20161026`
#for file in `find ./hqdata/ -name "*" -type f|  grep 20161031| grep T_`
do
	t=`echo $file | awk -F'_' '{print $3}' | cut -d '.' -f 1`
	f=`date -d "${t} 16:00:00" +%s `
	w=`cat $file | wc -l`
	c=`cat $file  | awk -v f=$f '($1/1000>f){print $0}' | wc -l`
	if [ $c -gt 0 ];then
		echo "tradingday = $t  maxtime = $f line = $w error_line= $c $file"
	fi
done
