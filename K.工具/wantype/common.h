
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <sys/time.h>

#define ETH_10MB        1
#define SEC2USEC        1000000
#define DEFAULT_TIMEOUT 3

enum wantype {
    WANTYPE_UNKNOWN = 0,
    WANTYPE_DHCP = 1,
    WANTYPE_PPPOE = 1 << 1,
};

struct wantype_config {
    char ifname[64];
    struct timeval timeout;
    int show_config;
};

void hwaddr_get(int sockfd, const char *ifname, char *res_hwaddr);

uint16_t checksum(void *addr, int count);

uint32_t random_xid(void);

#endif