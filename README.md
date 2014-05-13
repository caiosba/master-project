This is my master project on Computer Science at Federal University of Bahia - Brazil.

This repository is organized the following way:

* misc: codes not directly related to the project... includes testing code, related stuff and so on
* src: the project code
* old: old stuff, not used anymore (like VRML, ARToolKit, etc.)

In order to run the system:

* Copy the *.rules files to /etc/udev/rules.d, restart the udev service and connect the glasses to the computer... check if a /dev/wrap920AR device was created
* Enter each directory on src/ and compile each component (there is a README and a Makefile for each component)
* Define the IP for each component
* For the listener, open the port: sudo iptables -I INPUT -p udp --dport 6001 -j ACCEPT
* Run each component
* Run the listener
