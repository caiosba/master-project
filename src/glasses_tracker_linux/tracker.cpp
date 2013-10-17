/*
Copyright (c) 2012, Justin Philipp Heinermann and Jendrik Poloczek
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#include "Logger.h" 
#include "AttitudeSensor.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define THRESHOLD 2
#define ZERO_YAW 0
#define ZERO_PITCH 0
#define ZERO_ROLL 19
#define LIMIT 52 
#define INTERVAL 0.2 // Seconds

#define RECEIVER_IP_ADDR "10.1.1.118"
#define RECEIVER_PORT_NUM 6001   /* receiver port number */
#define SENDER_PORT_NUM   8000   /* sender port number   */

AttitudeSensor * as;
const Head * head;
bool run=true;
int counter=0;

void error(const char *msg) {
  perror(msg);
  exit(0);
}

int main(){
  int sockfd, n;
  struct sockaddr_in serv_addr, sa;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  server = gethostbyname(RECEIVER_IP_ADDR);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  memset(&sa, 0, sizeof(struct sockaddr_in));
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  sa.sin_port = htons(SENDER_PORT_NUM);

  if (bind(sockfd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1) {
    printf("Bind to port number %d ,IP address %s failed\n",RECEIVER_PORT_NUM,RECEIVER_IP_ADDR);
    close(sockfd);
    return 0;
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(RECEIVER_PORT_NUM);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");

	as = new AttitudeSensor();
	if(as->vuzixConnected){
		while(run){
			if(!as->vuzixConnected){
				run=false;
				break;
			}
			as->timerProc();
			head=as->getHeadDirection();

      int yaw = round(((head->angles.yawDeg * 180 / M_PI) / 1000) - ZERO_YAW);
      int pitch = round(((head->angles.pitchDeg * 180 / M_PI) / 1000) - ZERO_PITCH);
      int roll = round(((head->angles.rollDeg * 180 / M_PI) / 1000) - ZERO_ROLL);

      if (yaw >= -THRESHOLD && yaw <= THRESHOLD) yaw = 0;
      if (pitch >= -THRESHOLD && pitch <= THRESHOLD) pitch = 0;
      if (roll >= -THRESHOLD && roll <= THRESHOLD) roll = 0;

      char buf[100];
      int len = snprintf(buf, 100, "%c %d %d %d\n", 'G', yaw, pitch, roll);
      n = write(sockfd, buf, len);
      if (n < 0) error("ERROR writing to socket");
      sleep(INTERVAL);

      counter++;
		}
	}
  close(sockfd);
}
