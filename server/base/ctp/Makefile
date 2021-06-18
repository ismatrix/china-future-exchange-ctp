# Uncomment the following line for Solaris # pony add "-lm" 990221
# C_LINK = -lsocket -lnsl -lm

# Uncomment this for SCO.  (Note, this has only been reported to work with
# Revision 3.2.4 with the "SCO TCP/IP Development System" package installed.
# Please let me know if you have any other SCO success stories.
# C_LINK = -lsocket

TARGET  := ../libctp.a

include ../../../Makefile
include ../makefile.inc


$(TARGET) : $(OBJS)
	ar -rs $@ $^
	chmod 755 $(TARGET)

