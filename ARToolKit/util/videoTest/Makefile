INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS =
HEADDERS =

all: $(BIN_DIR)/videoTest

$(BIN_DIR)/videoTest: videoTest.o $(OBJS)
	cc -o $(BIN_DIR)/videoTest videoTest.o $(OBJS) $(LDFLAG) $(LIBS)

videoTest.o: videoTest.c $(HEADDERS)
	cc -c $(CFLAG) videoTest.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/videoTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/videoTest
	rm -f Makefile
