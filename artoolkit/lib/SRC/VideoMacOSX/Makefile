UNAME = $(shell uname)

AR_HOME = ../../..

CPPFLAGS = -I$(AR_HOME)/include
CFLAGS = -fpascal-strings -O -I/usr/X11R6/include
CXXFLAGS = -O -I/usr/X11R6/include
LDFLAGS = -L$(AR_HOME)/lib -L/usr/X11R6/lib
LIBS = -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
AR = ar
ARFLAGS = rs
RANLIB = 

TARGET = $(AR_HOME)/lib/libARvideo.a

HEADERS = \
	$(AR_HOME)/include/AR/video.h \
    videoInternal.h \
	ARVideoSettingsController.h

OBJS = \
    video.o \
    ARVideoSettingsController.o
	
# Implicit rule, to compile Objective-C files with the .m suffix.
%.o : %.m
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

default build all: $(TARGET)

$(OBJS) : $(HEADERS)

$(TARGET): $(OBJS)
	$(AR) ruv $@ $?
	$(RANLIB) $@

clean:
	-rm -f *.o *~ *.bak
	-rm $(TARGET)

allclean:
	-rm -f *.o *~ *.bak
	-rm $(TARGET)
	-rm -f Makefile
