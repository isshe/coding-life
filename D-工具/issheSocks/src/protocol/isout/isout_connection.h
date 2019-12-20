#ifndef _ISSHE_ISOUT_CONNECTION_H_
#define _ISSHE_ISOUT_CONNECTION_H_

#include <stdint.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isout_protocol.h"

#define ISOUT_OPTS_FLAG_DNAME       (1 << 0)
#define ISOUT_OPTS_FLAG_IPV4        (1 << 1)
#define ISOUT_OPTS_FLAG_IPV6        (1 << 2)
#define ISOUT_OPTS_FLAG_ADDR_TYPE   (1 << 3)
#define ISOUT_OPTS_FLAG_PORT        (1 << 4)


typedef struct
{
    uint64_t count;     // 计数器，初始化为0
    uint32_t random;    // 随机数, 初始化为0
    uint8_t *dname;     // domain name
    uint8_t *ipv6;      // 初始化为NULL
    uint32_t ipv4;      // 初始化为0
    uint16_t port;      // 初始化为0
    uint8_t addr_type;  // 初始化为0， ISSHE_SOCKS_ADDR_TYPE_DOMAIN
    uint8_t dname_len;  // 初始化为0
    uint8_t ipv6_len;   // 初始化为0
    uint16_t user_data_len;  // 用户数据长度
}isout_conn_opts_s;

typedef struct
{
    int fd;
    int status;
    struct bufferevent *bev;
    struct addrinfo *target_ai;
    uint64_t opts_flag;
    isout_conn_opts_s opts;
}isout_connection_s;


int isout_opts_parse(isout_conn_opts_s *opts, uint8_t *opts_str);

#endif