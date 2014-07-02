// Vuzix Tracker - This one runs on Windows only :-(

#include <iostream>
#include <fstream>
#include "iWearSDK.h"
#include "iweardrv.h"
#include "iwrstdrv.h"
#include <winsock.h>
#include <windows.h>
#include "IWRFilterTracking.cpp"

#define MAGIC 183
#define RECEIVER_PORT_NUM 6001   /* receiver port number */
#define SENDER_PORT_NUM   8000   /* sender port number   */
#define NUM_TX_LOOPS      20     /* number of time sender transmits data */
#define FILTER 1

/* broadcast IP address for 10.64.172.xxx network using subnetmask 255.255.255.0 */
#define RECEIVER_IP_ADDR "192.168.1.2"
// #define RECEIVER_IP_ADDR "10.1.1.118"

using namespace std;

int main() {

  // First, load iWear lib
  HMODULE m_hIwear = LoadLibrary(_T("IWEARDRV.DLL"));
  
  // Socket stuff
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
       printf("Socket call failed\n");
       return 0;
   }

   memset(&sa, 0, sizeof(struct sockaddr_in));
   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = htonl(INADDR_ANY);
   sa.sin_port = htons(SENDER_PORT_NUM);


   if (bind(socket_fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1)
   {
       printf("Bind to port number %d ,IP address %s failed\n",RECEIVER_PORT_NUM,RECEIVER_IP_ADDR);
       closesocket(socket_fd);
       return 0;
   }

   memset(&ra, 0, sizeof(struct sockaddr_in));
   ra.sin_family = AF_INET;
   ra.sin_addr.s_addr = inet_addr(RECEIVER_IP_ADDR); // broadcast IP address 
   ra.sin_port = htons(RECEIVER_PORT_NUM);

  if (m_hIwear) {

    printf("Opened lib\n");

    // Define some functions
    PIWROPENTRACKER IWROpenTracker = (PIWROPENTRACKER)GetProcAddress(m_hIwear, _T("IWROpenTracker"));
    PIWRGETTRACKING IWRGetTracking = (PIWRGETTRACKING)GetProcAddress(m_hIwear, _T("IWRGetTracking"));
    PIWRGETVERSION IWRGetVersion = (PIWRGETVERSION)GetProcAddress(m_hIwear, _T("IWRGetVersion"));

    // Start tracker values
    LONG yaw = 0,pitch = 0,roll = 0;

    // Open device and get its version
    IWRVERSION ver;
    if (IWROpenTracker() == ERROR_SUCCESS) {
      printf("Device opened\n");
      if (IWRGetVersion(&ver) == ERROR_SUCCESS) {
        printf("Got version\n");
      } else {
        printf("Could not get version\n");
      }
    } else {
      printf("Could not open device\n");
    }

    // Get data from glasses
    DWORD response = IWRGetTracking(&yaw, &pitch, &roll);
    if (response == ERROR_SUCCESS) {
      printf("Got data\n");
    } else {
      printf("Something strange happenned\n");
    }

    // Update data periodically
    while (true) {

      // Get data from glasses
      yaw = yaw/MAGIC;
      pitch = pitch/MAGIC;
      roll = roll/MAGIC;
      
      if (FILTER) {
        IWRFilterTracking(&yaw, &pitch, &roll);
      }
      
      printf("Got data from glasses: yaw: %ld pitch: %ld roll: %ld\n", yaw, pitch, roll);
      
      // Send to socket
      char buf[100];
      int len = snprintf(buf, 100, "%c %ld %ld %ld\n", 'G', yaw, pitch, roll);
      ret = sendto(socket_fd, buf, len, 0, (SOCKADDR*)&ra,sizeof(ra));
      if (ret < 0) { printf("Could not send data to socket\n"); }
      printf("Sent data #%d: %s\n", i, buf);
      
      IWRGetTracking(&yaw, &pitch, &roll);
      i++;
      
      // Don't maximize CPU
      Sleep(100);
    }

  } else {
    printf("Could not open lib\n");
  }
  return 0;
}
