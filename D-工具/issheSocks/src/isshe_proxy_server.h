#ifndef _ISSHE_PROXY_SERVER_H_
#define _ISSHE_PROXY_SERVER_H_

struct proxy_server
{
    struct event_base *evbase;
    struct evconnlistener *evlistener;
    struct isshe_socks_config *config;
};

#endif