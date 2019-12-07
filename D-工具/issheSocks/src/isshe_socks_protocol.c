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
    int i = 0;
    uint8_t cur_type;
    uint8_t cur_len;

    while(TRUE) {
        cur_type = buf[i];
        cur_len = buf[i + 1];
        printf("cur_type = %u, cur_len = %u\n", cur_type, cur_len);
        if (cur_type == type) {
            return i;
        } else if (cur_type == ISSHE_SOCKS_OPT_END) {
            break;
        } else {
            i = i + cur_len + sizeof(cur_len) + sizeof(cur_type);
        }
    }

    return FAILURE;
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
    if (end_pos == FAILURE) {
        printf("find end error!!!\n");
        return ;
    }

    buf += end_pos;
    memcpy(buf, &type, sizeof(type));
    buf += sizeof(type);
    memcpy(buf, &len, sizeof(len));
    buf += sizeof(len);
    if (len && data) {
        memcpy(buf, data, len);
        buf += len;
    }

    isshe_socks_opt_add_end(buf);
}

void isshe_socks_opt_parse(uint8_t *buf, int buflen, struct isshe_socks_opts *opts)
{
    if (!buf) {
        printf("ERROR: buf is NULL!!!\n");
        return ;
    }

    int i = 0;
    uint8_t cur_type;
    uint8_t cur_len;
    uint8_t *cur_data;
    uint8_t end = FALSE;

    while(!end && i < buflen) {
        cur_type = buf[i];
        cur_len = buf[i + sizeof(cur_type)];
        cur_data = buf + i + sizeof(cur_len) + sizeof(cur_type);
        switch (cur_type)
        {
        case ISSHE_SOCKS_OPT_DOMAIN:
            opts->dname_len = cur_len;
            opts->dname = (uint8_t *)malloc(opts->dname_len + 1);    // TODO: 释放 && 健壮化
            memcpy(opts->dname, cur_data, opts->dname_len);
            opts->dname[opts->dname_len] = '\0';
            printf("opts->dname = %s\n", opts->dname);
            break;
        case ISSHE_SOCKS_OPT_PORT:
            memcpy(&opts->port, cur_data, sizeof(opts->port));
            opts->port = ntohs(opts->port);
            printf("---isshe---: opts->port = %u\n", opts->port);
            break;
        case ISSHE_SOCKS_OPT_USER_DATA_LEN:
            memcpy(&opts->user_data_len, cur_data, sizeof(opts->user_data_len));
            //opts->user_data_len = opts->user_data_len;
            printf("---isshe---: opts->user_data_len = %u\n", opts->user_data_len);
            break;
        case ISSHE_SOCKS_OPT_END:
            end = TRUE;
            break;
        default:
            break;
        }
        i += cur_len + sizeof(cur_len) + sizeof(cur_type);
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

    memset(iso, 0, sizeof(struct isshe_socks_opts));

    return iso;
}

void
isshe_socks_opts_free(struct isshe_socks_opts *iso)
{
    // TODO
    if (iso) {
        if (iso->dname) {
            free(iso->dname);
            iso->dname = NULL;
        }
        if (iso->ipv6) {
            free(iso->ipv6);
            iso->ipv6 = NULL;
        }

        free(iso);
        iso = NULL;
    }

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

    memset(isc, 0, sizeof(struct isshe_socks_connection));

    isc->opts = isshe_socks_opts_new();

    return isc;
}

void
isshe_socks_connection_free(struct isshe_socks_connection *isc)
{
    printf("---free isshe_socks_connection_free---\n");
    if (isc) {
        if (isc->opts) {
            isshe_socks_opts_free(isc->opts);
        }

        if (isc->target_ai) {
            freeaddrinfo(isc->target_ai);
            isc->target_ai = NULL;
        }

        if (isc->bev) {
            printf("---free bev: %p\n", isc->bev);
            bufferevent_free(isc->bev);
            isc->bev = NULL;
            printf("---after free: %p\n", isc->bev);
        }

        free(isc);
        //isc = NULL;
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
    addr->sin_port = htons(port);

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