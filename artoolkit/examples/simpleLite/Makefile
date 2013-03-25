INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub_lite -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS =
HEADERS =

all: $(BIN_DIR)/simpleLite

$(BIN_DIR)/simpleLite: simpleLite.o $(OBJS)
	cc -o $(BIN_DIR)/simpleLite simpleLite.o $(OBJS) $(LDFLAG) $(LIBS)

simpleLite.o: simpleLite.c $(HEADERS)
	cc -c $(CFLAG) simpleLite.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleLite

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleLite
	rm -f Makefile
