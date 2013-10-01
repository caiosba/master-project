INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lARMulti -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS = paddle.o
HEADDERS = paddle.h

all: $(BIN_DIR)/paddleInteractionTest

$(BIN_DIR)/paddleInteractionTest: paddleInteractionTest.o $(OBJS)
	cc -o $(BIN_DIR)/paddleInteractionTest paddleInteractionTest.o $(OBJS) $(LDFLAG) $(LIBS)

paddleInteractionTest.o: paddleInteractionTest.c $(HEADDERS)
	cc -c $(CFLAG) paddleInteractionTest.c

paddle.o: paddle.c $(HEADDERS)
	cc -c $(CFLAG) paddle.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/paddleInteractionTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/paddleInteractionTest
	rm -f Makefile
