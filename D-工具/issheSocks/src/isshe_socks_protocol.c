#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>   
#include <stdlib.h> 
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isshe_socks_protocol.h"
#include "isshe_socks_common.h"

void isshe_socks_opt_add_end(uint8_t *buf)
{
    struct isshe_socks_opt opt;
    
    opt.type = ISSHE_SOCKS_OPT_END;
    opt.len = 0;

    memcpy(buf, &opt, sizeof(opt.type) + sizeof(opt.len));
}

void isshe_socks_opt_init(uint8_t *buf)
{
    isshe_socks_opt_add_end(buf);
}

// TODO 不够健壮，只考虑了正常已初始化的情况
int isshe_socks_opt_find(uint8_t *buf, uint8_t type)
{
    int i;
    for (i = 0; *(buf + i) != '\0'; ) {
        if (buf[i] == type) {
            return i;
        } else {
            i = i + buf[i + 1] + 2;     // 2 = sizeof(type) + sizeof(len)
            if (buf[i + 1] == 0) {
                return FAILURE;              // error
            }
        }
    }

    return FAILURE;                          // error
}


int isshe_socks_opt_find_end(uint8_t *buf)
{
    return isshe_socks_opt_find(buf, ISSHE_SOCKS_OPT_END);
}


int isshe_socks_opt_len(uint8_t *buf)
{
    return isshe_socks_opt_find_end(buf) + 2; // type + len;
}


void isshe_socks_opt_add(uint8_t *buf, uint8_t type, uint8_t len, const void *data)
{
    int end_pos = isshe_socks_opt_find_end(buf);
    if (end_pos == -1) {
        printf("find end error!!!\n");
        return ;
    }

    buf = buf + end_pos;
    memcpy(buf, &type, sizeof(type));
    buf += sizeof(type);
    memcpy(buf, &len, sizeof(len));
    if (len && data) {
        buf += sizeof(len);
        memcpy(buf, data, len);
    }

    isshe_socks_opt_add_end(buf);
}

void isshe_socks_opt_parse(uint8_t *buf, struct isshe_socks_opts *opts)
{
    if (!buf) {
        printf("ERROR: buf is NULL!!!\n");
        return ;
    }

    while(buf) {
        switch (*buf)
        {
        case ISSHE_SOCKS_OPT_DOMAIN:
            buf++;
            opts->dname_len = *buf++;
            opts->dname = (uint8_t *)malloc(opts->dname_len);    // TODO: 释放 && 健壮化
            printf("opts->dname = %s\n", opts->dname);
            memcpy(opts->dname, buf, opts->dname_len);
            break;
        case ISSHE_SOCKS_OPT_PORT:
            buf += 2;                               // TODO
            opts->port = ntohs(*buf);
            printf("---isshe---: opts->port = %d\n", opts->port);
            break;
        case ISSHE_SOCKS_OPT_END:
            buf++;
            break;
        default:
            break;
        }

        buf += *buf;
    }
}

struct isshe_socks_opts *
isshe_socks_opts_new()
{
    struct isshe_socks_opts *iso = 
        (struct isshe_socks_opts *)malloc(sizeof(struct isshe_socks_opts));
    if (!iso) {
        printf("malloc isshe_socks_opts failed!\n");
        return iso;
    }

    return iso;
}

void
isshe_socks_opts_free(struct isshe_socks_opts *iso)
{
    // TODO
}


void
isshe_socks_connection_init(struct isshe_socks_connection *isc)
{
    // TODO
    memset(isc, 0, sizeof(struct isshe_socks_connection));
}

struct isshe_socks_connection *
isshe_socks_connection_new()
{
    struct isshe_socks_connection *isc = 
        (struct isshe_socks_connection*)malloc(sizeof(struct isshe_socks_connection));
    if (!isc) {
        printf("malloc isshe_socks_connection failed!\n");
        exit(0);
    }

    isc->opts = isshe_socks_opts_new();

    isshe_socks_connection_init(isc);

    return isc;
}

void
isshe_socks_connection_free(struct isshe_socks_connection *isc)
{
    if (isc) {
        if (isc->opts) {
            isshe_socks_opts_free(isc->opts);
        }

        if (isc->target_ai) {
            freeaddrinfo(isc->target_ai);
            isc->target_ai = NULL;
        }

        free(isc);
    }
}

// TODO 拓展为根据配置使用特定的协议进行连接。（当前是TCP）
// TODO 支持IPV4、IPV6
/* 
 * 连接下一级
 */
struct bufferevent *
isshe_socks_connect_to_next(
    struct event_base *evbase,
	struct sockaddr *ai_addr,
    socklen_t ai_addrlen,
    uint16_t port)
{    
    struct bufferevent *bev_to_user;
    
    bev_to_user = isshe_bufferevent_socket_new(evbase, -1);
    assert(bev_to_user);

    // TODO 这里没用到opts里面的addr，相关部分进行优化
    // TODO 修改这个函数，考虑把这部分提成sockaddr_in *
    struct sockaddr_in *addr;
    addr = (struct sockaddr_in*)ai_addr;
    addr->sin_port = port;

    printf("isshe: connet to: %s(%d):%d\n", 
        inet_ntoa(addr->sin_addr), ai_addrlen, 
        ntohs(addr->sin_port));
        
	//isshe_print_addrinfo(sc->target_ai);

    if (bufferevent_socket_connect(bev_to_user,
		(struct sockaddr*)addr, sizeof(struct sockaddr_in)) < 0) {

		perror("bufferevent_socket_connect");
		bufferevent_free(bev_to_user);
		return NULL;
	}
    
    /*
    int fd_to_user = bufferevent_getfd(bev_to_user);
    // TODO 这里什么意思？搬示例libevent的
    struct linger linger;
    memset(&linger, 0, sizeof(struct linger));
    if (setsockopt(fd_to_user, SOL_SOCKET, SO_LINGER, 
        (const void *)&linger, sizeof(struct linger)) != 0) {
        printf("can not disable linger!\n");
        return FAILURE;
    }
    */

    return bev_to_user;
}