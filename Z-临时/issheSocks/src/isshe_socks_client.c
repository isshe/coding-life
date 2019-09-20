#include "isshe_socks_client.h"
#include "standard_socks.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#ifndef _WIN32
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static const int PORT = 9995;

struct socks_client *
socks_client_new()
{
    struct socks_client *new_client = (struct socks_client*)malloc(sizeof(struct socks_client));
    if (!new_client) {
        printf("socks client new failed: malloc error!\n");
        exit(1);
    }
    return new_client;
}

void
socks_client_init(struct socks_client *client, 
    struct event_base *base, int proxy_fd, int tunnel_fd)
{
    client->base = base;
    client->proxy_fd = proxy_fd;
    client->tunnel_fd = tunnel_fd;

    // TODO: FIX hardcode
    client->status = SOCKS_CLIENT_STATUS_WAITING_SELECTION_MSG;
}

void
proxy_readcb(struct bufferevent *bev, void *arg)
{
    printf("proxy_readcb\n");
    // 对数据进行读取
    char buf[BUFFER_LEN] = {0};
    size_t n;
    size_t i;
    struct socks_server_selection_msg smsg;
    struct socks_request sreq;
    struct socks_client *client = (struct socks_client*)arg;
    
    while (1) {
        memset(buf, 0, sizeof(buf));
        n = bufferevent_read(bev, buf, sizeof(buf));
        if (n <= 0) {
            break;
        }
        for (i=0; i<n; ++i) {
            printf("%d(%c), ", buf[i], buf[i]);
        }
        printf("\n");
        switch (client->status)
        {
            case SOCKS_CLIENT_STATUS_CONNECTED:
                printf("---isshe---: SOCKS_CLIENT_STATUS_CONNECTED---\n");
                // 发到隧道去
                break;
            case SOCKS_CLIENT_STATUS_WAITING_REQUEST:
                printf("---isshe---: SOCKS_CLIENT_STATUS_WAITING_REQUEST---\n");
                memset(&sreq, 0, sizeof(sreq));
                sreq.version = DEFAULT_SOCKS_VERSION;
                sreq.atype = SOCKS_ADDR_TYPE_IPV4;
                bufferevent_write(bev, &sreq, sizeof(sreq));
                client->status = SOCKS_CLIENT_STATUS_CONNECTED;
                break;
            case SOCKS_CLIENT_STATUS_WAITING_SELECTION_MSG:
                printf("---isshe---: SOCKS_CLIENT_STATUS_WAITING_REQUEST---\n");
                memset(&smsg, 0, sizeof(smsg));
                smsg.version = DEFAULT_SOCKS_VERSION;
                bufferevent_write(bev, &smsg, sizeof(smsg));
                client->status = SOCKS_CLIENT_STATUS_WAITING_REQUEST;
                break;
            default:
                printf("---isshe---: default---\n");
                break;
        }
    }
    // 发送数据给UDP隧道

}

void
proxy_writecb(struct bufferevent *bev, void *arg)
{
    printf("proxy_writecb\n");
}
void
proxy_eventcb(struct bufferevent *bev, short events, void *arg)
{
    printf("proxy_eventcb\n");
}

void udp_tunnel_new()
{

}

// after accept
static void
proxy_listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    printf("proxy_listener_cb\n");
    struct bufferevent *bev;
    struct event_base *base = user_data;
    struct socks_client *new_client;
    // 开一个UDP套接字——隧道客户端
    // UDP示例：https://github.com/eklitzke/event-clock/blob/master/clock.c

    // 设置读取回调，读取回调里面主要是读取，然后把读取的内容加密后，给上面开的套接字发送出去。

    new_client = socks_client_new();
    socks_client_init(new_client, base, fd, -1);

    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, proxy_readcb, proxy_writecb, proxy_eventcb, new_client);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

int
main(int argc, char *argv[])
{
    struct event_base *base;
    struct evconnlistener *proxy_listener;
    struct sockaddr_in sin;

    // parse client config

    // print client config

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    proxy_listener = evconnlistener_new_bind(base, proxy_listener_cb, (void *)base,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!proxy_listener) {
        fprintf(stderr, "Could not create a listener!\n");
        return 1;
    }

    event_base_dispatch(base);

    evconnlistener_free(proxy_listener);
    event_base_free(base);

    printf("done\n");
    return 0;
}