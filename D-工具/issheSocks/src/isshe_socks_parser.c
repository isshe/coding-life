
#include "isshe_socks_parser.h"
#include "isshe_config_parser.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void
socks_parser_init(struct socks_parser *parser, struct isshe_socks_config *config)
{
    parser->evbase = NULL;
    parser->evlistener = NULL;
    parser->config = config;
}

void
socks_parser_uninit(struct socks_parser *parser)
{
    evconnlistener_free(parser->evlistener);
    event_base_free(parser->evbase);
}

void
socks_parser_event_new(struct socks_parser *parser)
{
    parser->evbase = event_base_new();
    if (!parser->evbase) {
        fprintf(stderr, "Could not initialize libevent!\n");
        exit(1);
    }
}

// after accept
static void
socks_parser_listerner_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    // 打印对端的信息
    printf("addr:%s, port:%d\n",
    inet_ntoa(((struct sockaddr_in*)sa)->sin_addr),
    ntohs(((struct sockaddr_in*)sa)->sin_port));
}

void
socks_parser_listerner_new_bind(struct socks_parser *parser)
{
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(parser->config->socks_parser_port);

    parser->evlistener = evconnlistener_new_bind(parser->evbase, socks_parser_listerner_cb, (void *)parser,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sin, sizeof(sin));

    if (!parser->evlistener) {
        fprintf(stderr, "Could not create a listener!\n");
        exit(1);
    }
}


int
main(int argc, char *argv[])
{
    struct socks_parser parser;
    struct isshe_socks_config config;

    // config parse
    config_parse(&config);

    // config print
    config_print(&config);

    socks_parser_init(&parser, &config);
    socks_parser_event_new(&parser);
    socks_parser_listerner_new_bind(&parser);
    event_base_dispatch(parser.evbase);
    socks_parser_uninit(&parser);

    printf("done\n");
    return 0;
}