#include <pcap.h>
#include <stdio.h>

int main(){
    pcap_if_t *alldevs; 
    char errbuf[PCAP_ERRBUF_SIZE]; 
    if (pcap_findalldevs(&alldevs,errbuf) == -1){
        fprintf(stderr,"Error finding devices: %s\n", errbuf); 
        return 1; 
    }
    printf("Available Devices: \n"); 
    for (pcap_if_t *dev = alldevs; dev; dev= dev->next){
        printf("- %s",dev->name); 
        if(dev->description) printf(" (%s)",dev->description); 
        printf("\n"); 
    }
    pcap_freealldevs(alldevs); 
    return 0 ;
}