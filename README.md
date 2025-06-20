"Firewall practice" 


install npcap SDK and drivers

compile files where pcap is included 


gcc filename.c -IC:YourPATH\Npcap-SDK\Include -LC:YourPATH\Npcap-SDK\Lib -lwpcap -lPacket -lws2_32 -o filename.exe


when executing .exe make sure to copy and paste wpcap.dll and Packet.dll of npcap driver at the located exe file. its located on C:\Windows\System32