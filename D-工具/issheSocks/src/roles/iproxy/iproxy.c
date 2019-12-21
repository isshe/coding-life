
#include "iproxy.h"
#include "isession.h"


void iproxy_left_read_cb(struct bufferevent *bev, void *ctx)
{
    
}


void iproxy_right_read_cb(struct bufferevent *bev, void *ctx)
{

}


void iproxy_left_event_cb(
    struct bufferevent *bev, short what, void *ctx)
{

}

void iproxy_right_event_cb(
    struct bufferevent *bev, short what, void *ctx)
{

}

void iproxy_left_accept_cb(struct evconnlistener *listener, 
    evutil_socket_t fd, struct sockaddr *sa, int socklen, void *ctx)
{
    // TODO xxx = ctx;
    

    struct isshe_proxy_server_connection *psc = proxy_server_connection_new(ps);

    psc->from_user_conn->bev = isshe_bufferevent_socket_new(ps->evbase, fd);
    assert(psc->from_user_conn->bev);

    bufferevent_setcb(psc->from_user_conn->bev, 
        proxy_server_from_user_read_cb, NULL, 
        proxy_server_from_user_event_cb, (void*)psc);
    bufferevent_enable(psc->from_user_conn->bev, EV_READ|EV_WRITE);
}