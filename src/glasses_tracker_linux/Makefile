CPP      = /usr/bin/g++
INCLUDEDIR = .
CPPFLAGS  = -Wall -I$(INCLUDEDIR)
SRC = AttitudeSensor.cpp Head.cpp tracker.cpp
HEADERS = AttitudeSensor.h Head.h Logger.h 

tracker: $(SRC) $(HEADERS)
	$(CPP) $(CPPFLAGS) -o tracker $(SRC) 
