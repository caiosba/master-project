#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

void diff(timespec start, timespec end, timespec* dif) {
	if ((end.tv_nsec-start.tv_nsec)<0) {
		dif->tv_sec = end.tv_sec-start.tv_sec-1;
		dif->tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		dif->tv_sec = end.tv_sec-start.tv_sec;
		dif->tv_nsec = end.tv_nsec-start.tv_nsec;
	}
}

int main(int argc, char* argv[]) {
  if(argc <= 1) {
    fprintf(stderr, "Device not set\n");
    return 1;
  }
  int mode = 1;//atoi(argv[2]);
  
  ifstream file;
  file.open(argv[1], ios::in|ios::binary);

  int limit = 100; // try for 10 seconds
  while(!file.is_open() && limit > 0) {
    usleep(100000);
    limit--;
  }
  
  if(file.is_open()) {
    // Unbuffered reading 
    file.rdbuf()->pubsetbuf(0, 0);

    // 42-byte blocks
    char* block = new char[42];
    int16_t* block16 = (int16_t*)block;

    timespec start, now, dif;
    clock_gettime(CLOCK_REALTIME, &start);
    
    // while(true) {
      file.read(block, 42);

      clock_gettime(CLOCK_REALTIME, &now);
      diff(start, now, &dif);
      
      switch(mode) {
        case 0: 
          printf("Time %ld:%ld Magnetometer %d %d %d Accelerometer %d %d %d Low sensivity gyro %d %d %d High sensivity gyro %d %d %d\n", 
              dif.tv_sec, dif.tv_nsec,                // Time
              block16[1], block16[2], block16[3],     // Magnetometer 
              block16[4], block16[5], block16[6],     // Accelerometer
              block16[7], block16[8], block16[9],     // Low sensitivity Gyro
              block16[10], block16[11], block16[12]); // High sensitivity Gyro
        break;
        case 1:
          printf("Magnetometer %d %d %d\nAccelerometer %d %d %d\nLow sensivity gyro %d %d %d\nHigh sensivity gyro %d %d %d\n", 
              block16[1], block16[2], block16[3],     // Magnetometer 
              block16[4], block16[5], block16[6],     // Accelerometer
              block16[7], block16[8], block16[9],     // Low sensitivity Gyro
              block16[10], block16[11], block16[12]); // High sensitivity Gyro
        break;
        case 2:
          //TODO - output actual heading
        break;
     } 

      fflush(stdout);
    // }
    file.close();
  } else {
    fprintf(stderr, "Could not open device\n");
    return 2;
  }
  return 0;
}
