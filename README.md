# ICMP-Engine (alpha)


1. compile:
g++ *.cpp -std=c++11 -o ICMP-Engine -no-pie -Wall \`pkg-config gtkmm-3.0 --cflags --libs\`

2. set right to open raw socket as non root:
setcap 'cap_net_raw+ep' ./ICMP-Engine

![alt tag](https://github.com/bitspalter/ICMP-Engine/blob/master/icmp-engine.png "ICMP-Engine")
