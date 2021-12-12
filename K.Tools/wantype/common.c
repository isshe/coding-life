
#include "common.h"

#include <stdint.h>
#include <time.h>   // time()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  // open
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <net/if.h> // if_nametoindex()、ifreq
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_ether.h> // ETH_ALEN

void hwaddr_get(int sockfd, const char *ifname, char *res_hwaddr)
{
    struct ifreq ifr;
    int len;

    memset(&ifr, 0, sizeof(struct ifreq));
    len = strlen(ifname) > sizeof(ifr.ifr_name) - 1 ? sizeof(ifr.ifr_name) - 1 : strlen(ifname);
    memcpy(ifr.ifr_name, ifname, len);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        printf("ioctl error: %s\n", strerror(errno));
        exit(1);
    }

    memcpy(res_hwaddr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
}

uint16_t checksum(void *addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */
    register int32_t sum = 0;
    uint16_t *source = (uint16_t *) addr;

    while (count > 1)  {
        /*  This is the inner loop */
        sum += *source++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if (count > 0) {
        /* Make sure that the left-over byte is added correctly both
         * with little and big endian hosts */
        uint16_t tmp = 0;
        *(unsigned char *) (&tmp) = * (unsigned char *) source;
        sum += tmp;
    }
    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

/* Create a random xid */
uint32_t random_xid(void)
{
    static int initialized;
    if (!initialized) {
        int fd;
        uint32_t seed;

        fd = open("/dev/urandom", 0);
        if (fd < 0 || read(fd, &seed, sizeof(seed)) < 0) {
            printf("Could not load seed from /dev/urandom: %s", strerror(errno));
            seed = time(0);
        }
        if (fd >= 0) close(fd);
        srand(seed);
        initialized++;
    }
    return (uint32_t)rand();
}
