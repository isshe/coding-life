#ifndef _ISSHE_IAPP_H_
#define _ISSHE_IAPP_H_

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

//#include "iconfig.h"
//#include "iuser.h"

typedef struct 
{
    void *config;
    struct event_base *evbase;
    struct evconnlistener *evlistener;
}iapp_s;

#endif