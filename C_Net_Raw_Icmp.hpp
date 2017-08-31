//////////////////////////////////////////////////////////////////////////////////
// [ Raw_Icmp_Socket_Class_Header ] 
//////////////////////////////////////////////////////////////////////////////////
//
//
//
// [::Last modi: 28.08.17 L.ey (µ~)::]  
//
//
#ifndef _C_NET_RAW_ICMP_H_
 #define _C_NET_RAW_ICMP_H_

 #include "Net_Protokoll_Header.hpp"

 #include <cstring>
 #include <arpa/inet.h>
 
//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_RAW_ICMP_READY = 0x01;
 const int C_NET_RAW_ICMP_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 class C_Net_Raw_Icmp {
     
    public: 

       C_Net_Raw_Icmp(){};  
      ~C_Net_Raw_Icmp(){}; 

       int getPacket(UCHAR* Out_pData, DWORD* Out_pcData, 
                     S_IpPacket* In_pS_IpPacket, S_IcmpPacket* In_pS_IcmpPacket,
                     UCHAR* In_pData, DWORD In_cData);
    private:

       WORD checksum(WORD* pbuffer, int cBuffer);
 };

#endif // _C_NET_RAW_ICMP_H_
