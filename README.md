# china-future-exchange-ctp
<p align="center">
  <img src ="CheNote_Metallic_blacks.png" width="40%" height="40%"/>
</p>

<p align="center">
    <img src ="https://img.shields.io/badge/version-1.1.0-blueviolet.svg"/>
    <img src ="https://img.shields.io/badge/platform-windows|linux|macos-yellow.svg"/>
    <img src ="https://img.shields.io/badge/gcc-4.7+-blue.svg" />
    <img src ="https://img.shields.io/github/workflow/status/vnpy/vnpy/Python%20application/master"/>
    <img src ="https://img.shields.io/github/license/vnpy/vnpy.svg?color=orange"/>
</p>

# 说明

中国商品期货CTP接入服务程序，支持账户交易，期货行情订阅。 通过使用zero ice 对外提供rpc远程调用接口，实现对客户端跨语言的支持。本程序是搭建期货量化交易平台的基础。

##### 支持交易所
  * [CTP](https://www.simnow.com.cn/static/apiDownload.action)：国内期货、期权（上期所、大商所、中金所、郑商所、能源所）
  * [simnow](https://www.simnow.com.cn)：期货模拟仿真平台

# 目录介绍
```
base          基础服务函数库
extern_lib	  使用使用的外部库
init_machine  器配置和和初始化配置信息
server		  交易服务和行情服务程序
shell		  测试用shell的工具
```

# 功能程序介绍

* tdctp_svr 账户交易服务
* mdlive_svr 实时行情服务
* mdpast_svr 历史行情服务
* mdsave_svr 行情存储服务
* mdindex_svr 指数计算服务

# 编译
```
* 编译基础库
cd china-future-exchange-ctp/base && make clean && make
* 编译账户交易服务
cd china-future-exchange-ctp/server/base && make clean && make
cd china-future-exchange-ctp/server/tdctp_svr && make clean && make -f ./Makefile.615
* 编译行情相关服务
#cd china-future-exchange-ctp/server/mdlive_svr && make clean && make -f ./Makefile.615
#cd china-future-exchange-ctp/server/mdpast_svr && make clean && make -f ./Makefile.615
#cd china-future-exchange-ctp/server/mdsave_svr && make clean && make -f ./Makefile.615
#cd china-future-exchange-ctp/server/mdindex_svr && make clean && make -f ./Makefile.615
```
# 运行
* tdctp_svr 账户交易服务

  1. 将编译好的tdctp_svr、tdctp_config.ini 复制到同一个目录
  2. 修改tdctp_config.ini 的[ACCOUNT]段落，为自己的期货账户认证信息
    ```
    [ACCOUNT]
    updatedb = false
    auth = 0000000000000000
    user =simnow
    appid = simnow_client_test
    fundid = 109896
    ```
  3. ./tdctp_svr
