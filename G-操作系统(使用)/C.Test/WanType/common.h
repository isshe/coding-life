
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <sys/time.h>

#define ETH_10MB        1
#define IFNAME          "ens33"
#define SEC2USEC        1000000
#define DEFAULT_TIMEOUT 3

struct wantype_config {
    char ifname[64];
    struct timeval timeout;
};

void hwaddr_get(int sockfd, const char *ifname, char *res_hwaddr);

uint16_t checksum(void *addr, int count);

uint32_t random_xid(void);

#endif