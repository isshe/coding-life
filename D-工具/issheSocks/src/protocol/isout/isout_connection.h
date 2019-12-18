#ifndef _ISSHE_ISOUT_CONNECTION_H_
#define _ISSHE_ISOUT_CONNECTION_H_

#include <stdint.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

typedef struct
{
    int fd;
    int status;
    struct bufferevent *bev;
    struct addrinfo *target_ai;
    struct isshe_socks_opts *opts;
}isout_connection_s;

#endif