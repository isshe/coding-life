#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isshe_socks_common.h"


struct event_base *
isshe_socks_event_new()
{
    struct event_base *base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        exit(1);
    }
    return base;
}

struct evconnlistener *
isshe_socks_listerner_new_bind(struct event_base *base, 
    uint16_t port, evconnlistener_cb cb, void *ptr)
{
    // TODO IPV6支持: struct sockaddr_storage
    struct sockaddr_in sin;
    struct evconnlistener *listener;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    listener = evconnlistener_new_bind(base, cb, ptr,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        fprintf(stderr, "Could not create a listener!\n");
        exit(1);
    }

    return listener;
}

void
isshe_forward_data_read_cb(struct bufferevent *bev, void *ctx)
{
	struct bufferevent *partner = ctx;
	struct evbuffer *src, *dst;
	size_t len;

	src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);
	if (!partner) {
		evbuffer_drain(src, len);
		return;
	}
	dst = bufferevent_get_output(partner);
	evbuffer_add_buffer(dst, src);
    printf("Debug: %p(%lu) -> %p(%lu)\n", bev, len, partner, evbuffer_get_length(dst));

	// 水位/速度调节
	//isshe_drained_process(bev, partner, isshe_forward_data_read_cb, 
	//		isshe_drained_write_cb, isshe_forward_data_event_cb, bev);
}


static void
isshe_close_on_finished_write_cb(struct bufferevent *bev, void *ctx)
{
	struct evbuffer *b = bufferevent_get_output(bev);

	if (evbuffer_get_length(b) == 0) {
		bufferevent_free(bev);
	}
}

void
isshe_common_event_cb(struct bufferevent *bev, short what, void *ctx)
{
	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
		if (what & BEV_EVENT_ERROR) {
			if (errno)
				perror("connection error");
		}

		//bufferevent_free(bev);
	}
}

void
isshe_forward_data_event_cb(struct bufferevent *bev, short what, void *ctx)
{
	struct bufferevent *partner = ctx;

	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
		if (what & BEV_EVENT_ERROR) {
			if (errno)
				perror("connection error");
		}

		if (partner) {
			/* Flush all pending data */
			isshe_forward_data_read_cb(bev, ctx);

			if (evbuffer_get_length(bufferevent_get_output(partner))) {
				/* We still have to flush data from the other
				 * side, but when that's done, close the other
				 * side. */
				bufferevent_setcb(partner,
				    NULL, isshe_close_on_finished_write_cb,
				    isshe_forward_data_event_cb, NULL);
				bufferevent_disable(partner, EV_READ);
			} else {
				/* We have nothing left to say to the other
				 * side; close it. */
				//bufferevent_free(partner);
			}
		}

		//bufferevent_free(bev);
	}
}

static int
domain_name_resolution(const char *domain_name, struct addrinfo **res)
{
    struct addrinfo hints;
    printf("domain_name = %s\n", domain_name);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;              // 协议族
    hints.ai_flags = AI_CANONNAME;
    //hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = 0;

    // 记得使用freeaddrinfo进行释放！
    int ret = getaddrinfo(domain_name, NULL, &hints, res);
    if (ret) {
        gai_strerror(ret);
    }
    return ret;
}

int
isshe_domain_name_resolution(const char *domain_name, struct addrinfo **res)
{
    int rc;

    if ((rc = domain_name_resolution(domain_name, res)) != 0) {
        printf("domain name resolution error: %s\n", gai_strerror(rc));
        //exit(1);
    }

    return rc;
}

void
isshe_print_addrinfo(struct addrinfo *ai)
{
    struct sockaddr_in *addr;
    struct addrinfo *info;

    for(info = ai; info != NULL; info = info->ai_next)
    {
        addr = (struct sockaddr_in*)(info->ai_addr);
        printf("ip addresss: %s\n", inet_ntoa(addr->sin_addr));
    }
}

void
isshe_print_buffer(char *buf, int buf_len, int print_len)
{
    size_t n;
    size_t i;

    n = buf_len > print_len ? print_len : buf_len;
    for (i=0; i<n; ++i) {
        printf("%u(%x), ", buf[i], buf[i]);
    }
}

struct bufferevent *
isshe_bufferevent_socket_new(struct event_base *base, evutil_socket_t fd)
{
	struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, 
        BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
    if (!bev) {
        printf("Error: can not new bufferevent socket\n");
    }
	return bev;
}