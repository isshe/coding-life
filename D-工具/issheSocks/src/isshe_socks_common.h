#ifndef _ISSHE_SOCKS_COMMON_H_
#define _ISSHE_SOCKS_COMMON_H_

#include <event2/event.h>
#include <event2/listener.h>

struct event_base *isshe_socks_event_new();
struct evconnlistener *isshe_socks_listerner_new_bind(
    struct event_base *base, uint16_t port, evconnlistener_cb cb, void *ptr);

#endif