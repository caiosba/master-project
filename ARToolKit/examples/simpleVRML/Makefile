UNAME = $(shell uname)

AR_HOME = ../..
AR_CPPFLAGS = -I$(AR_HOME)/include
AR_LDFLAGS = -L$(AR_HOME)/lib

VRML_HOME = /usr
ifeq "$(UNAME)" "Darwin"
    VRML_HOME = /sw
endif
VRML_CPPFLAGS =
VRML_LDFLAGS = -L$(VRML_HOME)/lib

CPPFLAGS = $(AR_CPPFLAGS) $(VRML_CPPFLAGS)
CFLAGS = -O -I/usr/X11R6/include
CXXFLAGS = -O -I/usr/X11R6/include
LDFLAGS = $(AR_LDFLAGS) $(VRML_LDFLAGS) -L/usr/X11R6/lib
LIBS = -lARvrml -lARgsub_lite -lARvideo -lAR \
    -lopenvrml -lopenvrml-gl -lstdc++ -ljpeg -lpng -lz -lm \
    -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
AR = ar
ARFLAGS = rs
RANLIB = 

TARGET = $(AR_HOME)/bin/simpleVRML

HEADERS = \
    object.h

OBJS = \
    object.o \
    simpleVRML.o

default build all: $(TARGET)

$(OBJS) : $(HEADERS)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -f *.o *~ *.bak
	-rm $(TARGET)

allclean:
	-rm -f *.o *~ *.bak
	-rm $(TARGET)
	-rm -f Makefile
