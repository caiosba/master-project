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
GLLIBS = -lGL -lGLU -lglut -lGLEW
LIBS = $(OPENCVLIBS) $(GLLIBS)

# Includes

INCLUDES = -I/usr/include/eigen3 -I/usr/include/eigen3/unsupported

CXX = g++-4.4
CFLAGS = -fPIC -g -w -O3 -Wno-switch -fpermissive -Wno-deprecated

tracker:
	gcc listen-udp.c -o listen-udp $(GLLIBS)
	$(CXX) $(CFLAGS) listen-udp-2.cpp -o listen-udp-2 $(LIBPATHS) $(LIBS) $(INCLUDES) -Wl,-rpath ./

clean:
	rm -f listen-udp listen-udp-2

all: tracker
