# catalog
```
base          基础库
extern_lib	  使用使用的外部库
init_machine  器配置和和初始化配置信息
server		  交易和行情 server程序
shell		  一些shell的工具
```

# compile
```
compile all:
cd /home/ops/dreamy/backend/server
make clean && make release

compile only one server:
cd mdlive_svr
make clean && make release
```

# release
```
cd /home/ops/yunwei/release
./release_svr.sh  buid_release commit mdlive_svr
./release_svr.sh  buid_release commit mdindex_svr
./release_svr.sh  buid_release commit mdpast_svr
./release_svr.sh  buid_release commit tdctp_svr fundid
```

# release a  new fund
```
 > first insert the informations of the fund to table FUND on the development machine
 > then execute the cmd:
     ./release_svr.sh  buid_release commit tdctp_svr fundid
```

# 部署环境
```
120.76.98.94 上部署行情服务：

# 查询合约信息
17 15 * * * cd /home/ops/production/update_instrment_tools && ./update_instrment_tools --ctp -b 4200 -u 15100058 -p tm7409TTA -a tcp://180.166.11.33:41205 --daomon_update_instrument >> update_instrment_tools.txt &
30 18 * * * cd /home/ops/production/update_instrment_tools && ./update_instrment_tools --ctp -b 4200 -u 15100058 -p tm7409TTA -a tcp://180.166.11.33:41205 --daomon_update_instrument >> update_instrment_tools.txt &
40 20 * * * cd /home/ops/production/update_instrment_tools && ./update_instrment_tools --ctp -b 4200 -u 15100058 -p tm7409TTA -a tcp://180.166.11.33:41205 --daomon_update_instrument >> update_instrment_tools.txt &

# 指数每天计算一次 15:30更新的主要是国债数据 有实时的在跑
30 15 * * * /home/ops/production/mdtools/index.sh currentday all        &
# 持仓排名计算
20 15 * * * /home/ops/production/mdtools/index.sh currentday_rank all   &


# 每天重启一次rohon程序 不会自动断线重新登录更新tradingday的问题
30 15 * * * /home/ops/production/monitor/restart_trader.sh &
55 20 * * * /home/ops/production/monitor/restart_trader.sh &

# 行情服务
* * * * *  /home/ops/production/monitor/process_monitor.sh "mdindex_svr" "cd /home/ops/production/mdindex_svr;./mdindex_svr" &
* * * * *  /home/ops/production/monitor/process_monitor.sh "mdpast_svr" "cd /home/ops/production/mdpast_svr;./mdpast_svr" &
* * * * *  /home/ops/production/monitor/process_monitor.sh "mdlive_svr" "cd /home/ops/production/mdlive_svr;./mdlive_svr" &
* * * * *  /home/ops/production/monitor/process_monitor.sh "mdsave_svr" "cd /home/ops/production/mdsave_svr;./mdsave_svr" &


# 119.23.39.36 部署ctp交易服务
* * * * *  /home/ops/production/monitor/process_monitor.sh "ctp_RBDS2" "cd /home/ops/production/ctp_RBDS2;./ctp_RBDS2" &
* * * * *  /home/ops/production/monitor/process_monitor.sh "ctp_JHQHSIMNOW2" "cd /home/ops/production/ctp_JHQHSIMNOW2;./ctp_JHQHSIMNOW2" &
* * * * *  /home/ops/production/monitor/process_monitor.sh "ctp_JHQHSIMNOW4" "cd /home/ops/production/ctp_JHQHSIMNOW4;./ctp_JHQHSIMNOW4" &
* * * * *  /home/ops/production/monitor/process_monitor.sh "ctp_WILL01" "cd /home/ops/production/ctp_WILL01;./ctp_WILL01" &


```


