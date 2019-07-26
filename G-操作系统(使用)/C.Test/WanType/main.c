
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <net/if.h> // for if_nametoindex
#include <linux/if_ether.h>
#include <linux/if_pppox.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define MAC_LEN 6
#define IFNAME "ens33"
#define BUF_LEN 1024
#define ETHER_TYPE_DISCOVERY ETH_P_PPP_DISC

/*
enum pppoe_code {
    PADI = 0x09,    // PPPoE Active Discovery Initiation
    PADO = 0x07,    // PPPoE Active Discovery Offer
    PADR = 0x19,    // PPPoE Active Discovery Request
    PADS = 0x65,    // PPPoE Active Discovery Session-confirmation
    PADT = 0xa7,    // PPPoE Active Discovery Terminate
};
*/

void hwaddr_get(int sockfd, const char *ifname, char *res_hwaddr)
{
    struct ifreq ifr;
    int len = sizeof(ifname) > sizeof(ifr.ifr_name) ? sizeof(ifr.ifr_name) : sizeof(ifname);
    memcpy(ifr.ifr_name, ifname, len);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        exit(1);
    }

    memcpy(res_hwaddr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
}

int send_PADI(int sockfd)
{
    char buff[BUF_LEN];
    struct ethhdr ehdr;
    struct pppoe_hdr phdr;
    struct pppoe_tag ptag;
    struct sockaddr_ll dst;
    int send_len;

    memset(buff, 0, BUF_LEN);
    memset(ehdr.h_dest, 0xff, MAC_LEN);
    hwaddr_get(sockfd, IFNAME, (char *)ehdr.h_source);
    ehdr.h_proto = htons(ETHER_TYPE_DISCOVERY);

    phdr.type = 1;
    phdr.ver = 1;
    phdr.code = PADI_CODE;
    phdr.sid = 0;
    phdr.length = htons(sizeof(struct pppoe_tag));

    ptag.tag_type = htons(0x0101);
    ptag.tag_len = 0;

    dst.sll_family = AF_PACKET;
    dst.sll_halen = ETH_ALEN;
    dst.sll_ifindex = if_nametoindex(IFNAME);
    memset(dst.sll_addr, 0xff, sizeof(dst.sll_addr));

    memcpy(buff, &ehdr, sizeof(struct ethhdr));
    memcpy(buff + sizeof(struct ethhdr), &phdr, sizeof(struct pppoe_hdr));
    memcpy(buff + sizeof(struct ethhdr) + sizeof(struct pppoe_hdr), &ptag, sizeof(struct pppoe_tag));

    send_len = sizeof(struct ethhdr) + sizeof(struct pppoe_hdr) + sizeof(struct pppoe_tag);
    printf("---isshe---: send_len = %d\n", send_len);
    if (sendto(sockfd, buff, send_len, 0, (const struct sockaddr *)&dst,sizeof(dst)) < 0) {
        printf("sendto error: %s\n", strerror(errno));
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int pppoe_fd;
    int maxfd;
    fd_set rset;
    struct timeval timeout = {1, 0};

    pppoe_fd = socket(AF_PACKET, SOCK_RAW, htons(ETHER_TYPE_DISCOVERY));
    if (pppoe_fd < 0) {
        printf("socket error: %s\n", strerror(errno));
        exit(1);
    }

    send_PADI(pppoe_fd);

    FD_ZERO(&rset);
    while(1) {
        FD_SET(pppoe_fd, &rset);
        maxfd = pppoe_fd + 1;
        if (select(maxfd, &rset, NULL, NULL, &timeout) < 0) {
            printf("select error: %s\n", strerror(errno));
            exit(1);
        }

        if (FD_ISSET(pppoe_fd, &rset)) {
            printf("pppoe\n");
        }
        break;
    }


    exit(0);
}