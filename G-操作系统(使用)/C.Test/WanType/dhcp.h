#ifndef _DHCP_H_
#define _DHCP_H_

#include <linux/if_ether.h>
#include <stdint.h>
#include <linux/ip.h>
#include <linux/udp.h>

#include "common.h"

#define HOST_NAME       "isshe"
#define DHCP_MAGIC      0x63825363
#define BOOTREQUEST     1
#define BOOTREPLY       2
#define OPTION_CODE     0
#define OPTION_LEN      1
#define OPTION_DATA     2
#define SERVER_PORT     67
#define CLIENT_PORT     68

enum dhcp_message_type {
    INIT_SELECTING = 0,
    DHCP_DISCOVER = 1,
    DHCP_OFFER = 2,
    DHCP_REQUEST = 3,
    DHCP_DECLINE = 4,
    DHCP_ACK = 5,
    DHCP_NAK = 6,
    DHCP_RELEASE = 7,
    DHCP_INFORM = 8,
};

enum dhcp_option_code {
    DHCP_OPTION_PADDING = 0x00,
    DHCP_OPTION_SUBNET = 0x01,
    DHCP_OPTION_TIME_OFFSET = 0x02,
    DHCP_OPTION_ROUTER = 0x03,
    DHCP_OPTION_TIME_SERVER = 0x04,
    DHCP_OPTION_NAME_SERVER = 0x05,
    DHCP_OPTION_DNS_SERVER = 0x06,
    DHCP_OPTION_LOG_SERVER = 0x07,
    DHCP_OPTION_COOKIE_SERVER = 0x08,
    DHCP_OPTION_LPR_SERVER = 0x09,
    DHCP_OPTION_HOST_NAME = 0x0c,
    DHCP_OPTION_BOOT_SIZE = 0x0d,
    DHCP_OPTION_DOMAIN_NAME = 0x0f,
    DHCP_OPTION_SWAP_SERVER = 0x10,
    DHCP_OPTION_ROOT_PATH = 0x11,
    DHCP_OPTION_IP_TTL = 0x17,
    DHCP_OPTION_MTU = 0x1a,
    DHCP_OPTION_BROADCAST = 0x1c,
    DHCP_OPTION_NTP_SERVER = 0x2a,
    DHCP_OPTION_WINS_SERVER = 0x2c,
    DHCP_OPTION_REQUESTED_IP = 0x32,
    DHCP_OPTION_LEASE_TIME = 0x33,
    DHCP_OPTION_OPTION_OVER = 0x34,
    DHCP_OPTION_MESSAGE_TYPE = 0x35,
    DHCP_OPTION_SERVER_ID = 0x36,
    DHCP_OPTION_PARAM_REQ = 0x37,
    DHCP_OPTION_MESSAGE = 0x38,
    DHCP_OPTION_MAX_SIZE = 0x39,
    DHCP_OPTION_T1 = 0x3a,
    DHCP_OPTION_T2 = 0x3b,
    DHCP_OPTION_VENDOR = 0x3c,
    DHCP_OPTION_CLIENT_ID = 0x3d,
    DHCP_OPTION_END = 0xff,
};

struct dhcphdr {
    uint8_t    opcode;         // operation code
    uint8_t    hw_type;        // hardware type
    uint8_t    hw_addr_len;    // hardware address length
    uint8_t    hops;           // hop count
    uint32_t  tid;            // transaction ID
    uint16_t  secs;           // number of seconds
    uint16_t  flags;          // flags
    uint32_t  ciaddr;         // client IP address
    uint32_t  yiaddr;         // your IP address
    uint32_t  siaddr;         // server IP address
    uint32_t  giaddr;         // gateway IP address
    uint8_t    chw_addr[16];   // client hardware address
    uint8_t    shname[64];     // server host name
    uint8_t    bfname[128];    // boot filename
    uint32_t   cookie;        // magic cookie in option
    uint8_t    options[308];     // dhcp options
};

struct dhcp_packet {
    struct iphdr ip;
    struct udphdr udp;
    struct dhcphdr dhcp;
};

struct dhcpopt {
    uint8_t option;
    uint8_t length;
    uint8_t data[0];
};

int send_dhcp_discovery(int sockfd, struct wantype_config *cfg);

#endif

