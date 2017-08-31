//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(){
  
   this->set_title("ICMP - ENGINE");
   this->set_resizable(false);
   this->set_size_request(535, 382);
   
   /////////////////////////////////////////////
   // signal
   //CNet.CNRaw.signal_data().connect(sigc::mem_fun(CNet, &C_Net::on_icmp_data));
   
   // Connect the handler to the dispatcher.
   CNet.m_Dispatcher.connect(sigc::mem_fun(CNet, &C_Net::on_data));
   
   /////////////////////////////////////////////
   //Fill the interface combo:
   const vector<S_Net_Interface>* pDA_Interface = CNet.CNInterface.get_pDA_Interface();

   for(auto iface : *pDA_Interface) scombo.interface.append(iface.ps_Name);

   /////////////////////////////////////////////
   
   scombo.interface.set_active(CNet.CNInterface.get_First_Active());
   scombo.interface.set_size_request(84, 30); 
   
   ////////////////
   scombo.type.set_size_request(252, 30);
   scombo.type.append("Request");
   scombo.type.append("Response");
   scombo.type.append("Unreachable");
   scombo.type.append("Redirect");
   scombo.type.append("Router Advertisement");   
   scombo.type.append("Router Solicitation");
   scombo.type.append("Time Exceeded");
   scombo.type.append("Bad IP header");
   scombo.type.append("Timestamp");
   scombo.type.append("Timestamp Reply");
   scombo.type.set_active(0);
   scombo.type.set_sensitive(false);
   
   scombo.type.signal_changed().connect( sigc::mem_fun(*this, &C_App::on_combo_changed) );
   
   ////////////////
   scombo.code.set_size_request(252, 30);
   scombo.code.append("Request");
   scombo.code.set_active(0);
   scombo.code.set_sensitive(false);
   
   ////////////////
   scombo.count.set_size_request(168, 30);
   scombo.count.append("1 Packet");
   scombo.count.append("10 Packets");
   scombo.count.append("100 Packets");
   scombo.count.append("1000 Packets");
   scombo.count.set_active(0);
   scombo.count.set_sensitive(false);
   
   ////////////////
   scombo.sleep.set_size_request(94, 30);
   scombo.sleep.append("1 ms");
   scombo.sleep.append("10 ms");
   scombo.sleep.append("100 ms");
   scombo.sleep.append("1000 ms");
   scombo.sleep.set_active(0);
   scombo.sleep.set_sensitive(false);
   
   //////////////////////////////////////////////////////////////
   // Button
   sbutton.start.set_label("start");
   sbutton.start.set_size_request(84, 30);
   sbutton.start.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_start));
   
   sbutton.stop.set_label("stop");
   sbutton.stop.set_size_request(84, 30);
   sbutton.stop.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_stop));
   sbutton.stop.set_sensitive(false);
   
   sbutton.send.set_label("send");
   sbutton.send.set_size_request(84, 30);
   sbutton.send.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_send));
   sbutton.send.set_sensitive(false);
   
   // CheckButton
   sbutton.data.set_label(" append data");
   sbutton.data.set_size_request(84, 30);
   sbutton.data.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_data));
   sbutton.data.set_sensitive(false);
   
   ////////////////
   // label
   Pango::FontDescription fdsc("Courier New 12"); 
   
   slabel.mac_eth_s.set_label("MAC_ETH_S");
   slabel.mac_eth_s.set_alignment(0.0f, 0.5f);
   slabel.mac_eth_s.set_size_request(234, 30);
   slabel.mac_eth_s.override_font(fdsc);
   ////////////////
   slabel.mac_eth_d.set_label("MAC_ETH_D");
   slabel.mac_eth_d.set_alignment(0.0f, 0.5f);
   slabel.mac_eth_d.set_size_request(234, 30);
   slabel.mac_eth_d.override_font(fdsc);
   ////////////////
   slabel.ip_s.set_label("IP_S");
   slabel.ip_s.set_alignment(0.0f, 0.5f);
   slabel.ip_s.set_size_request(168, 30);
   slabel.ip_s.override_font(fdsc);
   ////////////////
   slabel.ip_d.set_label("IP_D");
   slabel.ip_d.set_alignment(0.0f, 0.5f);
   slabel.ip_d.set_size_request(168, 30);
   slabel.ip_d.override_font(fdsc);
   ////////////////
   slabel.icmp_type.set_label("ICMP_TYPE");
   slabel.icmp_type.set_alignment(0.0f, 0.5f);
   slabel.icmp_type.set_size_request(168, 30);
   slabel.icmp_type.override_font(fdsc);
   ////////////////
   slabel.icmp_code.set_label("ICMP_CODE");
   slabel.icmp_code.set_alignment(0.0f, 0.5f);
   slabel.icmp_code.set_size_request(168, 30);
   slabel.icmp_code.override_font(fdsc);
   
   ///////////////////////////////////////////////////////
   // edit
   EditEthMacS.setSize(3, 30);
   EditEthMacS.setMAC("00","0A","CD","21","5F","A5");
   
   EditEthMacD.setSize(3, 30);
   EditEthMacD.setMAC("00","1C","10","99","FB","A8");
   
   EditIpD.setSize(3, 30);
   EditIpD.setIP("192.168.23.10");

   EditIpS.setSize(3, 30);
   EditIpS.setIP("192.168.23.100");
   
   ///////////////////////////////////////////////////////
   // textview
   tvDataScroll.add(tvData);
   tvDataScroll.set_size_request(515, 70);
   tvDataScroll.set_sensitive(false);
   tvDataScroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

   ///////////////////////////////////////////////////////
   // layout
   m_layout.put(sbutton.start,     94,   0);
   m_layout.put(sbutton.stop,     178,   0);
   m_layout.put(sbutton.send,      10, 255);
   
   m_layout.put(sbutton.data,     385, 258);
   
   m_layout.put(tvDataScroll,      10, 300);
   
   m_layout.put(scombo.interface,  10,   0);
   m_layout.put(scombo.count,      94, 255);
   m_layout.put(scombo.sleep,     272, 255);
   
   m_layout.put(slabel.icmp_type,  10, 180);
   m_layout.put(scombo.type,       10, 210);
   m_layout.put(slabel.icmp_code, 272, 180);
   m_layout.put(scombo.code,      272, 210);
   
   m_layout.put(slabel.ip_s,       10, 105);
   m_layout.put(EditIpS.aEdit[0],  10, 140);
   m_layout.put(EditIpS.aEdit[1],  52, 140);
   m_layout.put(EditIpS.aEdit[2],  94, 140);
   m_layout.put(EditIpS.aEdit[3], 136, 140);
   
   m_layout.put(slabel.ip_d,      272, 105);
   m_layout.put(EditIpD.aEdit[0], 272, 140);
   m_layout.put(EditIpD.aEdit[1], 314, 140);
   m_layout.put(EditIpD.aEdit[2], 356, 140);
   m_layout.put(EditIpD.aEdit[3], 398, 140);

   m_layout.put(slabel.mac_eth_s,      10, 35);
   m_layout.put(EditEthMacS.aEdit[0],  10, 70);
   m_layout.put(EditEthMacS.aEdit[1],  52, 70);
   m_layout.put(EditEthMacS.aEdit[2],  94, 70);
   m_layout.put(EditEthMacS.aEdit[3], 136, 70);
   m_layout.put(EditEthMacS.aEdit[4], 178, 70);
   m_layout.put(EditEthMacS.aEdit[5], 220, 70);
   
   m_layout.put(slabel.mac_eth_d,     272, 35);
   m_layout.put(EditEthMacD.aEdit[0], 272, 70);
   m_layout.put(EditEthMacD.aEdit[1], 314, 70);
   m_layout.put(EditEthMacD.aEdit[2], 356, 70);
   m_layout.put(EditEthMacD.aEdit[3], 398, 70);
   m_layout.put(EditEthMacD.aEdit[4], 440, 70);
   m_layout.put(EditEthMacD.aEdit[5], 482, 70);

   add(m_layout);
   
   ////////////////////////
   
   signal_configure_event().connect(sigc::mem_fun(*this, &C_App::on_configure_changed), false);
   
   ////////////////////////
   
   show_all_children();
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_App::~C_App(){
  
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_start ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_start(){
    
   const S_Net_Interface* pSInterface = 0;
   
   pSInterface = CNet.CNInterface.get_pInterface(scombo.interface.get_active_row_number());
   
   if(pSInterface == nullptr){
      cout << "Error get_pInterface:" << scombo.interface.get_active_row_number() << endl;  
      return; 
   }
   
   if(CNet.start(pSInterface) != C_NET_READY){
      cout << "ERROR: CNet.start" << endl;
      return;
   }
   
   scombo.interface.set_sensitive(false);
   sbutton.start.set_sensitive(false);
   sbutton.stop.set_sensitive(true);
   sbutton.send.set_sensitive(true);
   sbutton.data.set_sensitive(true);
   scombo.type.set_sensitive(true);
   scombo.code.set_sensitive(true);
   scombo.count.set_sensitive(true);
   scombo.sleep.set_sensitive(true);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_stop ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_stop(){
    
   if(CNet.stop() != C_NET_READY){
      cout << "ERROR: CNet.stop" << endl;
      return;
   } 
   
   scombo.interface.set_sensitive(true);
   sbutton.start.set_sensitive(true);
   sbutton.stop.set_sensitive(false);
   sbutton.send.set_sensitive(false);
   sbutton.data.set_sensitive(false);
   scombo.type.set_sensitive(false);
   scombo.code.set_sensitive(false);
   scombo.count.set_sensitive(false);
   scombo.sleep.set_sensitive(false);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_data ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_data(){

   if(sbutton.data.get_active()){
      tvDataScroll.set_sensitive(true); 
   }else{
      tvDataScroll.set_sensitive(false);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_send ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_send(){

   UCHAR IP_D[SIZE_IP];
   UCHAR IP_S[SIZE_IP];

   UCHAR MAC_ETH_D[SIZE_MAC];
   UCHAR MAC_ETH_S[SIZE_MAC];
       
   EditEthMacS.getMAC(MAC_ETH_S);
   EditEthMacD.getMAC(MAC_ETH_D);
   
   EditIpS.getIP(IP_S);
   EditIpD.getIP(IP_D);
   
   ////////////////////////////////
   
   S_IpPacket sIP;
   
   sIP.pETH_MAC_D   = MAC_ETH_D;
   sIP.pETH_MAC_S   = MAC_ETH_S;
   sIP.pIP_D        = IP_D;
   sIP.pIP_S        = IP_S;
   sIP.pIP_Type     = 0x45;
   sIP.pIP_ID       = 0xFFFF;
   sIP.pIP_Frag     = 0x00;
   sIP.pIP_TTL      = 0x40;
   sIP.pIP_Protocol = IP_ICMP;
   
   ////////////////////////////////

   S_IcmpPacket sIcmp;
   
   int code = scombo.code.get_active_row_number();
   int type = scombo.type.get_active_row_number();

   switch(type){
      case 0: sIcmp.ICMP_Type = ICMP_TYPE_REQUEST;
              sIcmp.ICMP_Code = ICMP_CODE_ECHO_REQUEST;
              break;
      case 1: sIcmp.ICMP_Type = ICMP_TYPE_RESPONSE;
              sIcmp.ICMP_Code = ICMP_CODE_ECHO_RESPONSE;
              break;
      case 2: sIcmp.ICMP_Type = ICMP_TYPE_UNREACHABLE;
              switch(code){
                 case  0: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_NET;       break;
                 case  1: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_HOST;      break;
                 case  2: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_PROTO;     break;
                 case  3: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_PORT;      break;
                 case  4: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_FRAG;      break;
                 case  5: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_ROUTE;     break;
                 case  6: sIcmp.ICMP_Code = ICMP_CODE_UNKNOWN_NET;           break;
                 case  7: sIcmp.ICMP_Code = ICMP_CODE_UNKNOWN_HOST;          break;
                 case  8: sIcmp.ICMP_Code = ICMP_CODE_ISOLATED_HOST;         break;
                 case  9: sIcmp.ICMP_Code = ICMP_CODE_PROHIBITED_NET;        break;
                 case 10: sIcmp.ICMP_Code = ICMP_CODE_PROHIBITED_HOST;       break;
                 case 11: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_TOS_NET;   break;
                 case 12: sIcmp.ICMP_Code = ICMP_CODE_UNREACHABLE_TOS_HOST;  break;
                 case 13: sIcmp.ICMP_Code = ICMP_CODE_PROHIBITED;            break;
                 case 14: sIcmp.ICMP_Code = ICMP_CODE_PRECEDENCE_VIOLATION;  break;
                 case 15: sIcmp.ICMP_Code = ICMP_CODE_PRECEDENCE_CUTOFF;     break;
              }
              break;
      case 3: sIcmp.ICMP_Type = ICMP_TYPE_REDIRECT;
              switch(code){
                 case 0: sIcmp.ICMP_Code = ICMP_CODE_REDIRECT_NET;      break;
                 case 1: sIcmp.ICMP_Code = ICMP_CODE_REDIRECT_HOST;     break;
                 case 2: sIcmp.ICMP_Code = ICMP_CODE_REDIRECT_NET_TOS;  break;
                 case 3: sIcmp.ICMP_Code = ICMP_CODE_REDIRECT_HOST_TOS; break;
              }
              break;
      case 4: sIcmp.ICMP_Type = ICMP_TYPE_R_ADVERT;
              sIcmp.ICMP_Code = ICMP_CODE_R_ADVERTISEMENT;
              break;
      case 5: sIcmp.ICMP_Type = ICMP_TYPE_R_SOLICIT;
              sIcmp.ICMP_Code = ICMP_CODE_R_SOLICITATION;
              break;
      case 6: sIcmp.ICMP_Type = ICMP_TYPE_TIME;
              switch(code){
                 case 0: sIcmp.ICMP_Code = ICMP_CODE_TIME_TTL_EXPIRED; break;
                 case 1: sIcmp.ICMP_Code = ICMP_CODE_TIME_FRAGMENT;    break;
              }
              break;
      case 7: sIcmp.ICMP_Type = ICMP_TYPE_BAD_IP_HDR;
              switch(code){
                 case 0: sIcmp.ICMP_Code = ICMP_CODE_BAD_IP_POINTER;        break;
                 case 1: sIcmp.ICMP_Code = ICMP_CODE_BAD_IP_MISSING_OPTION; break;
                 case 2: sIcmp.ICMP_Code = ICMP_CODE_BAD_IP_BAD_LENGTH;     break;
              }
              break;
      case 8: sIcmp.ICMP_Type = ICMP_TYPE_TIMESTAMP;
              sIcmp.ICMP_Code = ICMP_CODE_TIMESTAMP;
              break;
      case 9: sIcmp.ICMP_Type = ICMP_TYPE_TIMESTAMP_REP;
              sIcmp.ICMP_Code = ICMP_CODE_TIMESTAMP_REPLY;
              break;
   }
   
   sIcmp.ICMP_ID        = 0xFFFF;
   sIcmp.ICMP_Sequence  = 0x0001;
   sIcmp.ICMP_TimeStamp = 0x0000;
   
   //////////////////////////////

   int cPackets = 0;
   
   type = scombo.count.get_active_row_number();
   
   switch(type){
      case 0: cPackets = 1;    break; 
      case 1: cPackets = 10;   break;
      case 2: cPackets = 100;  break;
      case 3: cPackets = 1000; break;
   }
   
   int cSleep = 0;
   
   type = scombo.sleep.get_active_row_number();
   
   switch(type){
      case 0: cSleep = 1;    break; 
      case 1: cSleep = 10;   break;
      case 2: cSleep = 100;  break;
      case 3: cSleep = 1000; break;
   }
   
   if(sbutton.data.get_active()){
      CNet.send(&sIP, &sIcmp, cPackets, cSleep, tvData.get_buffer()->get_text().data());
   }else{
      CNet.send(&sIP, &sIcmp, cPackets, cSleep, 0);
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_combo_changed ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_combo_changed(){
    
   scombo.code.remove_all();
   
   int type = scombo.type.get_active_row_number();
   
   switch(type){
      case 0: scombo.code.append("Request");  break;  
      case 1: scombo.code.append("Response"); break; 
      
      case 2: scombo.code.append("Network unreachable");
              scombo.code.append("Host unreachable");
              scombo.code.append("Protocol unreachable");
              scombo.code.append("Port unreachable");
              scombo.code.append("Fragmentation required");
              scombo.code.append("Source route failed");
              scombo.code.append("Network unknown");
              scombo.code.append("Host unknown");
              scombo.code.append("Source Host isolated");
              scombo.code.append("Network prohibited");
              scombo.code.append("Host prohibited");
              scombo.code.append("Network unreachable ToS");
              scombo.code.append("Host unreachable ToS");
              scombo.code.append("Communication prohibited");
              scombo.code.append("Host Precedence Violation");
              scombo.code.append("Precedence cutoff in effect"); 
              break;
              
      case 3: scombo.code.append("Redirect Network");
              scombo.code.append("Redirect Host");
              scombo.code.append("Redirect Network ToS");
              scombo.code.append("Redirect Host ToS");
              break;  
              
      case 4: scombo.code.append("Router Advertisement"); break; 
      case 5: scombo.code.append("Router Solicitation");  break; 
         
      case 6: scombo.code.append("TTL expired");
              scombo.code.append("Reassembly Time exceeded");
              break; 
              
      case 7: scombo.code.append("Pointer indicates the error");
              scombo.code.append("Missing a required Option");
              scombo.code.append("Bad length");
              break; 
              
      case 8: scombo.code.append("Timestamp");       break; 
      case 9: scombo.code.append("Timestamp Reply"); break; 
   }
   
   scombo.code.set_active(0);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_configure_changed ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_configure_changed(GdkEventConfigure* configure_event){
   //m_ScrolledWindow.set_size_request(configure_event->width, configure_event->height - 30);
   return(false);
}
