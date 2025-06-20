"Firewall practice" 

compile files where pcap is included
gcc filename.c -IC:YourPATH\Npcap-SDK\Include -LC:YourPATH\Npcap-SDK\Lib -lwpcap -lPacket -lws2_32 -o filename.exe
