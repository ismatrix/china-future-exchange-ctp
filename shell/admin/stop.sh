#########################################################################
# File Name: stop.sh
# Created Time: Thu 12 May 2016 05:51:32 PM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash


if [ $^ -lt 1];then
	echo "need param"
	exit
fi


server=$1
