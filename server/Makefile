# Date  : 2012-05-23
# Note  :
#    1. the defualt target will make a release version with -O2 option.
#    2. the "debug" target will make a debug version with -g -O0 option.
#    3. make FLAGS=O3 or make debug FLAGS=-g -O1 to change complile flags.
#    4. make install or make dbginstall to do distribution.
#    5. make DISTRDIR=/data/release to change distribution destnation directory.
#    6. make iconv=1 (or any value not empty) to link with standalone libiconv.
#    7. make m=32 to compile with -m32.

SUBDIRS ?= base mdlive_svr mdsave_svr mdtools tdctp_svr mdpast_svr mdindex_svr
DISTR   ?= 
FLAGS   ?=
DISTRDIR?=
ININAME ?=
iconv   ?=
m       ?=

all     : release

release :
	@for i in $(SUBDIRS) ; do make -j4 -C $$i release "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $? ; done
debug   :
	@for i in $(SUBDIRS) ; do make  -C $$i debug "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $? ; done

clean   :
	@for i in $(SUBDIRS) ; do make -j4 -C $$i clean || exit $? ; done
cleantarget:
	@for i in $(SUBDIRS) ; do make -j4 -C $$i cleantarget || exit $? ; done

install :
	@for i in $(DISTR) ; do make -j4 -C $$i install "DISTRDIR=$(DISTRDIR)" "ININAME=$(ININAME)" "m=$(m)" || exit $? ; done
stop :

start :

restart :

alive :

binbackup :

inibackup :
