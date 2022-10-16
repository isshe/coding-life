#include <stdint.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

//#include "iconfig.h"
//#include "iuser.h"

typedef struct 
{
    void *in;
    void *out;
    void *config;
    uint64_t flag;
    struct event_base *evbase;
    struct evconnlistener *evlistener;
}isession_s;