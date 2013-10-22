#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../control_cube/cube.c"

#define SERVER_PORT 6001

int sock;

void listenToUDP() {
  char code;
  char message[1024];
  long yaw, pitch, roll;
  int bytes;

  bytes = read(sock, message, 1024);

  if (bytes > 0) {
    message[bytes] = '\0';
    printf("Received: %s\n", message);

    sscanf(message, "%c %ld %ld %ld\n", &code, &yaw, &pitch, &roll);

    // Data coming from the glasses
    if (code == 'G') {
      printf("Data coming from the glasses\n");
      printf("Yaw: %d Pitch: %d Roll: %d\n", yaw, pitch, roll);
      controlCube(yaw, pitch, roll);
    }
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
 
  initiateCube(sock);
  close(sock);
}
