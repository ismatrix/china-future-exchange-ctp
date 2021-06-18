#########################################################################
# File Name: jisuan.sh
# Created Time: Thu 16 Jun 2016 10:18:30 AM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash


#for f in $file; do ./mdtools  -rundata  $f  ./hqdata     ; echo $f; done

if [ $# -lt 1 ] ; then
        echo "usage: $0 /data/data-06/201606*/* 201606"
        exit -1
fi


echo "ls ${1}"
file=`ls ${1}`

for f in $file
do
	./mdtools  -rundata  $f ./hqdata
	echo "./mdtools  -rundata  $f ./hqdata"
done
