//////////////////////////////////////////////////////////////////////////////////
//+++++++++++++++++[ RawSocket_Class_Header ]+++++++++++++++++++++++++++++++++++//
//////////////////////////////////////////////////////////////////////////////////
//
// [::Last modi: 30.08.17 L.ey (µ~)::]  
//
//
#ifndef _C_NET_RAW_H_
 #define _C_NET_RAW_H_
 
 #include <iostream>
 #include <thread> 
 using namespace std;
  
 #include <string.h>
 #include <sys/socket.h>
 #include <unistd.h>
 #include <errno.h>

 #include <linux/if_packet.h>
  
 #include "C_Net_Interface.hpp"

 #include <sigc++/sigc++.h>
  
//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_RAW_READY  = 0x01;
 const int C_NET_RAW_ERROR  = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 class C_Net; // signal
 
 class C_Net_Raw {
     
    public: 

       C_Net_Raw();  
      ~C_Net_Raw();  

       int open(const S_Net_Interface* pSInterface, C_Net* pCNet);
       int close();

       int send(unsigned char* pData, unsigned int cData);

       // Non Blocking
       int start(unsigned char* pBuffer, unsigned int cBuffer, int* pcData);
       int stop();

    private:

       struct sockaddr_ll socket_address;

       int  sockfd;

       bool bOpen, bRun;

       /////////////////
         
       unsigned char* pBuffer;
       unsigned int   cBuffer;
       
       /////////////////
       
       C_Net* pCNet;
       int* pcData;

       // thread
       void run();       
       thread m_thread;
 };

#endif // _C_NET_RAW_H_
