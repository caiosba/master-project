INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS = object.o
HEADDERS = object.h

all: $(BIN_DIR)/loadMultiple

$(BIN_DIR)/loadMultiple: loadMultiple.o $(OBJS)
	cc -o $(BIN_DIR)/loadMultiple loadMultiple.o $(OBJS) $(LDFLAG) $(LIBS)

loadMultiple.o: loadMultiple.c $(HEADDERS)
	cc -c $(CFLAG) loadMultiple.c

object.o: object.c $(HEADDERS)
	cc -c $(CFLAG) object.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/loadMultiple

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/loadMultiple
	rm -f Makefile
