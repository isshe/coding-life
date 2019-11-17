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
    struct isshe_socks_connection *isc;
    struct proxy_server *ps;
};

#endif