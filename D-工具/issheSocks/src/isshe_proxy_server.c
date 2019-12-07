


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

    memset(psc, 0, sizeof(struct proxy_server_connection));
    psc->from_user_conn = isshe_socks_connection_new();
    psc->flag |= ISSHE_SOCKS_FLAG_FROM_USER;
    psc->to_user_conn = isshe_socks_connection_new();
    psc->flag |= ISSHE_SOCKS_FLAG_TO_USER;
    psc->ps = server;

    return psc;
}

void proxy_server_connection_free(struct proxy_server_connection *psc, uint64_t flag)
{
    printf("---proxy_server_connection_free---\n");
    if (psc) {
        if (flag & ISSHE_SOCKS_FLAG_FROM_USER 
            && psc->flag & ISSHE_SOCKS_FLAG_FROM_USER) {
            printf("---free proxy_server_connection_free---free from user\n");
            isshe_socks_connection_free(psc->from_user_conn);
            psc->flag &= ~ISSHE_SOCKS_FLAG_FROM_USER;
            psc->from_user_conn = NULL;
        }
        if (flag & ISSHE_SOCKS_FLAG_TO_USER 
            && psc->flag & ISSHE_SOCKS_FLAG_TO_USER) {
            printf("---free proxy_server_connection_free---free to user\n");
            isshe_socks_connection_free(psc->to_user_conn);
            psc->flag &= ~ISSHE_SOCKS_FLAG_TO_USER;
            psc->to_user_conn = NULL;
        }

        if (!flag) {
            free(psc);
            psc = NULL;
        }
    }
}


void
proxy_server_to_user_read_cb(struct bufferevent *bev, void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
    struct bufferevent *partner = NULL;

    //uint8_t mac[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //struct isshe_socks_opt opt;
	struct evbuffer *src, *dst;
    //uint8_t buf[1024];      // temp
	size_t len;
    
	src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);
    if (psc && psc->from_user_conn && psc->from_user_conn->bev) {
        partner = psc->from_user_conn->bev;
    }
	if (!partner) {
        printf("evbuffer_drain!!!\n");
		evbuffer_drain(src, len);
		return;
	}

	dst = bufferevent_get_output(partner);
    if (!dst) {
        printf("dst == NULL!!!\n");
        return;
    }
    /*
    // 添加消息校验码
    evbuffer_add(dst, mac, sizeof(mac));

    // 添加选项
    isshe_socks_opt_init((uint8_t *)&opt);
    evbuffer_add(dst, &opt, sizeof(opt.type) + sizeof(opt.len));
    */
    // 转发消息
    printf("Debug: %p(%lu) <- %p(%lu)\n", partner, evbuffer_get_length(dst), bev, len);
	evbuffer_add_buffer(dst, src);
}

struct bufferevent *
proxy_server_connect_to_next(struct proxy_server_connection *psc)
{
    struct isshe_socks_connection *isc_from_user = psc->from_user_conn;
    //struct isshe_socks_connection *isc_to_user = psc->to_user_conn;
    struct sockaddr_storage connect_to_addr;
    int connect_to_addrlen;

    memset(&connect_to_addr, 0, sizeof(connect_to_addr));
    connect_to_addrlen = sizeof(connect_to_addr);

    // TODO 根据配置决定下一跳，当前暂为目标服务器
    if (isc_from_user->opts->ipv4) {
        printf("---isc->opts->ipv4---\n");
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
#ifdef EVENT__HAVE_STRUCT_SOCKADDR_IN_SIN_LEN
		sin.sin_len = sizeof(sin);
#endif
		sin.sin_family = AF_INET;
		sin.sin_port = htons(isc_from_user->opts->port);
        sin.sin_addr.s_addr = htonl(isc_from_user->opts->ipv4);
        connect_to_addrlen = sizeof(sin);
		memcpy(&connect_to_addr, &sin, connect_to_addrlen);
    } else if (isc_from_user->opts->ipv6) {
        // TODO
        printf("unsupport ipv6\n");
    } else if (isc_from_user->opts->dname) {
        printf("---isc->opts->dname---\n");
        struct addrinfo *ai = isc_from_user->target_ai;
        while(ai) {
            if (!ai->ai_addr) {
                ai = ai->ai_next;
                continue;
            }
            connect_to_addrlen = ai->ai_addrlen;
		    memcpy(&connect_to_addr, 
                ai->ai_addr, 
                connect_to_addrlen);
            break;
        }

    } else {
        printf("Error: invalid next partner\n");
        return NULL;
    }

    struct bufferevent * bev_to_user = 
        isshe_socks_connect_to_next(psc->ps->evbase,
            (struct sockaddr *)&connect_to_addr, 
            connect_to_addrlen, isc_from_user->opts->port);
    return bev_to_user;
}

void
proxy_server_from_user_read_cb(struct bufferevent *bev, void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
    struct isshe_socks_connection *isc_from_user = psc->from_user_conn;
    struct isshe_socks_connection *isc_to_user = psc->to_user_conn;
    struct bufferevent *partner = NULL;
    struct evbuffer *src, *dst;
    char mac[16] = {0};
    uint8_t buf[128];      // TODO: 暂时写死选项长度，调试代理通过后再修改
	size_t len;

    src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);

    // 连接目标服务器
    if (isc_to_user->status != ISSHE_SCS_ESTABLISHED) {
        // 读取消息验证码，进行消息验证
        bufferevent_read(bev, mac, sizeof(mac));
        //isshe_print_buffer(mac, sizeof(mac), sizeof(mac));

        // TODO check_mac()

        // TODO read_encode_opts()

        // TODO decode_opts()

        // 解析选项，有新的选项，就添加到配置结构中
        bufferevent_read(bev, buf, sizeof(buf));
        //isshe_print_buffer((char *)buf, sizeof(buf), sizeof(buf));
        isshe_socks_opt_parse(buf, sizeof(buf), isc_from_user->opts);

        if (!isc_from_user->opts->ipv4
            && !isc_from_user->opts->ipv6
            && isc_from_user->opts->dname) {
            if (isshe_domain_name_resolution(
                (char *)isc_from_user->opts->dname, 
                &isc_from_user->target_ai) != 0) {
                    printf("域名解析错误！！！\n");
                    return;
                }
        }
        // 连接下一跳
        isc_to_user->bev = proxy_server_connect_to_next(psc);
        if (!isc_to_user->bev) {
            // TODO 关闭、清除连接
            printf("Error: connect to next failed!\n");
            return;
        }

        isc_to_user->status = ISSHE_SCS_ESTABLISHED;
        isc_to_user->fd = bufferevent_getfd(isc_to_user->bev);

        // 设置回调
        bufferevent_setcb(isc_to_user->bev, 
            proxy_server_to_user_read_cb, NULL, 
            proxy_server_to_user_event_cb, (void*)psc);
        bufferevent_enable(isc_to_user->bev, EV_READ|EV_WRITE);
    }
    
    if (isc_to_user && isc_to_user->bev) {
        partner = isc_to_user->bev;
    }
    if (!partner) {
        printf("evbuffer_drain!!!\n");
		evbuffer_drain(src, len);
		return;
	}
    // 读取消息，发送给代理服务器
	dst = bufferevent_get_output(partner);
    printf("Debug: %p(%lu) -> %p(%lu)\n", bev, len, partner, evbuffer_get_length(dst));
	evbuffer_add_buffer(dst, src);
}


void
proxy_server_common_event(
    struct bufferevent *bev, 
    struct bufferevent *partner, 
    bufferevent_data_cb read_cb,
    uint64_t bev_flag, 
    uint64_t partner_flag,
    short what,
    void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
        if (what & BEV_EVENT_ERROR) {
            if (errno) {
                printf("bev = %p, partner = %p\n", bev, partner);
                perror("connection error");
            }
        }

        if (partner) {
            // 把所有数据读出来，发给partner
            read_cb(bev, ctx);
            if (evbuffer_get_length(bufferevent_get_output(partner))) {
                bufferevent_disable(partner, EV_READ);
            } else {
                proxy_server_connection_free(psc, partner_flag);
            }
        }
        
        proxy_server_connection_free(psc, bev_flag);
    }
}

void
proxy_server_from_user_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
    struct bufferevent *partner = NULL;
    if (psc && psc->to_user_conn && psc->to_user_conn->bev) {
        partner = psc->to_user_conn->bev;
    }
    proxy_server_common_event(bev, partner, 
        proxy_server_from_user_read_cb,
        ISSHE_SOCKS_FLAG_FROM_USER, 
        ISSHE_SOCKS_FLAG_TO_USER, what, ctx);
}

void
proxy_server_to_user_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    struct proxy_server_connection *psc = (struct proxy_server_connection *)ctx;
    struct bufferevent *partner = NULL;
    if (psc && psc->from_user_conn && psc->from_user_conn->bev) {
        partner = psc->from_user_conn->bev;
    }
    proxy_server_common_event(bev, partner, 
        proxy_server_to_user_read_cb,
        ISSHE_SOCKS_FLAG_TO_USER,
        ISSHE_SOCKS_FLAG_FROM_USER, what, ctx);
}

void
proxy_server_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    // 打印对端的信息
    printf("\nfd: %d, addr:%s, port:%d\n", fd,
    inet_ntoa(((struct sockaddr_in*)sa)->sin_addr),
    ntohs(((struct sockaddr_in*)sa)->sin_port));

    struct proxy_server *ps = (struct proxy_server *)user_data;
    struct proxy_server_connection *psc = proxy_server_connection_new(ps);

    psc->from_user_conn->bev = isshe_bufferevent_socket_new(ps->evbase, fd);
    assert(psc->from_user_conn->bev);

    bufferevent_setcb(psc->from_user_conn->bev, 
        proxy_server_from_user_read_cb, NULL, 
        proxy_server_from_user_event_cb, (void*)psc);
    bufferevent_enable(psc->from_user_conn->bev, EV_READ|EV_WRITE);
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