//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// APP
//
// [::Last modi: 28.08.17 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_

 #include <iostream>
 
 using namespace std;
 
 #include <gtkmm/window.h>
 #include <gtkmm/button.h>
 #include <gtkmm/checkbutton.h>
 #include <gtkmm/comboboxtext.h>
 #include <gtkmm/scrolledwindow.h>
 #include <gtkmm/textview.h>
 #include <gtkmm/layout.h>
 
 #include "C_Edit_IP.hpp"
 #include "C_Edit_MAC.hpp"
 
 #include "C_Net.hpp"

//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;
   
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct SButton {
    Gtk::Button start;
    Gtk::Button stop;
    Gtk::Button send;
    Gtk::CheckButton data;
 };
 
 //////////////////////////////
 
 struct SLabel {
    Gtk::Label mac_eth_s;
    Gtk::Label mac_eth_d;
    Gtk::Label ip_s;
    Gtk::Label ip_d;
    Gtk::Label icmp_type;
    Gtk::Label icmp_code;
 };
 
 struct SCombo {
    Gtk::ComboBoxText interface;
    Gtk::ComboBoxText type;
    Gtk::ComboBoxText code;
    Gtk::ComboBoxText count;
    Gtk::ComboBoxText sleep;
 };
 
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_App : public Gtk::Window {
  
    public:
      
       C_App();
       virtual ~C_App();

    protected:

       Gtk::Layout m_layout;

       Gtk::ScrolledWindow tvDataScroll;
       Gtk::TextView tvData;
       
       SButton sbutton;
       SLabel  slabel;
       SCombo  scombo;
       
       C_Edit_MAC EditEthMacS;
       C_Edit_MAC EditEthMacD;
       
       C_Edit_IP EditIpD;
       C_Edit_IP EditIpS;

       ////////////////////////
      
       //Signal handlers:
       void on_button_start();
       void on_button_stop();
       void on_button_send();
       void on_button_data();
       bool on_configure_changed(GdkEventConfigure* configure_event);
       void on_combo_changed();
       
       ////////////////////////
      
       C_Net CNet;
 };

#endif // _C_APP_H_
