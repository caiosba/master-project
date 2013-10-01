PANDAPATH = /home/heekinho/Workspace/panda3d/built
PYTHONPATH = -I/usr/include/python2.7
OPENNI2PATH = /home/heekinho/kinect/OpenNI2
OPENNI2LIBPATH = $(OPENNI2PATH)/Bin/x64-Release 

PANDAPATHINCLUDE = -I$(PANDAPATH)/include
PANDAPATHLIB = -L$(PANDAPATH)/lib
PANDALIBS = $(PANDAPATHLIB) -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub

LIBOPENCV1 = `pkg-config --libs opencv` 
LIBOPENCV2 = -lcxcore231 -lcv231 -lcvaux230 -lhighgui230 -lml230 

FREE_LIBS = -L/usr/local/lib -lfreenect 
OPENNI_LIBS = -L$(OPENNI2LIBPATH) -lOpenNI2

CXX = g++ 

CFLAGS=-fPIC -g -Wall `pkg-config --cflags opencv` -O3 -Wno-switch
INCLUDE = -I/usr/local/include/libfreenect -I$(OPENNI2PATH)/Include -I/usr/include $(PANDAPATHINCLUDE) $(PYTHONPATH)


HEADERFILES := $(foreach dir,src,$(wildcard $(dir)/*.h))
SRC := $(foreach dir,src,$(wildcard $(dir)/*.cpp))
OBJECTFILES := $(patsubst src/%.cpp,src/%.o$,$(SRC))

opencv2-openni : $(HEADERFILES) $(OBJECTFILES)
	$(CXX) $(CFLAGS) $(OBJECTFILES) -o opencv2-openni $(LIBOPENCV1) $(OPENNI_LIBS) $(PANDALIBS)

.cpp.o: $*.h
	@echo "Compiling: " $*
	@($(CXX) -o $*.o -c $(CFLAGS) $(INCLUDE) $*.cpp)
	
clean:
	rm -rf *.o src/*.o opencv2-openni

all: opencv2-openni
