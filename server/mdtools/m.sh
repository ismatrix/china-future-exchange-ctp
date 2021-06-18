#!/bin/sh


#nohup ./jisuan.sh "/mnt/futuresData/CFFEX/2014*/*"  > CFFEX_2014.log
#nohup ./jisuan.sh "/mnt/futuresData/CFFEX/2015*/*"  > CFFEX_2015.log &
#nohup ./jisuan.sh "/mnt/futuresData/CFFEX/2016*/*"  > CFFEX_2016.log &
#nohup ./jisuan.sh "/mnt/futuresData/CZCE/2014*/*"   > CZCE_2014.log &
#nohup ./jisuan.sh "/mnt/futuresData/CZCE/2015*/*"   > CZCE_2015.log &
#nohup ./jisuan.sh "/mnt/futuresData/CZCE/2016*/*"   > CZCE_2016.log &
#nohup ./jisuan.sh "/mnt/futuresData/DCE/2014*/*"    > DCE_2014.log &
#nohup ./jisuan.sh "/mnt/futuresData/DCE/2015*/*"    > DCE_2015.log &
#nohup ./jisuan.sh "/mnt/futuresData/DCE/2016*/*"    > DCE_2016.log &
#nohup ./jisuan.sh "/mnt/futuresData/SHFE/2014*/*"   > SHFE_2014.log &
#nohup ./jisuan.sh "/mnt/futuresData/SHFE/2015*/*"   > SHFE_2015.log &
#nohup ./jisuan.sh "/mnt/futuresData/SHFE/2016*/*"   > SHFE_2016.log &

find ./hqdata/ -name "*" -type f | grep 201606 | xargs rm
./jisuan.sh "/data/data-06/201606*/*" 
./jisuan.sh "/data/data-06/dc/tick/201606*/*" 
./jisuan.sh "/data/data-06/sc/tick/201606*/*" 
./jisuan.sh "/data/data-06/zc/tick/201606*/*" 




