
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
#include <sys/ioctl.h>
#include <sys/select.h>

#include "pppoe.h"

int send_PADI(int sockfd, struct wantype_config *cfg)
{
    struct pppoe_hdr phdr;
    struct pppoe_tag ptag;
    struct sockaddr_ll dest;
    struct pppoe_packet packet;
    int send_len;

    memset(packet.eth.h_dest, 0xff, ETH_ALEN);
    hwaddr_get(sockfd, cfg->ifname, (char *)packet.eth.h_source);
    packet.eth.h_proto = htons(ETH_P_PPP_DISC);

    packet.phdr.type = PPPOE_TYPE;
    packet.phdr.ver = PPPOE_VERSION;
    packet.phdr.code = PADI_CODE;
    packet.phdr.sid = 0;
    packet.phdr.length = htons(sizeof(struct pppoe_tag));

    packet.ptag.tag_type = htons(PPPOE_TAG_TYPE);
    packet.ptag.tag_len = 0;

    dest.sll_family = AF_PACKET;
    dest.sll_halen = ETH_ALEN;
    dest.sll_ifindex = if_nametoindex(cfg->ifname);
    memset(dest.sll_addr, 0xff, sizeof(dest.sll_addr));

    if (sendto(sockfd, &packet, sizeof(packet), 0, (const struct sockaddr *)&dest,sizeof(dest)) < 0) {
        printf("sendto error: %s\n", strerror(errno));
        exit(1);
    }

    return 0;
}