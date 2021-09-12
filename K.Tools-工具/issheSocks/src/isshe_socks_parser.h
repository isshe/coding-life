#ifndef _ISSHE_SOCKS_PARSER_H_
#define _ISSHE_SOCKS_PARSER_H_

// 标准
#include <stdint.h>

// 第三方
#include <event2/event.h>
#include <event2/listener.h>

// 自定义
#include "isshe_config_parser.h"

#define BUFFER_LEN              1500
#define IPV4_ADDR_LEN           4
#define IPV6_ADDR_LEN           16

#define DEFAULT_SOCKS_VERSION   5

#define SOCKS_METHOD_NO_AUTH_REQ        0x00
#define SOCKS_METHOD_GSSAPI             0x01
#define SOCKS_METHOD_USERNAME_PWD       0x02
#define SOCKS_METHOD_NO_SUPPORT_METHOD  0xff

#define SOCKS_CMD_CONNECT           0x01
#define SOCKS_CMD_BIND              0x02
#define SOCKS_CMD_UDP_ASSOCIATE     0x03

#define SOCKS_ADDR_TYPE_IPV4        0x01
#define SOCKS_ADDR_TYPE_DOMAIN      0x03
#define SOCKS_ADDR_TYPE_IPV6        0x04

#define ISSHE_SOCKS_ADDR_TYPE_IPV4      0x01
#define ISSHE_SOCKS_ADDR_TYPE_IPV6      0x02
#define ISSHE_SOCKS_ADDR_TYPE_DOMAIN    0x03

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

// socks5的连接状态
enum socks5_connection_status {
    SCS_UNKNOWN = -1,
    SCS_WAITING_SELECTION_MSG = 0,
    SCS_WAITING_REQUEST = 1,
    SCS_CONNECTED = 2,
};

struct socks_selection_msg {
    uint8_t version;
    uint8_t nmethods;
    uint8_t methods;
};

struct socks_selection_msg_reply {
    uint8_t version;
    uint8_t method;
};

struct socks_request {
    uint8_t version;
    uint8_t cmd;
    uint8_t rsv;
    uint8_t atype;
    uint8_t remain[0];
};

#pragma pack(2)
struct socks_reply {
    uint8_t version;
    uint8_t rep;
    uint8_t rsv;
    uint8_t atype;
    uint32_t addr;
    uint16_t port;
};
#pragma pack()

struct isshe_socks_parser
{
    struct event_base *evbase;
    struct evconnlistener *evlistener;
    struct isshe_socks_config *config;
};

struct isshe_socks_parser_connection
{
    uint64_t flag;
    struct isshe_socks_connection *to_user_conn;
    struct isshe_socks_connection *from_user_conn;
    struct isshe_socks_parser *sp;
    // evtimer: 定时清理状态没有转换的连接
};


#endif