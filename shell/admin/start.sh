#########################################################################
# File Name: start.sh
# Created Time: Thu 12 May 2016 06:11:05 PM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash


date=`date "+%Y%m%d"`
backdir=/mnt/backup/crontab

crontab -l > ${backdir}/${date}.bak

#check not exist for update
echo "* * * * * a.sh" >> ${backdir}/${date}.bak
crontab ${backdir}/${date}.bak


