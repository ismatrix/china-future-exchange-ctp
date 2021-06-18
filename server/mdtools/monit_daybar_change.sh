#!/bin/sh

export PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/lib:

declare -A p1
p1["ag1712"]=
p1["rb1710"]=
p1["au1712"]=

function log() {
    now_time='['$(date +"%Y-%m-%d %H:%M:%S")']'
    echo "$now_time $1" >> /home/ops/production/mdtools/monit_daybar_change.log	
}

function checkchange() {
    instrument=$1
    d=`date  "+%Y-%m-%d %H:%M:%S" `
    tradingday=`./mdtools -ctd "$d" | awk -F'tradingday:' '{print $2}'`
    p=`mongo --quiet smartwin  --eval "db.DAYBAR.find({tradingday:\"$tradingday\", instrument:\"$instrument\"}).forEach(function(v){print(v.updatedate)})" | head -1`

    pf=${p1[$instrument]}
    log "tradingday=$tradingday instrument=$instrument p=$p pf=$pf"
    
    if [ "$p" == "" ];then
	log "$instrument db no data"
	return 0
    fi
	
    if [ "$pf" == "$p" ]; then
	log "$instrument not change"
	return 0
    else
	p1[$instrument]=$p
    fi
    return 1
}

cd /home/ops/production/mdtools

pid=`cat monit_daybar_change.pid 2>/dev/null`
cpid=`ps aux|grep monit_daybar_change.sh | awk -v pid="$pid" '($2==pid){print $2}'`
echo "pip=$pid cpid=$cpid"
if [ "$pid" == "$cpid" ];then
    exit 0
fi

echo $$ > monit_daybar_change.pid
while (true)
do
    trading=`./mdtools -istrading ag  0`
    if [ "$trading" == "false" ]; 
    then
	sleep 60
	continue
    fi

    checkchange "ag1712"
    ag=$?
   
    if [ $ag -eq 0 ];#  && [ $rb -eq 0 ] && [ $rb -eq 0 ];
    then
	eml_sms_tl -t "daybar not update" -b "not update" -H "smtp.exmail.qq.com" -p 25 -u "dreamy.zhang@invesmart.cn" -p "tm7409TTA" -f "dreamy.zhang@invesmart.cn" -m "13651401725@qq.com" -i  
        log "daybar not update"
    fi

    sleep 30

done








