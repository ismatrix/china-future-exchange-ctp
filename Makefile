BASE_DIR=$(shell pwd | awk -F'backend' '{print $$1 "backend"}')

LIB_CTP_TD=-Wl,-Bdynamic  /usr/local/lib/thosttraderapi.15.so
LIB_CTP_MD=-Wl,-Bdynamic  /usr/local/lib/thostmduserapi.15.so

LIB_CTP_TD_613=-Wl,-Bdynamic  /usr/local/lib/6.3.13_20181119/thosttraderapi_se.so
LIB_CTP_MD_613=-Wl,-Bdynamic  /usr/local/lib/6.3.13_20181119/thostmduserapi_se.so

LIB_CTP_TD_615=-Wl,-Bdynamic  /usr/local/lib/SFIT_CTP_6.3.15_traderapi_server/thosttraderapi_se.so
LIB_CTP_MD_615=-Wl,-Bdynamic  /usr/local/lib/SFIT_CTP_6.3.15_traderapi_server/thostmduserapi_se.so

LIB_CTP_TD_619=-Wl,-Bdynamic  /usr/local/lib/v6.3.19_P1_20200106_api/thosttraderapi_se.so
LIB_CTP_MD_619=-Wl,-Bdynamic  /usr/local/lib/v6.3.19_P1_20200106_api/thostmduserapi_se.so

LIB_CTP_TD_619T=-Wl,-Bdynamic  /usr/local/lib/v6.3.19_T1_20200423_api/thosttraderapi_se.so
LIB_CTP_MD_619T=-Wl,-Bdynamic  /usr/local/lib/v6.3.19_T1_20200423_api/thostmduserapi_se.so


LIB_RONGHANG_TD=-Wl,-Bdynamic  /usr/local/lib/thosttraderapi_rohon_test.15.so
LIB_RONGHANG_TD_613=-Wl,-Bdynamic,-rpath=/usr/local/lib/rohon615  /usr/local/lib/rohon613/thosttraderapi_se.so
LIB_RONGHANG_TD_615=-Wl,-Bdynamic,-rpath=/usr/local/lib/rohon615  /usr/local/lib/rohon615/thosttraderapi_se.so
LIB_RONGHANG_TD_619=-Wl,-Bdynamic,-rpath=/usr/local/lib/rohon619  /usr/local/lib/rohon619/thosttraderapi_se.so

LIBS_SHARE= -Wl,-Bdynamic   -lIce -lIceUtil -lIceDiscovery -lIceSSL -lmongoc-1.0 -lpthread -lcrypto -lssl  -lbson-1.0 -lev -ljsoncpp 
LIBS_STATIC=  -Wl,-Bstatic   -lutil_base

LIBS+=-L/usr/lib  -L/usr/local/lib -L/usr/lib64  -L/usr/local/Ice-3.6.3/lib64   -L${BASE_DIR}/extern_lib/lib  -L${BASE_DIR}/base/ ${LIBS_STATIC} ${LIBS_SHARE}

INCLUDE+=-I/usr/include/libbson-1.0/ -I/usr/include/jsoncpp/  -I/usr/include -I/usr/local/include   -I/usr/local/Ice-3.6.3/include/ -I${BASE_DIR}/base -I${BASE_DIR}/extern_lib/include/ 

