#!/bin/bash

#cd /data/smb/will

#cd /home/ops/code/backend/init_machine/yunwei/abs

#cd /home/ops/code/backend/tools
cd /home/ops/yunwei/abs


max=20180810
min=20180501

echo "120.76.98.94 8800 ops ops!@#9988" > ./abs-iplist
echo "file:::/home/ops/code/backend/tools/export_trade_data.sh  /home/ops/yunwei/ push" > ./abs-config
echo "com:::cd /home/ops/yunwei; ./export_trade_data.sh 087590 $min $max  kevin" >> ./abs-config
echo "file:::/home/ops/yunwei/data/kevin/ /data/smb/kevin/ pull" >> ./abs-config
./abs.sh




