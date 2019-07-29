
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
#include <sys/time.h>

#include "dhcp.h"
#include "pppoe.h"
#include "common.h"

void display_help()
{
    printf("Usage: wantype <interface> [OPTION...]\n");
    printf("Examples:\n");
    printf("  wantype eth0\n");
    printf("  wantype eth0 -t 10 -s\n");
    printf("Options:\n");
    printf("  -s            show config\n");
    printf("  -t <timeout>  set timeout\n");
    printf("  -h            help\n");
}

int parse_opt(int argc, char *argv[], struct wantype_config *cfg)
{
    char c;
    int len;
    int res;

    res = 0;
    while((c = getopt(argc, argv, "sht:")) != -1 && !res) {
        switch(c) {
            case 's':
                cfg->show_config = 1;
                break;
            case 'h':
                res = -1;
                break;
            case 't':
                cfg->timeout.tv_sec = atoi(optarg);
                if (cfg->timeout.tv_sec <= 0) {
                    printf("invalid timeout\n");
                    res = -1;
                }
                break;
            case '?':
                res = -1;
                break;
            default:
                break;
        }
    }

    if (optind != argc - 1 || res == -1) {
        display_help();
        res = -1;
    } else {
        len = strlen(argv[optind]);
        if (len > sizeof(cfg->ifname) - 1) {
            printf("Warning: ifname to long!\n");
            len = sizeof(cfg->ifname) - 1;
        }

        memset(cfg->ifname, 0, sizeof(cfg->ifname));
        memcpy(cfg->ifname, argv[optind], len);
    }

    return res;
}


void init_config(struct wantype_config *cfg)
{
    memset(cfg, 0, sizeof(struct wantype_config));
}

void fix_config(struct wantype_config *cfg)
{
    if (cfg->timeout.tv_sec <= 0) {
        cfg->timeout.tv_sec = DEFAULT_TIMEOUT;
    }
}

void display_config(struct wantype_config *cfg)
{
    printf("------config------\n");
    printf("ifname: %s\n", cfg->ifname);
    printf("timeout: %d\n", (int)(cfg->timeout.tv_sec));
    printf("------------------\n");
}

void display_wantype(int wantype)
{
    printf("wantype: %d (", wantype);
    if (wantype == 0) {
        printf("[unknown]");
    }

    if (wantype & WANTYPE_DHCP) {
        printf("[dhcp]");
    }

    if (wantype & WANTYPE_PPPOE) {
        printf("[pppoe]");
    }
    printf(")\n");
}

int main(int argc, char *argv[])
{
    int pppoe_fd;
    int dhcp_fd;
    int maxfd;
    fd_set rset, allset;
    struct timeval st, et;
    long long spend_time;
    struct wantype_config cfg;
    struct timeval timeout;
    int wantype = 0;

    init_config(&cfg);
    if (parse_opt(argc, argv, &cfg) < 0) {
        exit(1);
    }
    fix_config(&cfg);

    if (cfg.show_config) {
        display_config(&cfg);
    }

    pppoe_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_PPP_DISC));
    if (pppoe_fd < 0) {
        printf("socket error: %s\n", strerror(errno));
        exit(1);
    }

    dhcp_fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
    if (dhcp_fd < 0) {
        printf("socket error: %s\n", strerror(errno));
        exit(1);
    }

    send_PADI(pppoe_fd, &cfg);
    send_dhcp_discovery(dhcp_fd, &cfg);

    FD_ZERO(&allset);
    FD_SET(pppoe_fd, &allset);
    FD_SET(dhcp_fd, &allset);

    if (gettimeofday(&st, NULL) < 0 ) {
        printf("gettimeofday error: %s\n", strerror(errno));
        exit(1);
    }

    maxfd = dhcp_fd > pppoe_fd ? dhcp_fd : pppoe_fd;
    maxfd += 1;
    memcpy(&timeout, &cfg.timeout, sizeof(struct timeval));
    do {
        rset = allset;
        if (select(maxfd, &rset, NULL, NULL, &timeout) < 0) {
            printf("select error: %s\n", strerror(errno));
            exit(1);
        }

        if (FD_ISSET(pppoe_fd, &rset)) {
            wantype |= WANTYPE_PPPOE;
            FD_CLR(pppoe_fd, &allset);
            maxfd = dhcp_fd + 1;
        }

        if (FD_ISSET(dhcp_fd, &rset) && is_dhcp_packet(dhcp_fd)) {
            wantype |= WANTYPE_DHCP;
            FD_CLR(dhcp_fd, &allset);
            maxfd = pppoe_fd + 1;
        }

        if (!(FD_ISSET(pppoe_fd, &allset) || FD_ISSET(dhcp_fd, &allset))) {
            break;
        }

        if (gettimeofday(&et, NULL) < 0 ) {
            printf("gettimeofday error: %s\n", strerror(errno));
            exit(1);
        }
        spend_time = (et.tv_sec - st.tv_sec) * SEC2USEC + et.tv_usec - st.tv_usec;
    } while(spend_time < cfg.timeout.tv_sec * SEC2USEC);

    display_wantype(wantype);

    close(pppoe_fd);
    close(dhcp_fd);

    exit(0);
}