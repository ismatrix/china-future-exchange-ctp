#########################################################################
# File Name: c.sh
# Created Time: Thu 23 Jun 2016 05:44:54 PM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash

#s=`cat ./ThostFtdcUserApiDataType.h  | grep char | grep -v "\[" |  sed "s/;//g" | awk '{print "map[\""$3"\"]=\"char\""}'`
s=`cat ./ThostFtdcUserApiDataType.h  | grep char | grep typedef | grep -v "\[" |  sed "s/;//g" | awk '{print $3}'`
for type in $s
do
	 sed -i "s/\t${type}\t/\tchar\t/g" ./ThostFtdcUserApiStruct.h
done

#s+=`cat ./ThostFtdcUserApiDataType.h  | grep int | grep  typedef | sed "s/;//g" | awk '{print "map[\""$3"\"]=\"int\""}'`
s=`cat ./ThostFtdcUserApiDataType.h  | grep int | grep  typedef | sed "s/;//g" | awk '{print $3}'`
for type in $s
do
	 sed -i "s/\t${type}\t/\tint\t/g" ./ThostFtdcUserApiStruct.h
done


#s+=`cat ./ThostFtdcUserApiDataType.h  | grep double  | grep  typedef | sed "s/;//g" | awk '{print "map[\""$3"\"]=\"double\""}'`
s=`cat ./ThostFtdcUserApiDataType.h  | grep double  | grep  typedef | sed "s/;//g" | awk '{print $3}'`
for type in $s
do
	 sed -i "s/\t${type}\t/\tdouble\t/g" ./ThostFtdcUserApiStruct.h
done





