#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pcap.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <stdlib.h>

void list_interfaces();
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main() {
    const char *dev = "eth0";
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    printf("-- Network interfaces --\n");
    list_interfaces();

    printf("-- Starting packet capture on %s --\n", dev);
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "[-] Could not open device %s: %s\n", dev, errbuf);
        return 2;
    }

    pcap_loop(handle, 10, packet_handler, NULL);
    pcap_close(handle);
    return 0;
}

void list_interfaces() {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];

    printf("[i] Available network interfaces:\n");
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        void *addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
        inet_ntop(AF_INET, addr, ip, sizeof(ip));
        printf("[i] Interface: %s\tIP: %s\n", ifa->ifa_name, ip);
    }

    freeifaddrs(ifaddr);
}

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ip *ip_hdr = (struct ip *)(packet + 14); // Skip Ethernet header
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(ip_hdr->ip_src), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip_hdr->ip_dst), dst_ip, INET_ADDRSTRLEN);

    printf("\n[+] Captured Packet:\n");
    printf("[i] FROM: %s\n", src_ip);
    printf("[i] TO: %s\n", dst_ip);

    if (ip_hdr->ip_p == IPPROTO_TCP) {
        struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + 14 + ip_hdr->ip_hl * 4);
        printf("[i] Protocol: TCP\n");
        printf("[i] SRC Port: %u\n", ntohs(tcp_hdr->th_sport));
        printf("[i] DST Port: %u\n", ntohs(tcp_hdr->th_dport));
    } else if (ip_hdr->ip_p == IPPROTO_UDP) {
        struct udphdr *udp_hdr = (struct udphdr *)(packet + 14 + ip_hdr->ip_hl * 4);
        printf("[i] Protocol: UDP\n");
        printf("[i] SRC Port: %u\n", ntohs(udp_hdr->source));
        printf("[i] DST Port: %u\n", ntohs(udp_hdr->dest));
    } else {
        printf("[i] Other Protocol: %d\n", ip_hdr->ip_p);
    }
}

