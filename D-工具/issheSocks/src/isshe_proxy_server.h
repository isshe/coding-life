#ifndef _ISSHE_PROXY_SERVER_H_
#define _ISSHE_PROXY_SERVER_H_

// 标准
#include <stdint.h>

// 第三方
#include <event2/event.h>
#include <event2/listener.h>

// 自定义
#include "isshe_config_parser.h"
#include "isshe_socks_protocol.h"


struct proxy_server
{
    struct event_base *evbase;
    struct evconnlistener *evlistener;
    struct isshe_socks_config *config;
};


struct proxy_server_connection
{
    uint64_t flag;
    //struct isshe_socks_connection *isc;
    struct isshe_socks_connection *to_user_conn;
    struct isshe_socks_connection *from_user_conn;
    struct proxy_server *ps;
};

#endif