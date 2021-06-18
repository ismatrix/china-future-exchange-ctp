# Date  : 2012-05-23
# Note  :
#    1. the defualt target will make a release version with -O2 option.
#    2. the "debug" target will make a debug version with -g -O0 option.
#    3. make FLAGS=O3 or make debug FLAGS=-g -O1 to change complile flags.
#    4. make install or make dbginstall to do distribution.
#    5. make DISTRDIR=/data/release to change distribution destnation directory.
#    6. make iconv=1 (or any value not empty) to link with standalone libiconv.
#    7. make m=32 to compile with -m32.

SUBDIRS ?= ctp ice future encrypt
DISTR   ?= 
FLAGS   ?=
DISTRDIR?=
ININAME ?=
iconv   ?=
m       ?=

all     : release

release :
	#@for i in $(SUBDIRS) ; do make -j4 -C $$i clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $? ; done
	@for i in $(SUBDIRS) ; do make -j4 -C $$i release "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $? ; done
	@make -j4 -C ctp -f Makefile.615 clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.615 release "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619 clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619 release "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619T clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619T release "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?

debug   :
	#@for i in $(SUBDIRS) ; do make -j4 -C $$i clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $? ; done
	@for i in $(SUBDIRS) ; do make -j4 -C $$i debug "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $? ; done
	@make -j4 -C ctp -f Makefile.615 clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.615 debug "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619 clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619 debug "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619T clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619T debug "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?

clean   :
	@for i in $(SUBDIRS) ; do make -j4 -C $$i clean || exit $? ; done
	@make -j4 -C ctp -f Makefile.615 clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619 clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?
	@make -j4 -C ctp -f Makefile.619T clean "FLAGS=$(FLAGS)" "iconv=$(iconv)" "m=$(m)" || exit $?

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
