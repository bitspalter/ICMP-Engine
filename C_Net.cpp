//////////////////////////////////////////////////////////////////////////////////
// [ Net_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_Net.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]
//////////////////////////////////////////////////////////////////////////////////
C_Net::C_Net(){
   if(CNInterface.create() != C_NET_INTERFACE_READY){
      cout << "ERROR: CNInterface.create" << endl;
   } 
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_Net::~C_Net(){
   this->stop();
}
//////////////////////////////////////////////////////////////////////////////////
// [ start ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::start(const S_Net_Interface* pSInterface){

   if(CNRaw.open(pSInterface, this) != C_NET_RAW_READY){
      cout << "ERROR: CNRaw.open" << endl;
      return(C_NET_ERROR);
   }
   
   if(CNRaw.start(&CA_Icmp[0], C_NET_BUFFER, &cData) != C_NET_RAW_READY){
      cout << "ERROR: CNRaw.start" << endl;
      return(C_NET_ERROR);
   }

   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ stop ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::stop(){
    
   if(CNRaw.stop() != C_NET_RAW_READY){
      cout << "ERROR: CNRaw.stop" << endl;
      return(C_NET_ERROR);       
   }
   
   if(CNRaw.close() != C_NET_RAW_READY){
      cout << "ERROR: CNRaw.close" << endl;
      return(C_NET_ERROR);       
   }
   
   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ send ]
//////////////////////////////////////////////////////////////////////////////////
int C_Net::send(S_IpPacket* pSIP, S_IcmpPacket* pSIcmp, int cPackets, int cSleep, const char* pData){

   if(!pSIcmp || !pSIP || !cPackets) return(C_NET_ERROR);
   
   UCHAR pPacket[5000];
   DWORD cPacket = 0;
   
   UCHAR pPayload[1500 - (sizeof(S_IpPacket) + sizeof(S_IcmpPacket))];
   DWORD cPayload = 0;
   
   if(pData){
      
      cPayload = strlen(pData);
      
      if(cPayload > 1500 - (sizeof(S_IpPacket) + sizeof(S_IcmpPacket))){
         cout << "Data to Big" << endl;
         return(C_NET_ERROR);
      }
      
      memcpy(pPayload, pData, strlen(pData));
      CNIcmp.getPacket(pPacket, &cPacket, pSIP, pSIcmp, pPayload, strlen(pData));
   }else{
      CNIcmp.getPacket(pPacket, &cPacket, pSIP, pSIcmp, 0, 0);
   }
   
   usleep(50000);
   
   for(int n = 0; n < cPackets; n++){
      CNRaw.send(pPacket, cPacket);
      usleep(cSleep * 1000);
   }
  
   usleep(50000);
   
   return(C_NET_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ notify ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::notify(){
   m_Dispatcher.emit();
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_Net::on_data(){

   if(cData < (int)(cETHERNET_HEADER + cIP_HEADER + cICMP_HEADER)) return;

   {
      lock_guard<mutex> lock(m_Mutex);
      if(cData < C_NET_BUFFER) memcpy(&CA_Buffer[0], &CA_Icmp[0], cData);
      else return;
   }
      
   ETHERNET_HEADER* pRCV_ethhdr = (ETHERNET_HEADER*)&CA_Buffer[0];
   IP_HEADER*       pRCV_ip     = (IP_HEADER*)(&CA_Buffer[cETHERNET_HEADER]);
   ICMP_HEADER*     pRCV_icmp   = 0;
      
   if(pRCV_ethhdr->Type == ETH_TYP_IPv4){
      if(pRCV_ip->IP_Protocol == IP_ICMP){
         int cIPHead = (pRCV_ip->IP_VerLen & 0x0F) << 2;
         pRCV_icmp = (ICMP_HEADER*)(&CA_Buffer[cETHERNET_HEADER + cIPHead]);
            
         if(pRCV_icmp->ICMP_Type == ICMP_TYPE_RESPONSE){

            cout << "IP:" << dec 
                 << (int)pRCV_ip->IP_IP_S[0] << "." << (int)pRCV_ip->IP_IP_S[1] << "." 
                 << (int)pRCV_ip->IP_IP_S[2] << "." << (int)pRCV_ip->IP_IP_S[3] << endl;   

            cout << "MAC:" << hex 
                 << setfill('0') << setw(2) << (int)pRCV_ethhdr->MAC_S[0] << ":"
                 << setfill('0') << setw(2) << (int)pRCV_ethhdr->MAC_S[1] << ":" 
                 << setfill('0') << setw(2) << (int)pRCV_ethhdr->MAC_S[2] << ":"
                 << setfill('0') << setw(2) << (int)pRCV_ethhdr->MAC_S[3] << ":" 
                 << setfill('0') << setw(2) << (int)pRCV_ethhdr->MAC_S[4] << ":" 
                 << setfill('0') << setw(2) << (int)pRCV_ethhdr->MAC_S[5] << endl;     
         }
      }
   }
}
