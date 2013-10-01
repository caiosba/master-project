INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS = object.o
HEADDERS = object.h

all: $(BIN_DIR)/collideTest

$(BIN_DIR)/collideTest: collideTest.o $(OBJS)
	cc -o $(BIN_DIR)/collideTest collideTest.o $(OBJS) $(LDFLAG) $(LIBS)

collideTest.o: collideTest.c $(HEADDERS)
	cc -c $(CFLAG) collideTest.c

object.o: object.c $(HEADDERS)
	cc -c $(CFLAG) object.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/collideTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/collideTest
	rm -f Makefile
