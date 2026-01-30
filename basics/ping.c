#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socker.h>
#include <time.h>

#define BUFFER_SIZE 65536
int main(int argc, char *argv[]){
	int sockfd; 
	unsigned char buffer[BUFFER_SIZE];
	struct sockaddr_in source;
	socklen_t source_len = sizeof(source);
	sockfd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
	if(sockfd < 0){
		perror("[-] Socket creation failed.");
		return 1;
	}
	printf("[+] ICMP ping Monitor");
	printf("[+] Waiting for ping requests");
	while(1){
		int data_size = recvfrom(sockfd,buffer,BUFFER_SIZE,0,(struct sockaddr *)&source,&source_len);
		if(data_size < 0){
			perror("[-] recvfrom error");
			break;
		}
		struct iphdr *ip = (struct iphdr *)buffer;
		struct icmphdr *icmp = (struct icmphdr *)(buffer + (ip->ihl * 4));
		if(icmp->type == ICMP_ECHO){
			time_t now = time(NULL);
			char *time_str = ctime(&now);
			time_str[strlen(time_str) - 1] = '\0';
			struct in_addr src_ip;
			src_ip.s_addr = ip->saddr;
			printf("[!] ping detected\n");
			printf("[i] Time : %s\n",time_str);
			printf("[i] From : %s\n",inet_ntoa(src_ip));
			printf("[i] ICMP type : %d \n",icmp->type);
		}
	}
	close(sockfd);
	return 0;
}
