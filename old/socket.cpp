// compile at the command prompt using "cl udp_sender.c ws2_32.lib". 
// If cl is not found execute vcvars32.bat file to set up the visual studio 
// enviroment. 
// VCVARS32.bat can be found in c:\program files\Microsoft Visual Studio x"\vc\bin

#include <iostream>
#include <fstream>
#include <winsock.h>
#include <windows.h>

#define RECEIVER_PORT_NUM 6001   /* receiver port number */
#define SENDER_PORT_NUM   8000   /* sender port number   */
#define NUM_TX_LOOPS      20     /* number of time sender transmits data */

/* broadcast IP address for 10.64.172.xxx network using subnetmask 255.255.255.0 */
#define RECEIVER_IP_ADDR "192.168.129.119"

using namespace std;
int main()
{
SOCKET socket_fd;
struct sockaddr_in sa,ra;
WSADATA wsdata;
DWORD ver_req,ret;
int i= 0;
char data_buffer[80]; 

  /* initialize winsock */
   ver_req = MAKEWORD( 1, 1);
   ret = WSAStartup(ver_req, &wsdata);

   socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
	
   if ( socket_fd < 0 )
   {
       printf("socket call failed");
       return 0;
   }

   memset(&sa, 0, sizeof(struct sockaddr_in));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = htonl(INADDR_ANY);
   sa.sin_port = htons(SENDER_PORT_NUM);


   if (bind(socket_fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1)
   {
       printf("Bind to Port Number %d ,IP address %s failed\n",RECEIVER_PORT_NUM,RECEIVER_IP_ADDR);
       closesocket(socket_fd);
       return 0;
   }

   memset(&ra, 0, sizeof(struct sockaddr_in));
   ra.sin_family = AF_INET;
   ra.sin_addr.s_addr = inet_addr(RECEIVER_IP_ADDR); // broadcast IP address 
   ra.sin_port = htons(RECEIVER_PORT_NUM);

   while(1)
   {
       i++;
       char * buf = "Hello World\n";
       ret = sendto(socket_fd, buf, 12, 0, (SOCKADDR*)&ra,sizeof(ra));

       if(ret < 0) { printf("sendto failed\n"); }

       printf("send loop #%d (%s)\n",i,buf);
       Sleep(200);
   }

   printf("sender exiting\n");
   closesocket(socket_fd);
   return 0;
}
