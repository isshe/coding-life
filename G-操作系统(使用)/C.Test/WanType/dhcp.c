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

#include "dhcp.h"

void init_dhcp_header(struct dhcphdr *hdr)
{
    memset(hdr, 0, sizeof(struct dhcphdr));
    hdr->opcode = BOOTREQUEST;
    hdr->hw_type = ETH_10MB;
    hdr->hw_addr_len = ETH_ALEN;
    hdr->cookie = htonl(DHCP_MAGIC);
    hdr->options[0] = DHCP_OPTION_END;
}

int end_option_pos(uint8_t *options)
{
    int i = 0;

    while (options[i] != DHCP_OPTION_END) {
        if (options[i] == DHCP_OPTION_PADDING) {
            i++;
        } else {
            i += options[i + OPTION_LEN] + 2;   // 2: CODE + LENGTH
        }
    }
    return i;
}


void add_dhcp_option(struct dhcphdr *hdr, uint8_t code, uint8_t len, uint8_t *data)
{
    int end_pos = end_option_pos(hdr->options);
    hdr->options[end_pos++] = code;
    hdr->options[end_pos++] = len;
    memcpy(hdr->options + end_pos, data, len);
    end_pos += len;
    hdr->options[end_pos] = DHCP_OPTION_END;
}

void add_dhcp_option_u8(struct dhcphdr *hdr, uint8_t code, uint8_t data)
{
    add_dhcp_option(hdr, code, 1, &data);
}

void add_dhcp_option_u16(struct dhcphdr *hdr, uint8_t code, uint16_t data)
{
    add_dhcp_option(hdr, code, 2, (uint8_t *)&data);
}

void add_dhcp_option_u32(struct dhcphdr *hdr, uint8_t code, uint32_t data)
{
    add_dhcp_option(hdr, code, 4, (uint8_t *)&data);
}

int send_dhcp_discovery(int sockfd, struct wantype_config *cfg)
{
    const int on = 1;
    struct sockaddr_in myaddr;
    struct sockaddr_in servaddr;
    uint8_t buff[16];
    int len = 0;
    struct dhcpopt opt;
    struct dhcp_packet packet;
    struct sockaddr_ll dest;

    // dhcp
    init_dhcp_header(&packet.dhcp);
    packet.dhcp.tid = random_xid();
    packet.dhcp.secs = htons(100);
    hwaddr_get(sockfd, cfg->ifname, packet.dhcp.chw_addr);

    add_dhcp_option_u8(&packet.dhcp, DHCP_OPTION_MESSAGE_TYPE, DHCP_DISCOVER);
    add_dhcp_option(&packet.dhcp, DHCP_OPTION_HOST_NAME, sizeof(HOST_NAME), HOST_NAME);
    add_dhcp_option_u16(&packet.dhcp, DHCP_OPTION_MAX_SIZE, htons(576));
    buff[len++] = ETH_10MB;    // ethernet
    memcpy(buff + len, packet.dhcp.chw_addr, ETH_ALEN);
    len += ETH_ALEN;
    add_dhcp_option(&packet.dhcp, DHCP_OPTION_CLIENT_ID, len, buff);

    // udp && ip
    packet.ip.protocol = IPPROTO_UDP;
    packet.ip.saddr = INADDR_ANY;
    packet.ip.daddr = INADDR_BROADCAST;
    packet.udp.source = htons(CLIENT_PORT);
    packet.udp.dest = htons(SERVER_PORT);
    packet.udp.len = htons(sizeof(struct udphdr) + sizeof(struct dhcphdr));
    packet.ip.tot_len = packet.udp.len;
    packet.udp.check = checksum(&packet, sizeof(struct dhcp_packet));

    packet.ip.tot_len = htons(sizeof(struct dhcp_packet));
    packet.ip.ihl = sizeof(packet.ip) >> 2;
    packet.ip.version = IPVERSION;
    packet.ip.ttl = IPDEFTTL;
    packet.ip.check = checksum(&(packet.ip), sizeof(packet.ip));

    dest.sll_family = AF_PACKET;
    dest.sll_protocol = htons(ETH_P_IP);
    dest.sll_ifindex = if_nametoindex(cfg->ifname);;
    dest.sll_halen = ETH_ALEN;
    memset(dest.sll_addr, 0xff, sizeof(dest.sll_addr));
    if (sendto(sockfd, &packet, sizeof(struct dhcp_packet), 0, (struct sockaddr *) &dest, sizeof(dest)) < 0) {
        printf("sendto error: %s\n", strerror(errno));
        exit(1);
    }

    return 0;
}