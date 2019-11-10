#ifndef _ISSHE_SOCKS_COMMON_H_
#define _ISSHE_SOCKS_COMMON_H_

#include <event2/event.h>
#include <event2/listener.h>

#define MAX_OUTPUT (512*1024)       // ???!!!

struct event_base *isshe_socks_event_new();
struct evconnlistener *isshe_socks_listerner_new_bind(
    struct event_base *base, uint16_t port, 
    evconnlistener_cb cb, void *ptr);

void isshe_forward_data_read_cb(
    struct bufferevent *bev, void *ctx);
void isshe_forward_data_event_cb(
    struct bufferevent *bev, short what, void *ctx);

int isshe_domain_name_resolution(
    const char *domain_name, struct addrinfo **res);

void isshe_print_addrinfo(struct addrinfo *addr_info);

#endif