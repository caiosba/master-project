#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../control_cube/cube2.c"
#include <math.h>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/Cholesky>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <iostream>

#define SERVER_PORT 6003

int sock;

void listenToUDP() {
  char code;
  char message[1024];
  float r1, r2, r3, r4, r5, r6, r7, r8, r9, t1, t2, t3;
  int bytes;
	Eigen::Vector3f t;
	Eigen::Matrix3f r;

  bytes = read(sock, message, 1024);

  if (bytes > 0) {
    message[bytes] = '\0';
    printf("Received: %s\n", message);

    sscanf(message, "%f %f %f %f %f %f %f %f %f|%f %f %f", &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &t1, &t2, &t3);
		r << r1, r2, r3,
		     r4, r5, r6,
				 r7, r8, r9;
	  t = { t1, t2, t3 };
		std::cout << "Rotation:" << std::endl;
		std::cout << r << std::endl;
		std::cout << "Translation:" << std::endl;
		std::cout << t << std::endl;
    controlCube(r, t);
  }
}

main(int argc, char *argv[]) {
  struct sockaddr_in name;
  struct hostent *hp, *gethostbyname();

  printf("Listen activating.\n");

  /* Create socket from which to read */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)   {
    perror("Opening datagram socket\n");
    exit(EXIT_FAILURE);
  }
  
  /* Bind our local address so that the client can send to us */
  bzero((char *) &name, sizeof(name));
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  name.sin_port = htons(SERVER_PORT);
  
  if (bind(sock, (struct sockaddr *) &name, sizeof(name))) {
    perror("Binding datagram socket\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Socket has port number #%d\n", ntohs(name.sin_port));
 
  initiateCube();
  close(sock);
}
