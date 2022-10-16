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


struct isshe_proxy_server
{
    struct event_base *evbase;
    struct evconnlistener *evlistener;
    struct isshe_socks_config *config;
};


struct isshe_proxy_server_connection
{
    uint64_t flag;
    //struct isshe_socks_connection *isc;
    struct isshe_socks_connection *to_user_conn;
    struct isshe_socks_connection *from_user_conn;
    struct isshe_proxy_server *ps;
};

void proxy_server_accept_cb(
    struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data);

void proxy_server_from_user_event_cb(
    struct bufferevent *bev, short what, void *ctx);

void proxy_server_from_user_read_cb(
    struct bufferevent *bev, void *ctx);

void proxy_server_to_user_event_cb(
    struct bufferevent *bev, short what, void *ctx);

void proxy_server_to_user_read_cb(
    struct bufferevent *bev, void *ctx);

#endif