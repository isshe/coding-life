#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "isshe_socks_common.h"


struct event_base *
isshe_socks_event_new()
{
    struct event_base *base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        exit(1);
    }
    return base;
}

struct evconnlistener *
isshe_socks_listerner_new_bind(struct event_base *base, 
    uint16_t port, evconnlistener_cb cb, void *ptr)
{
    struct sockaddr_in sin;
    struct evconnlistener *listener;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, cb, ptr,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sin, sizeof(sin));

    if (!listener) {
        fprintf(stderr, "Could not create a listener!\n");
        exit(1);
    }

    return listener;
}