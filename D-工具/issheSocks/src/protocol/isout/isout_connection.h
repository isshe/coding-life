#ifndef _ISSHE_ISOUT_CONNECTION_H_
#define _ISSHE_ISOUT_CONNECTION_H_

#include <stdint.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isout_protocol.h"


typedef struct
{
    uint64_t count;     // 计数器，初始化为0
    uint8_t *dname;     // domain name
    uint8_t *ipv6;      // 初始化为NULL
    uint32_t ipv4;      // 初始化为0
    uint16_t port;      // 初始化为0
    uint8_t addr_type;  // 初始化为0， ISSHE_SOCKS_ADDR_TYPE_DOMAIN
    uint8_t dname_len;  // 初始化为0
    uint8_t ipv6_len;   // 初始化为0
    //uint32_t user_data_len;  // 用户数据长度
}isout_conn_opts_s;

typedef struct
{
    int fd;
    int status;
    struct bufferevent *bev;
    struct addrinfo *target_ai;
    //isout_conn_opts_s opts;
    isout_conn_opts_s sent_opts;   // 已发送选项
    isout_conn_opts_s all_opts;    // 包含所有选项
}isout_connection_s;

#endif