"Firewall practice" 

for windows 
install npcap SDK and drivers

compile files where pcap is included 

gcc filename.cpp -IC:YourPATH\Npcap-SDK\Include -LC:YourPATH\Npcap-SDK\Lib -lwpcap -lPacket -lws2_32 -o filename.exe

for linux : 

sudo apt-get install libpcap-dev

complie file 

gcc filename.cpp -lpcap -o filename.exe 

run using 
sudo ./filename.exe

when executing .exe make sure to copy and paste wpcap.dll and Packet.dll of npcap driver at the located exe file. its located on C:\Windows\System32
