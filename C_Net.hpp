//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Net
//
// [::Last modi: 16.08.17 L.ey (µ~)::]
//
//
#ifndef _C_NET_H_
 #define _C_NET_H_
 
   #include <iostream>
   #include <fstream>
   #include <string>
   #include <sstream>
   #include <iomanip>
   #include <mutex>
   
   #include <errno.h>
   
   using namespace std;
 
   #include <sigc++/sigc++.h>
   
   #include <glibmm/dispatcher.h>
   
   #include "Net_Protokoll_Header.hpp"
   
   #include "C_Net_Interface.hpp"
   #include "C_Net_Raw.hpp"
   #include "C_Net_Raw_Icmp.hpp"
   
   typedef unsigned char  UCHAR;
   typedef unsigned short WORD;
   typedef unsigned int   DWORD;
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
   const int C_NET_ERROR = 0x00;
   const int C_NET_READY = 0x01;

   const int C_NET_BUFFER = 0x1400;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

   
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
   
   class C_Net { // : public sigc::trackable
     
      public:
	
         C_Net();
        ~C_Net();

         int start(const S_Net_Interface* pSInterface);
         int stop();
         
         int send(S_IpPacket* pSIP, S_IcmpPacket* pSIcmp, int cPackets, int cSleep, const char* pData);

         ///////////////////////////////////////////////////////////////
       
         C_Net_Interface  CNInterface;

         array<unsigned char, C_NET_BUFFER> CA_Icmp;
         array<unsigned char, C_NET_BUFFER> CA_Buffer;
         
         int cData;

         C_Net_Raw_Icmp CNIcmp;
         C_Net_Raw      CNRaw;

         /////////////////////////////////////////
         // Signal
         Glib::Dispatcher m_Dispatcher;
         
         void notify();
         void on_data();
         
         mutable mutex m_Mutex;

      private: 

         int status;
   };
 
#endif // _C_NET_H_
 
