//////////////////////////////////////////////////////////////////////////////////
// [ Raw_Icmp_Socket_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net_Raw_Icmp.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [_get_packet]  
//////////////////////////////////////////////////////////////////////////////////
int C_Net_Raw_Icmp::getPacket(UCHAR* Out_pData, DWORD* Out_pcData, 
                              S_IpPacket* In_pS_IpPacket, S_IcmpPacket* In_pS_IcmpPacket,
                              UCHAR* In_pData, DWORD In_cData){

   if(!Out_pData || !Out_pcData || !In_pS_IpPacket || !In_pS_IcmpPacket)
      return(C_NET_RAW_ICMP_ERROR);
   
   ETHERNET_HEADER* pETHERNET_HEADER = (ETHERNET_HEADER*)Out_pData;
   IP_HEADER* pIP_HEADER   = 0x00;
   ICMP_HEADER* pICMP_HEADER = 0x00;

   DWORD cData = 0;

   //////////////////////////////////////////////////////////////////////////
   // ETHERNET_HEADER
   pETHERNET_HEADER->Type = ETH_TYP_IPv4;

   *((DWORD*)pETHERNET_HEADER->MAC_D) = *((DWORD*)In_pS_IpPacket->pETH_MAC_D);
   *((DWORD*)pETHERNET_HEADER->MAC_S) = *((DWORD*)In_pS_IpPacket->pETH_MAC_S);

    *((WORD*)pETHERNET_HEADER->MAC_D + 2) = *((WORD*)In_pS_IpPacket->pETH_MAC_D + 2);
    *((WORD*)pETHERNET_HEADER->MAC_S + 2) = *((WORD*)In_pS_IpPacket->pETH_MAC_S + 2);

   cData += cETHERNET_HEADER;

   pIP_HEADER = (IP_HEADER*)(Out_pData + cData);

   //////////////////////////////////////////////////////////////////////////
   // IP_HEADER
   pIP_HEADER->IP_VerLen   = IP_DEFAULT;
   pIP_HEADER->IP_ToS      = In_pS_IpPacket->pIP_Type;
   pIP_HEADER->IP_cPacket  = htons((WORD)(cIP_HEADER + cICMP_HEADER + In_cData));
   pIP_HEADER->IP_ID       = In_pS_IpPacket->pIP_ID;
   pIP_HEADER->IP_Flags    = In_pS_IpPacket->pIP_Frag;
   pIP_HEADER->IP_Ttl      = In_pS_IpPacket->pIP_TTL;
   pIP_HEADER->IP_Protocol = IP_ICMP;
   pIP_HEADER->IP_Checksum = 0x00;

   *((DWORD*)pIP_HEADER->IP_IP_S) = *((DWORD*)In_pS_IpPacket->pIP_S);
   *((DWORD*)pIP_HEADER->IP_IP_D) = *((DWORD*)In_pS_IpPacket->pIP_D);

   pIP_HEADER->IP_Checksum  = checksum((WORD*)pIP_HEADER, cIP_HEADER);
      
   cData += cIP_HEADER;

   pICMP_HEADER = (ICMP_HEADER*)(Out_pData + cData);

   //////////////////////////////////////////////////////////////////////////
   // ICMP_HEADER
   pICMP_HEADER->ICMP_Type      =  In_pS_IcmpPacket->ICMP_Type;
   pICMP_HEADER->ICMP_Code      =  In_pS_IcmpPacket->ICMP_Code;
   pICMP_HEADER->ICMP_ID        =  In_pS_IcmpPacket->ICMP_ID;
   pICMP_HEADER->ICMP_Sequence  =  In_pS_IcmpPacket->ICMP_Sequence;
   pICMP_HEADER->ICMP_CheckSum  =  0x00;

   //////////////////////////////////////////////////////////////////////////
   // ICMP_CHECKSUM
   char t_Data[5000];

   if(In_pData){
      memcpy(t_Data, pICMP_HEADER, cICMP_HEADER);
      memcpy(t_Data + cICMP_HEADER, In_pData, In_cData);

      pICMP_HEADER->ICMP_CheckSum = checksum((WORD*)t_Data, cICMP_HEADER + In_cData);
   }else{
      pICMP_HEADER->ICMP_CheckSum = checksum((WORD*)pICMP_HEADER, cICMP_HEADER);
   }

   cData += cICMP_HEADER;

   //////////////////////////////////////////////////////////////////////////
   // DATA to Packet
   if(In_pData){
      memcpy(&Out_pData[cData], In_pData, In_cData);
      cData += In_cData;
   }

   *Out_pcData = cData;

   return(C_NET_RAW_ICMP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
 // [ checksum ]
//////////////////////////////////////////////////////////////////////////////////
unsigned short C_Net_Raw_Icmp::checksum(unsigned short *buffer, int size){
   unsigned long cksum = 0;

   while(size > 1){
      cksum += *buffer++;
      size  -=  sizeof(unsigned short);
   }

   if(size) cksum += *(unsigned char*)buffer;

   cksum  = (cksum >> 16) + (cksum & 0xffff);
   cksum += (cksum >> 16);

   return((unsigned short)(~cksum));
}
