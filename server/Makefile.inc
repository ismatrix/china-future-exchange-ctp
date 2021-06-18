ORIBIN  := $(TARGET)
SOURCE  := $(wildcard *.c *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
CC      := gcc
XX      := g++
LDFLAGS :=
DEFINES :=
CFLAGS  := -Wl,-rpath=/usr/local/lib -Wall -Wno-strict-aliasing -Wno-uninitialized -fasynchronous-unwind-tables -fpermissive  -fomit-frame-pointer -rdynamic $(DEFINES)  $(INCLUDE) #-static
CXXFLAGS:= $(CFLAGS)
FLAGS   ?=
ifeq ($(FLAGS), )
	DBGFLAGS = -g -O0
	RLSFLAGS = -O2
else
	DBGFLAGS = $(FLAGS)
	RLSFLAGS = $(FLAGS)
endif
DISTRDIR?=
ifeq ($(DISTRDIR), )
	override DISTRDIR = /usr/local/release/
endif
INIDIR  ?=
ifneq ($(INIDIR), )
	TEMP := $(INIDIR)/$(TARGET)/
	override INIDIR = $(TEMP)
endif
ININAME ?=
ifeq ($(ININAME), )
	override ININAME = config.ini
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

.PHONY : all release debug clean install

all     : release

release : $(TARGET)
release : CFLAGS += $(RLSFLAGS)
release : CXXFLAGS += $(RLSFLAGS)
debug   : $(TARGET)
debug   : CFLAGS += $(DBGFLAGS)
debug   : CXXFLAGS += $(DBGFLAGS)

clean :
	rm -fr *.d *.o *.log  $(TARGET)

$(TARGET) : $(OBJS)
	$(XX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
.cpp.o :
	$(XX) $(CXXFLAGS) -MMD -MF $*.d -c -o $@ $<
.c.o :
	$(CC) $(CFLAGS) -MMD -MF $*.d -c -o $@ $<

install :
	-mkdir -p $(DISTRDIR)/$(TARGET)
	\cp $(ORIBIN) $(DISTRDIR)/$(TARGET)/$(TARGET)





