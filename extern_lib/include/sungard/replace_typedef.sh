#########################################################################
# File Name: c.sh
# Created Time: Thu 23 Jun 2016 05:44:54 PM CST
# Author: 
# description: 
#########################################################################
#!/bin/bash

#s=`cat ./kfts_trade_type.h  | grep char | grep -v "\[" |  sed "s/;//g" | awk '{print "map[\""$3"\"]=\"char\""}'`
s=`cat ./kfts_trade_type.h  | grep char | grep -v "\["  | awk '{print $3}' |  sed "s/;//g;s/ //g"`
for type in $s
do
	 sed -i "s/\t${type}\t/\tchar\t/g" ./kfts_trade_struct.h
	 sed -i "s/\t${type} /\tchar /g" ./kfts_trade_struct.h
	 sed -i "s/ ${type} / char /g" ./kfts_trade_struct.h
done

#s+=`cat ./kfts_trade_type.h  | grep int | grep  typedef | sed "s/;//g" | awk '{print "map[\""$3"\"]=\"int\""}'`
s=`cat ./kfts_trade_type.h  | grep int | grep  typedef | awk '{print $3}' |  sed "s/;//g;s/ //g"`
for type in $s
do
	 sed -i "s/\t${type}\t/\tint\t/g" ./kfts_trade_struct.h
	 sed -i "s/\t${type} /\tint /g" ./kfts_trade_struct.h
	 sed -i "s/ ${type} / int /g" ./kfts_trade_struct.h
done


#s+=`cat ./kfts_trade_type.h  | grep double  | grep  typedef | sed "s/;//g" | awk '{print "map[\""$3"\"]=\"double\""}'`
s=`cat ./kfts_trade_type.h  | grep double  | grep  typedef | awk '{print $3}' |  sed "s/;//g;s/ //g"`
for type in $s
do
	 sed -i "s/\t${type}\t/\tdouble\t/g" ./kfts_trade_struct.h
	 sed -i "s/\t${type} /\tdouble /g" ./kfts_trade_struct.h
	 sed -i "s/ ${type} / double /g" ./kfts_trade_struct.h
done





