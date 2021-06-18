#!/bin/bash

fundid=$1
start_time=$2 
end_time=$3
dir=$4
mkdir -p /home/ops/yunwei/data/${dir}/${start_time}
cd /home/ops/yunwei/data/${dir}/${start_time}


echo "tradeid, orderid, instrumentid, direction, ffsetflag, price, volume, tradedate, tradetime, signalname, strategyid, userid" > done_${fundid}_${start_time}.csv

mongo --quiet  smartwin --eval "db.DONE.find({fundid:\"$fundid\", tradingday:{\$gte: \"$start_time\", \$lt: \"$end_time\"}}).sort({tradingday:1}).forEach(v=>v.done.map(d=>print(d.tradeid,',', d.orderid, ',', d.instrumentid, ',', d.direction, ',', d.offsetflag, ',', d.price, ',', d.volume, ',', d.tradedate, ',', d.tradetime, ',', d.signalname, ',', d.strategyid, ',', d.userid)))"  >> done_${fundid}_${start_time}.csv


#查出所有的合约乘数
#openaverageprice = item.opencost/item.position/item.volumemultiple; 开仓均价计算方法

#需要保证持仓合约在合约表中可以查到合约乘数
echo "基金id,合约,持仓量,持仓方向,今开,今平, 昨日总持仓,昨日余仓,开仓均价, 保证金" > position_${fundid}_${start_time}.csv
mongo --quiet  smartwin --eval "db.POSITION.find({fundid:\"$fundid\", tradingday:\"$start_time\"}).sort({tradingday:1}).forEach(v=>v.positions.map(d=>db.INSTRUMENT.find({instrumentid: d.instrumentid}, {volumemultiple:1}).limit(1).map(i=>print(v.fundid,',', d.instrumentid, ',', d.position, ',', d.direction, ',', d.openvolume, ',', d.closevolume, ',', d.preposition, ',', d.preholdposition, ',', d.opencost/d.position/i.volumemultiple, ',',  d.margin))))"  >> position_${fundid}_${start_time}.csv















