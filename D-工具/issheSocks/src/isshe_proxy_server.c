


// 初步完成socks_parser后，进行这个的编码；然后再进行隧道客户端服务器的编码

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isshe_config_parser.h"
#include "isshe_proxy_server.h"
#include "isshe_socks_common.h"

void
proxy_server_init(struct proxy_server *server, struct isshe_socks_config *config)
{
    server->evbase = NULL;
    server->evlistener = NULL;
    server->config = config;
}

void
proxy_server_uninit(struct proxy_server *server)
{
    // free
    evconnlistener_free(server->evlistener);
    event_base_free(server->evbase);
}

static void
proxy_server_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    // 打印对端的信息
    printf("\nfd: %d, addr:%s, port:%d\n", fd,
    inet_ntoa(((struct sockaddr_in*)sa)->sin_addr),
    ntohs(((struct sockaddr_in*)sa)->sin_port));

    struct bufferevent *bev_in, *bev_out;
    struct proxy_server *ps = (struct socks_parser *)user_data;

    bev_in = isshe_bufferevent_socket_new(ps->evbase, fd);
    assert(bev_in);

    bufferevent_setcb(bev_in, isshe_forward_data_read_cb, 
        NULL, isshe_forward_data_event_cb, (void*)bev_out);
    bufferevent_enable(bev_in, EV_READ|EV_WRITE);
}

int
main(int argc, char *argv[])
{
    struct isshe_socks_config config;
    struct proxy_server server;

    // config parse
    config_parse(&config);

    // config print
    config_print(&config);

    proxy_server_init(&server, &config);
    server.evbase = isshe_socks_event_new();
    server.evlistener = isshe_socks_listerner_new_bind(server.evbase, 
        config.sp_config->socks_parser_port, proxy_server_accept_cb, (void *)&server);
    event_base_dispatch(server.evbase);
    proxy_server_uninit(&server);

    config_free(&config);

    printf("done\n");
    return 0;
}