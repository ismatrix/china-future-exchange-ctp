#!/bin/bash

fundid=$1
start_time=$2 
end_time=$3
dir=$4
mkdir -p /home/ops/yunwei/data/${dir}/${start_time}
cd /home/ops/yunwei/data/${dir}/${start_time}


echo "tradeid, orderid, instrumentid, direction, ffsetflag, price, volume, tradedate, tradetime, signalname, strategyid, userid" > done_${fundid}_${start_time}.csv

mongo --quiet  smartwin --eval "db.DONE.find({fundid:\"$fundid\", tradingday:{\$gte: \"$start_time\", \$lt: \"$end_time\"}}).sort({tradingday:1}).forEach(v=>v.done.map(d=>print(d.tradeid,',', d.orderid, ',', d.instrumentid, ',', d.direction, ',', d.offsetflag, ',', d.price, ',', d.volume, ',', d.tradedate, ',', d.tradetime, ',', d.signalname, ',', d.strategyid, ',', d.userid)))"  >> done_${fundid}_${start_time}.csv


#������еĺ�Լ����
#openaverageprice = item.opencost/item.position/item.volumemultiple; ���־��ۼ��㷽��

#��Ҫ��֤�ֲֺ�Լ�ں�Լ���п��Բ鵽��Լ����
echo "����id,��Լ,�ֲ���,�ֲַ���,��,��ƽ, �����ֲܳ�,�������,���־���, ��֤��" > position_${fundid}_${start_time}.csv
mongo --quiet  smartwin --eval "db.POSITION.find({fundid:\"$fundid\", tradingday:\"$start_time\"}).sort({tradingday:1}).forEach(v=>v.positions.map(d=>db.INSTRUMENT.find({instrumentid: d.instrumentid}, {volumemultiple:1}).limit(1).map(i=>print(v.fundid,',', d.instrumentid, ',', d.position, ',', d.direction, ',', d.openvolume, ',', d.closevolume, ',', d.preposition, ',', d.preholdposition, ',', d.opencost/d.position/i.volumemultiple, ',',  d.margin))))"  >> position_${fundid}_${start_time}.csv















