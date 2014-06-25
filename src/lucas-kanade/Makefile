# Includes

INCPATH = -I/usr/include
LOCALINCPATH = -I/usr/local/include
INCLUDES = $(INCPATH) $(LOCALINCPATH)

# Lib paths

LIBPATH = -L/usr/lib/
LOCALLIBPATH = -L/usr/local/lib
LIBPATHS = $(LOCALLIBPATH) $(LIBPATH)

# Libs

OPENCVLIBS = -lopencv_core -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_calib3d -lopencv_video
LIBS = $(OPENCVLIBS)

CXX = g++-4.4
CFLAGS = -fPIC -g -w -O3 -Wno-switch

HEADERFILES := $(foreach dir,src,$(wildcard $(dir)/*.h))
SRC := $(foreach dir,src,$(wildcard $(dir)/*.cpp))
OBJECTFILES := $(patsubst src/%.cpp,src/%.o$,$(SRC))

lkdemo : $(HEADERFILES) $(OBJECTFILES)
	$(CXX) $(CFLAGS) $(OBJECTFILES) -o lkdemo $(LIBPATHS) $(LIBS) -Wl,-rpath ./

.cpp.o: $*.h
	@echo "Compiling: " $*
	@($(CXX) -o $*.o -c $(CFLAGS) $(INCLUDES) $*.cpp)

clean:
	rm -rf *.o src/*.o lkdemo

all: lkdemo
