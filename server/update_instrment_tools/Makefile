TARGET  := update_instrment_tools


LIBS    +=  -Wl,-Bstatic -L../base/ -lfuture -lctp.615 -lcommon -lencrypt  ${LIB_CTP_TD_615}

INCLUDE += -I./  -I../base/ -D  __CTP__VERSION__=615

include ../../Makefile

include ../Makefile.inc

