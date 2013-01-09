aptitude install g++ libusb-dev ncurses-dev openjdk-6-jdk libfuse-dev wget libconfig++8 libconfig-dev
wget http://mmc.geofisica.unam.mx/debian/pool/main/libc/libconfig/libconfig++8-dev_1.3.2-2_i386.deb
wget http://mmc.geofisica.unam.mx/debian/pool/main/libc/libconfig/libconfig++8-dev_1.3.2-2_i386.deb
wget http://mmc.geofisica.unam.mx/debian/pool/main/libc/libconfig/libconfig++8_1.3.2-2_i386.deb
wget http://mmc.geofisica.unam.mx/debian/pool/main/libc/libconfig/libconfig8-dev_1.3.2-2_i386.deb
wget http://mmc.geofisica.unam.mx/debian/pool/main/libc/libconfig/libconfig8_1.3.2-2_i386.deb

dpkg -i libconfig8_1.3.2-2_i386.deb  
dpkg -i libconfig++8-dev_1.3.2-2_i386.deb 

dpkg -i libconfig8-dev_1.3.2-2_i386.deb 
dpkg -i libconfig++8-dev_1.3.2-2_i386.deb 

echo Now you may run make or use the binary driver
