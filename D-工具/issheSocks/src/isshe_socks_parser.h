#ifndef _ISSHE_SOCKS_PARSER_H_
#define _ISSHE_SOCKS_PARSER_H_

#include <event2/event.h>
#include <event2/listener.h>

#include "isshe_config_parser.h"

struct socks_parser 
{
    struct event_base *evbase;
    struct evconnlistener *evlistener;
    struct isshe_socks_config *config;
};

#endif