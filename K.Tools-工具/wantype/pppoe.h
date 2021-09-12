
#ifndef _PPPOE_H_
#define _PPPOE_H_

#include <linux/if_ether.h>
#include <linux/if_pppox.h>

#include "common.h"

#define PPPOE_TYPE      0x01
#define PPPOE_VERSION   0x01
#define PPPOE_TAG_TYPE  0x0101

struct pppoe_packet {
    struct ethhdr eth;
    struct pppoe_hdr phdr;
    struct pppoe_tag ptag;
};

int send_PADI(int sockfd, struct wantype_config *cfg);

#endif