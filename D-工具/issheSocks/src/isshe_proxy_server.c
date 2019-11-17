


#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isshe_config_parser.h"
#include "isshe_proxy_server.h"
#include "isshe_socks_common.h"
#include "isshe_socks_protocol.h"

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

struct proxy_server_connection *
proxy_server_connection_new(struct proxy_server *server)
{
    struct proxy_server_connection *psc = 
        (struct proxy_server_connection*)malloc(sizeof(struct proxy_server_connection));
    if (!psc) {
        printf("malloc proxy_server_connection failed!\n");
        return psc;
    }

    psc->isc = isshe_socks_connection_new();
    psc->ps = server;

    return psc;
}

void proxy_server_connection_free(struct proxy_server_connection *psc)
{
    if (psc) {
        if (psc->isc) {
            isshe_socks_connection_free(psc->isc);
        }

        free(psc);
    }
}

void
proxy_server_from_user_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    isshe_common_event_cb(bev, what, ctx);
}

void
proxy_server_to_user_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    isshe_common_event_cb(bev, what, ctx);
}


void
proxy_server_to_user_read_cb(struct bufferevent *bev, void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
    struct bufferevent *partner = psc->isc->bev_from_user;
    uint8_t mac[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    struct isshe_socks_opt opt;
	struct evbuffer *src, *dst;
    //uint8_t buf[1024];      // temp
	size_t len;

	src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);
	if (!partner) {
		evbuffer_drain(src, len);
		return;
	}

	dst = bufferevent_get_output(partner);

    // 添加消息校验码
    evbuffer_add(dst, mac, sizeof(mac));

    // 添加选项
    isshe_socks_opt_init((uint8_t *)&opt);
    evbuffer_add(dst, &opt, sizeof(opt.type) + sizeof(opt.len));

    // 转发消息
	evbuffer_add_buffer(dst, src);
    printf("Debug: %p(%lu) -> %p(%lu)\n", bev, len, partner, evbuffer_get_length(dst));
}

void
proxy_server_from_user_read_cb(struct bufferevent *bev, void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
    struct isshe_socks_connection *isc = psc->isc;
    struct bufferevent *partner = isc->bev_to_user;
    struct evbuffer *src, *dst;
    uint8_t mac[16] = {0};
    uint8_t buf[128];      // temp
	size_t len;

    src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);
	if (!partner) {
		evbuffer_drain(src, len);
		return;
	}

    // 读取消息验证码，进行消息验证
    bufferevent_read(bev, mac, sizeof(mac));

    // TODO check_mac()

    // TODO read_encode_opts()

    // TODO decode_opts()

    // 解析选项，有新的选项，就添加到配置结构中
    bufferevent_read(bev, buf, sizeof(buf));
    isshe_socks_opt_parse(buf, isc->opts);

    // TODO 根据配置决定下一跳，当前暂为目标服务器
    // 连接目标服务器
    if (isc->status != ISSHE_SCS_ESTABLISHED) {
        if (!isc->opts->ipv4 
            && !isc->opts->ipv6 
            && isc->opts->dname) {
            isshe_domain_name_resolution(
                (char *)isc->opts->dname, &isc->target_ai);
        }
        isc->bev_to_user = isshe_socks_connect_to_next(
            psc->ps->evbase, psc->isc->target_ai->ai_addr, 
            psc->isc->target_ai->ai_addrlen, psc->isc->opts->port);
        if (isc->bev_to_user) {
            isc->status = ISSHE_SCS_ESTABLISHED;
            isc->fd_to_user = bufferevent_getfd(isc->bev_to_user);
            // 设置回调

            bufferevent_setcb(isc->bev_to_user, 
                proxy_server_to_user_read_cb, NULL, 
                proxy_server_to_user_event_cb, (void*)psc);
            bufferevent_enable(isc->bev_to_user, EV_READ|EV_WRITE);
        }
    }
    
    // 读取消息，发送给代理服务器
	dst = bufferevent_get_output(partner);
	evbuffer_add_buffer(dst, src);
}

static void
proxy_server_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    // 打印对端的信息
    printf("\nfd: %d, addr:%s, port:%d\n", fd,
    inet_ntoa(((struct sockaddr_in*)sa)->sin_addr),
    ntohs(((struct sockaddr_in*)sa)->sin_port));

    struct proxy_server *ps = (struct proxy_server *)user_data;
    struct proxy_server_connection *psc = proxy_server_connection_new(ps);

    psc->isc->bev_from_user = isshe_bufferevent_socket_new(ps->evbase, fd);
    assert(psc->isc->bev_from_user);

    bufferevent_setcb(psc->isc->bev_from_user, proxy_server_from_user_read_cb, 
        NULL, proxy_server_from_user_event_cb, (void*)psc);
    bufferevent_enable(psc->isc->bev_from_user, EV_READ|EV_WRITE);
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
    // TODO 支持TCP、UDP！当前只有TCP。（两个协议可同时开）
    server.evlistener = isshe_socks_listerner_new_bind(server.evbase, 
        config.ps_config->proxy_server_port, proxy_server_accept_cb, (void *)&server);
    event_base_dispatch(server.evbase);
    proxy_server_uninit(&server);

    config_free(&config);

    printf("done\n");
    return 0;
}