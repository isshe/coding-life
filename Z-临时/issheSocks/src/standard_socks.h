#ifndef _STANDARD_SOCKS_H_
#define _STANDARD_SOCKS_H_

#include <stdint.h>

#define DEFAULT_SOCKS_VERSION 5

#define SOCKS_METHOD_NO_AUTH_REQ        0x00
#define SOCKS_METHOD_GSSAPI             0x01
#define SOCKS_METHOD_USERNAME_PWD       0x02
#define SOCKS_METHOD_NO_SUPPORT_METHOD  0xff

#define SOCKS_CMD_CONNECT           0x01
#define SOCKS_CMD_BIND              0x02
#define SOCKS_CMD_UDP_ASSOCIATE     0x03

#define SOCKS_ADDR_TYPE_IPV4        0x01
#define SOCKS_ADDR_TYPE_DNS         0x03
#define SOCKS_ADDR_TYPE_IPV6        0x04

#define SOCKS_REPLY_SUCCEEDED               0x00
#define SOCKS_REPLY_GENERAL_SVR_FAILURE     0x01
#define SOCKS_REPLY_CONN_NOT_ALLOWED        0x02
#define SOCKS_REPLY_NETWORK_UNREACHABLE     0x03
#define SOCKS_REPLY_HOST_UNREACHABLE        0x04
#define SOCKS_REPLY_CONNECTION_REFUSED      0x05
#define SOCKS_REPLY_TTL_EXPIRED             0x06
#define SOCKS_REPLY_CMD_NOT_SUPPORTED       0x07
#define SOCKS_REPLY_ATYPE_NOT_SUPPORTED     0x08
// 0x09-0xff: unassigned

struct socks_client_selection_msg {
    uint8_t version;
    uint8_t nmethods;
    uint8_t methods;
};

struct socks_server_selection_msg {
    uint8_t version;
    uint8_t method;
};

struct socks_request {
    uint8_t version;
    uint8_t cmd;
    uint8_t rsv;
    uint8_t atype;
    uint32_t addr;
    uint16_t port;
};

struct socks_reply {
    uint8_t version;
    uint8_t rep;
    uint8_t rsv;
    uint8_t atype;
    uint32_t addr;
    uint16_t port;
};

struct socks_domain_request {
    uint8_t version;
    uint8_t cmd;
    uint8_t rsv;
    uint8_t atype;
    uint8_t dn_len;
    uint8_t dn[0];
};

#endif