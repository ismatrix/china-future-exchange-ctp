#!/bin/bash


print () 
{
	echo -e "usage: "
	echo -e "    --check  --tick   repeated_next_key            "  
	echo -e "    --check  --tick   repeated_key                 "              
	echo -e "    --check  --tick   repeated_all                 "           
	echo -e "    --check  --tick   error_key                    "          
	echo -e "    --check  --bar    repeated_key                 "        
	echo -e "    --check  --bar    repeated_all                 "         
	echo -e "    --check  --bar    error_key                    "       
	echo -e "    --update --tick   repeated_next_key            "       
	echo -e "    --update --tick   repeated_all                 "       
	echo -e "    --update --tick   repeated_key                 "          
	echo -e "    --update --tick   error_key                    "          
	echo -e "    --update --bar    repeated_all                 " 
	echo -e "    --update --bar    repeated_key                 "    
	echo -e "    --update --bar    error_key                    "  

	echo -e "\nonly execute ones for the following"	
	echo -e "    --update --tick   turnover                     "        
	echo -e "    --update --bar    turnover                     " 
	echo -e "    --update --daybar turnover_average             "   
}

#检查连续两个数据key是否又重复
check_repeated_next_key()
{
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f| grep -v 000 | grep -v IDX | grep  $filter `
	do
		cat $file | awk -v f=$file 'BEGING{count=0; key=0}
		{
			if(key == $1) count++;
			key=$1
		}
		END{if(count>1) print count, f;}'
	done	
}

#更改连续两个ticker的key一样的第二个key增加毫秒
update_repeated_next_ticker_key()
{
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f| grep -v 000 | grep -v IDX | grep  $filter`
	do
		cat $file | awk -v f=$file 'BEGIN{key=0}{
			if(key == $1)
			{
				$1+=500
			}
			key=$1
			print $0
		}' > /tmp/update_repeated_next_ticker_key.tmp
		mv /tmp/update_repeated_next_ticker_key.tmp $file
	done
}


#检查错误的key数据 交易日16点后的数据
check_error_data() 
{
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f | grep -v 000 | grep -v IDX | grep  $filter`
	do
		tradingday=`echo $file | awk -F'_' '{print $3}' | cut -d '.' -f 1`
		keyuplimit=`date -d "${tradingday} 16:00:00" +%s `
		l=`cat $file | wc -l`
		c=`cat $file  | awk -v key=$keyuplimit  '($1/1000>key){print $0}' | wc -l`
		if [ $c -gt 0 ];then
			echo "$file   line = $l    errorkeynum = $c"
		fi
	done
}


#清理数据中交易日18点后的数据
clear_error_data() 
{
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f | grep -v 000 | grep -v IDX | grep  $filter`
	do
		tradingday=`echo $file | awk -F'_' '{print $3}' | cut -d '.' -f 1`
		keyuplimit=`date -d "${tradingday} 16:00:00" +%s `
		cat $file  | awk -v key=$keyuplimit  '($1/1000<key){print $0}' > /tmp/clear_error_data_${filter}.tmp
		echo "$file $tradingday $keyuplimit"
		mv /tmp/clear_error_data_${filter}.tmp $file
	done
}

#检查完全重复数据
check_repeated_all() 
{
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f | grep -v 000 | grep -v IDX | grep  $filter`
	do
		w=`cat $file | wc -l`
		c=`cat $file  | sort -n -k1 | uniq  | wc -l `
		diff=$[$w-$c]
		if [ $diff -gt 0 ];
		then
			echo "$file  diff = $diff line = $w uniq = $c "
		fi
	done
}

#检查合约中重复key的数据
check_repeated_key() 
{
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f | grep -v 000 | grep -v IDX | grep  $filter`
	do
		w=`cat $file | wc -l`
		c=`cat $file  | awk '{print $1}' | sort | uniq  | wc -l `
		diff=$[$w-$c]
		if [ $diff -gt 0 ];
		then
			echo "$file  diff = $diff line = $w uniq = $c "
		fi
	done
}

#清除重复数据
clear_repeated_all() 
{	
	filter=$1
	for file in `find ./hqdata/ -name "*" -type f | grep -v 000 | grep -v IDX | grep  $filter`
	do
		cat $file | sort -n -k1 | uniq -u  > /tmp/clear_repeated_all_${filter}.tmp
		echo  "mv /tmp/clear_repeated_all_${filter}.tmp $file"
		mv  /tmp/clear_repeated_all_${filter}.tmp $file
	done
}

clear_repeated_key()
{
	echo "not realize."	
}

#修改ticker中的成交额 没有成交均价
process_multiply_quest_ticker() 
{
	for instrument in `ls ./hqdata/`
	do
	    s=`mongo smartwin --quiet --eval "db.INSTRUMENT.find({instrumentid:'$instrument'}, {volumemultiple:1, exchangeid:1}).forEach(function(v){print(v.volumemultiple, v.exchangeid);})"`
	    volumemultiple=`echo $s | awk '{print $1}'`
	    exchangeid=`echo $s | awk '{print $2}'`
	   #echo "$instrument, $volumemultiple, $exchangeid."
	   if [ "$exchangeid" == "CZCE" ];
	    then
	        for file in `find ./hqdata/${instrument} -name "*" -type f |  grep  T_`
	        do
	    	    echo "$file,  $instrument, $volumemultiple, $exchangeid."
		    #成交额乘以合约乘数
	            cat $file | awk -v volumemultiple=$volumemultiple '{$4=$4*volumemultiple;$7=$7*volumemultiple; print $0;}' > /tmp/process_multiply_quest_ticker.tmp
	            mv  /tmp/process_multiply_quest_ticker.tmp $file
	     	done
	     fi
	done
}

#修改bar中的成交额 没有成交均价
process_multiply_quest_bar() 
{
	for instrument in `ls ./hqdata/`
	do
	    s=`mongo smartwin --quiet --eval "db.INSTRUMENT.find({instrumentid:'$instrument'}, {volumemultiple:1, exchangeid:1}).forEach(function(v){print(v.volumemultiple, v.exchangeid);})"`
	    volumemultiple=`echo $s | awk '{print $1}'`
	    exchangeid=`echo $s | awk '{print $2}'`
	   if [ "$exchangeid" == "CZCE" ];
	    then
	        for file in `find ./hqdata/${instrument} -name "*" -type f |  grep  M1_`
	        do
	    	    echo "$file,  $instrument, $volumemultiple, $exchangeid."
		    #成交额乘以合约乘数
		    cat $file | awk -v volumemultiple=$volumemultiple '{ $7=$7*volumemultiple; print $0;}' > /tmp/process_multiply_quest_bar.tmp
		    mv /tmp/process_multiply_quest_bar.tmp $file
	     	done
	     fi
	done
}

#修改daybar中的成交额 成交均价
process_multiply_quest_daybar() 
{
	#获取所有合约信息
	mongo --quiet smartwin --eval "db.INSTRUMENT.find({instrumentid:{\$not:/&/}}).forEach(function(v){print(v.instrumentid, v.exchangeid, v.volumemultiple);})" | while read instrumentid exchangeid volumemultiple
	do
		
		echo "$instrumentid, $exchangeid, $volumemultiple."

		mongo --quiet smartwin --eval "db.DAYBAR.find({instrument:'$instrumentid'}, {instrument:1, turnover:1, average:1}).limit(1).pretty()"
		if [ "$exchangeid" == "CZCE" ];
		then
			mongo --quiet smartwin --eval "db.DAYBAR.find({instrument:'$instrumentid'}).forEach(function(v){
				db.DAYBAR.update({_id:v._id}, {\$set:{turnover: v.turnover * ${volumemultiple}}}, false, true)
			})" 
		else
			mongo --quiet smartwin --eval "db.DAYBAR.find({instrument:'$instrumentid'}).forEach(function(v){
				db.DAYBAR.update({_id:v._id}, {\$set:{average: v.average / ${volumemultiple}}}, false, true)
			})" 
		fi
		mongo --quiet smartwin --eval "db.DAYBAR.find({instrument:'$instrumentid'}, {instrument:1, turnover:1, average:1}).limit(1).pretty()"
		


	done 
}


case  $* in
	"--check --tick repeated_next_key")
		eval check_repeated_next_key T_
	;;
	"--check --tick repeated_key")
		eval check_repeated_key T_ 
	;;
	"--check --tick repeated_all")
		eval check_repeated_all T_ 
	;;
	"--check --tick error_key")
		eval check_error_data T_
	;;
	"--check --bar repeated_key")
		eval check_repeated_key M1_ 
	;;
	"--check --bar repeated_all")
		eval check_repeated_all M1_ 
	;;
	"--check --bar error_key")
		eval check_error_data M1_
	;;
	"--update --tick repeated_next_key")
		eval update_repeated_next_ticker_key T_
	;;
	"--update --tick repeated_all")
		eval clear_repeated_all T_
	;;
	"--update --tick repeated_key")
		eval clear_repeated_key T_
	;;
	"--update --tick error_key")
		eval clear_error_data T_
	;;
	"--update --bar repeated_all")
		eval clear_repeated_all M1_
	;;
	"--update --bar repeated_key")
		eval clear_repeated_key M1_
	;;
	"--update --bar error_key")
		eval clear_error_data M1_
	;;
	"--update --tick turnover")
		eval process_multiply_quest_ticker
	;;
	"--update --bar turnover")
		eval process_multiply_quest_bar 
	;;
	"--update --daybar turnover_average")
		eval process_multiply_quest_daybar
	;;
	*)
		eval print
	;;
esac

