prefix=/usr/local

all : vrtrack vrcontrol multicastdemoclient JavaDemoClient

vrtrack : vrtrack.cpp
	g++ -Wall `pkg-config fuse --cflags --libs` -O2 -o $@ vrtrack.cpp  -lpthread -lconfig++ -lusb

vrcontrol : vrcontrol.cpp
	g++ -O2 -o $@ vrcontrol.cpp  -lcurses 

multicastdemoclient: multicastdemoclient.cpp
	g++ -O2 -o $@ multicastdemoclient.cpp -lcurses

JavaDemoClient: JavaDemoClient.java
	javac JavaDemoClient.java

clean :
	- rm vrtrack
	- rm multicastdemoclient
	- rm vrcontrol
	- rm JavaDemoClient.class
	- rm TrackingData.class

install : all
	install -m 0755 vrtrack $(prefix)/bin
	install -m 0755 vrcontrol $(prefix)/bin
	install -m 0755 multicastdemoclient $(prefix)/bin

uninstall : all
	rm $(prefix)/bin/vrtrack
	rm $(prefix)/bin/vrcontrol
	rm $(prefix)/bin/multicastdemoclient
