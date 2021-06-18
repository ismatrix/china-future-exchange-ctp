# server
include the direction: 
```
mdlive_svr      实时行情server
mdsave_svr      行情持久化server
mdtools         shell工具集
tdsungard_svr   金仕达交易server
base            业务基础库
mdpast_svr      获取历史行情服务
tdctp_svr       ctp交易server
mdindex_svr     指数server
update_instrment_tools 所有server相关测试工具  
```

##  base
```
ctp         ctp封库
sungard     金仕达库
ice         ice文件集合
future      业务功能函数库

```

## mdlive_svr mdindex_svr tdctp_svr
release:    
``` bash
cd /home/ops/yunwei/release
./release_svr.sh  buid_release commit mdlive_svr
./release_svr.sh  buid_release commit mdindex_svr
    
```

## mdpast_svr
release(config.server ice连接相关配置有需要手工修改发布):
``` bash
./release_svr.sh  buid_release commit mdpast_svr

```

## tdctp_svr 
release:
``` bash
cd /home/ops/yunwei/release
./release_svr.sh  buid_release commit tdctp_svr fundid

```
## compile
```bash
compile all:
cd /home/ops/dreamy/backend/server
make clean && make release

compile only one server:
cd mdlive_svr
make clean && make release
```



