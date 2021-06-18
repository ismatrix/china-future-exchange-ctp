# Uncomment the following line for Solaris # pony add "-lm" 990221
# C_LINK = -lsocket -lnsl -lm

# Uncomment this for SCO.  (Note, this has only been reported to work with
# Revision 3.2.4 with the "SCO TCP/IP Development System" package installed.
# Please let me know if you have any other SCO success stories.
# C_LINK = -lsocket
include ../Makefile
SOURCE  := $(wildcard *.c *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
CC      := gcc
XX      := g++
LDFLAGS :=
DEFINES :=
CFLAGS  := -Wall -Wno-strict-aliasing -Wno-uninitialized $(DEFINES) $(INCLUDE) 
CXXFLAGS  := -Wall -Wno-strict-aliasing -Wno-uninitialized $(DEFINES) $(INCLUDE) 


TARGET=libutil_base.a

OBJSC= Attr_API.o Attr_API_Str.o Attr_API_Var.o base64.o crc32.o crypt_3des.o  crypt_md5.o crypt_sha1.o crypt_sha256.o debug.o fifo.o http.o pack.o py.o  shm.o utf8.o wry.o ut_str.o util_base64.o
OBJCPP=  conf.o crypto.o crypt_tea.o md5.o mongoc_json.o RollLog.o convert.o util.o csocket.o ssl_csocket.o smtp_send_mail.o
OBJS=$(OBJSC) $(OBJCPP)

CC = gcc -O3 -Wall -fno-strict-aliasing -fomit-frame-pointer -fasynchronous-unwind-tables -fPIC 
XX = g++ -O3 -Wall -fno-strict-aliasing -fomit-frame-pointer -fasynchronous-unwind-tables -fPIC 


.PHONY : all release debug clean

all     : release

release : $(TARGET)
release : CFLAGS += -O3 
release : CXXFLAGS += -O3 
debug   : $(TARGET)
debug   : CFLAGS += -g -O0 
debug   : CXXFLAGS += -g -O0

clean :
	rm -fr *.d *.o $(TARGET)

-include *.d

$(TARGET) : $(OBJS)
	$(XX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

.cpp.o :
	    $(XX) $(CXXFLAGS) -MMD -MF $*.d -c -o $@ $< 
.c.o :
	    $(CC) $(CFLAGS) -MMD -MF $*.d -c -o $@ $<


$(TARGET) : $(OBJS)
	ar -rs $@ $^
	chmod 755 $(TARGET)


