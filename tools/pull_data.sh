#!/bin/bash

#cd /data/smb/will

#cd /home/ops/code/backend/init_machine/yunwei/abs

#cd /home/ops/code/backend/tools
cd /home/ops/yunwei/abs


max=`date "+%Y%m01"`
min=`date -d"-1 month" +"%Y%m01"`

echo "120.76.98.94 8800 ops ops!@#9988" > ./abs-iplist
echo "file:::/home/ops/code/backend/tools/export_trade_data.sh  /home/ops/yunwei/ push" > ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 80000528 $min $max  month" >> ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 0920000999 $min $max month" >> ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 870218 $min $max   month" >> ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 3000121 $min $max   month" >> ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 3000736new $min $max  month" >> ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 3000686 $min $max  month" >> ./abs-config

echo "file:::/home/ops/yunwei/data/ /data/smb/rickon/ pull" >> ./abs-config
echo "file:::/home/ops/yunwei/data/ /data/smb/stacy/ pull" >> ./abs-config
./abs.sh




