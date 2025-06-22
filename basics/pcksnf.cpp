#include <pcap.h>
#include <stdio.h>

void packet_hdl(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){
    printf("Packet captured, Length: %d\n",header->len);
}

int main(){
    char errbuf[PCAP_ERRBUF_SIZE]; 
    pcap_t *handle; 
    handle = pcap_open_live("eth0",65535,1,1000,errbuf); 
    if(!handle){
        fprintf(stderr,"Could not open device: %s \n",errbuf) ;
        return 1 ;
    }
    struct bpf_program fp ; 
    char filter_exp[] = "tcp port 80"; 
    if (pcap_compile(handle,&fp,filter_exp,0,PCAP_NETMASK_UNKNOWN) == -1){
        fprintf(stderr,"Bad filter: %s\n",pcap_geterr(handle)); 
        return 1; 
    }
    pcap_setfilter(handle,&fp); 
    pcap_loop(handle,10,packet_hdl,NULL); 
    pcap_freecode(&fp); 
    pcap_close(handle); 
    return 0 ; 
}