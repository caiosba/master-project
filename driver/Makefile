CPP      = /usr/bin/g++
INCLUDEDIR = src/
CPPFLAGS  = -Wall -I$(INCLUDEDIR)
SRC = src/AttitudeSensor.cpp src/Head.cpp src/example/main.cpp 
HEADERS = src/AttitudeSensor.h src/Head.h src/Logger.h 

example: $(SRC) $(HEADERS)
	$(CPP) $(CPPFLAGS) -o example $(SRC) 
