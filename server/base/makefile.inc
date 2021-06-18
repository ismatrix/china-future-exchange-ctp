SOURCE  := $(wildcard *.c *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
CC      := gcc
XX      := g++
LDFLAGS :=
DEFINES :=
CFLAGS  := -Wall -Wno-strict-aliasing -Wno-uninitialized $(DEFINES) $(INCLUDE)
ifneq ($(OS),Windows_NT)
	CFLAGS += -fPIC
endif
CXXFLAGS:= $(CFLAGS)
CFLAGS  += -std=gnu99
FLAGS   ?=
ifeq ($(FLAGS), )
	DBGFLAGS = -g -O0
	RLSFLAGS = -O2
else
	DBGFLAGS = $(FLAGS)
	RLSFLAGS = $(FLAGS)
endif

iconv   ?=
ifneq ($(iconv), )
	LIBS += -liconv
endif
m       ?=
ifneq ($(m), )
	CFLAGS += -m$(m)
	CXXFLAGS += -m$(m)
endif

.PHONY : all release debug clean install installini

all     : release

release : $(TARGET)
release : CFLAGS += $(RLSFLAGS)
release : CXXFLAGS += $(RLSFLAGS)
debug   : $(TARGET)
debug   : CFLAGS += $(DBGFLAGS)
debug   : CXXFLAGS += $(DBGFLAGS)

clean :
	rm -fr *.d *.o $(TARGET)
cleantarget:
	rm -fr $(TARGET)

-include *.d

.cpp.o :
	$(XX) $(CXXFLAGS) -MMD -MF $*.d -c -o $@ $<
.c.o :
	$(CC) $(CFLAGS) -MMD -MF $*.d -c -o $@ $<

install :

installini:

stop :

start:

restart : stop start

alive:

binbackup:

inibackup:
