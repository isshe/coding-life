
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "isshe_socks_parser.h"
#include "isshe_config_parser.h"
#include "isshe_socks_common.h"
#include "isshe_socks_protocol.h"

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


struct socks_parser_connection *
socks_parser_connection_new(int fd)
{
   struct socks_parser_connection *spc = malloc(sizeof(struct socks_parser_connection));
    if (!spc) {
        printf("can not malloc socks connection, return\n");
        return NULL;
    }

    memset(spc, 0, sizeof(struct socks_parser_connection));
    spc->from_user_conn = isshe_socks_connection_new();
    spc->flag |= ISSHE_SOCKS_FLAG_FROM_USER;
    spc->to_user_conn = isshe_socks_connection_new();
    spc->flag |= ISSHE_SOCKS_FLAG_TO_USER;

    spc->from_user_conn->fd = fd;
    spc->from_user_conn->status = SCS_WAITING_SELECTION_MSG;

    return spc;
}


void
socks_parser_connection_free(struct socks_parser_connection *spc, uint64_t flag)
{
    printf("---free socks_parser_connection_free---\n");
    if (spc) {
        if (flag & ISSHE_SOCKS_FLAG_FROM_USER
            && spc->flag & ISSHE_SOCKS_FLAG_FROM_USER) {
            printf("---free socks_parser_connection_free---free from user\n");
            isshe_socks_connection_free(spc->from_user_conn);
            spc->flag &= ~ISSHE_SOCKS_FLAG_FROM_USER;
            spc->from_user_conn = NULL;
        }
        if (flag & ISSHE_SOCKS_FLAG_TO_USER 
            && spc->flag & ISSHE_SOCKS_FLAG_TO_USER) {
            printf("---free socks_parser_connection_free---free to user\n");
            isshe_socks_connection_free(spc->to_user_conn);
            spc->flag &= ~ISSHE_SOCKS_FLAG_TO_USER;
            spc->to_user_conn = NULL;
        }

        if (!flag) {
            free(spc);
            spc = NULL;
        }
    }
}

int is_socks_selection_msg(struct socks_selection_msg *msg)
{
    // TODO
    return 1;
}

int is_socks_request(struct socks_request *req)
{
    // TODO
    return 1;
}

void 
read_and_drop_all(struct bufferevent *bev, struct evbuffer *evb)
{
    char temp;
    while(evbuffer_get_length(evb)) {
        //printf("len: %d\n", evbuffer_get_length(evb));
        bufferevent_read(bev, &temp, sizeof(temp));
    }
}

int scs_selction_msg_process(struct bufferevent *bev)
{
    printf("---isshe---: scs_selction_msg_process---\n");
    struct evbuffer *inevb = bufferevent_get_input(bev);
    int len = evbuffer_get_length(inevb);
    struct socks_selection_msg msg;
    printf("len = %d, sizeof(msg) = %lu\n", len, sizeof(msg));
    if (len < sizeof(msg)) {
        return 0;
    }

    bufferevent_read(bev, &msg, sizeof(msg));
    if (!is_socks_selection_msg(&msg)) {
        printf("scs_selction_msg_process: 有问题\n");
        return 0;
    }

    // 剩下的全部读出来
    read_and_drop_all(bev, inevb);

    struct socks_selection_msg_reply reply;
    memset(&reply, 0, sizeof(reply));
    reply.version = DEFAULT_SOCKS_VERSION;
    bufferevent_write(bev, &reply, sizeof(reply));

    return 1;
}

/*
 * 处理socks5命令
 */
void
connect_cmd_process(struct socks_parser_connection *spc, 
    struct bufferevent *bev, struct socks_request *request)
{
    struct isshe_socks_connection *isc_from_user = spc->from_user_conn;
    switch (request->atype)
    {
        case SOCKS_ADDR_TYPE_DOMAIN:
            isc_from_user->opts->addr_type = ISSHE_SOCKS_ADDR_TYPE_DOMAIN;
            bufferevent_read(bev, &isc_from_user->opts->dname_len, 
                sizeof(isc_from_user->opts->dname_len));
            break;
        case SOCKS_ADDR_TYPE_IPV4:
            isc_from_user->opts->addr_type = ISSHE_SOCKS_ADDR_TYPE_IPV4;
            isc_from_user->opts->dname_len = IPV4_ADDR_LEN;
            break;
        case SOCKS_ADDR_TYPE_IPV6:
            isc_from_user->opts->addr_type = ISSHE_SOCKS_ADDR_TYPE_IPV6;
            isc_from_user->opts->dname_len = IPV6_ADDR_LEN;
            break;
        default:
            break;
    }

    // 复制到target
    int len = evbuffer_get_length(bufferevent_get_input(bev));
    if (isc_from_user->opts->dname_len + sizeof(isc_from_user->opts->port) > len) {
        printf("Warning: expect len %d, get len %d\n", isc_from_user->opts->dname_len, len);
        isc_from_user->opts->dname_len = len - sizeof(isc_from_user->opts->port);
    }

    isc_from_user->opts->dname = (uint8_t *)malloc(isc_from_user->opts->dname_len + 1);
    if (!isc_from_user->opts->dname) {
        printf("Error: malloc failed!\n");
        // TODO：释放内存，关闭连接
    }
    bufferevent_read(bev, isc_from_user->opts->dname, isc_from_user->opts->dname_len);
    isc_from_user->opts->dname[(int)isc_from_user->opts->dname_len] = '\0';
    printf("Debug: target: %s\n", isc_from_user->opts->dname);

    bufferevent_read(bev, &isc_from_user->opts->port, sizeof(isc_from_user->opts->port));
}

int scs_request_process(struct socks_parser_connection *sc, struct bufferevent *bev)
{
    printf("---isshe---: scs_request_process---\n");
    int len = evbuffer_get_length(bufferevent_get_input(bev));
    struct socks_request request;
    struct socks_reply reply;
    printf("len = %d, sizeof(request) = %lu\n", len, sizeof(request));
    if (len < sizeof(request)) {
        return 0;
    }

    bufferevent_read(bev, &request, sizeof(request));
    if (!is_socks_request(&request)) {
        printf("scs_request_process: 有问题\n");
        return 1;
    }

    switch (request.cmd) {
        case SOCKS_CMD_CONNECT:
            connect_cmd_process(sc, bev, &request);
            break;
        default:
            break;
    }

    // 还有剩，就丢了吧
    read_and_drop_all(bev, bufferevent_get_input(bev));

    memset(&reply, 0, sizeof(reply));
    reply.version = DEFAULT_SOCKS_VERSION;
    reply.atype = SOCKS_ADDR_TYPE_IPV4;
    //reply.port = htons(1080);
    // TODO 由于对齐，发送回去的信息多了一些字节(4字节对齐时：原本10字节，现在12字节)
    //bufferevent_write(bev, &reply, sizeof(reply) - 2);
    bufferevent_write(bev, &reply, sizeof(reply));
    return 1;
}

void
socks_parser_from_user_read_cb(struct bufferevent *bev, void *ctx)
{
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
    struct isshe_socks_connection *isc_from_user = spc->from_user_conn;
    struct isshe_socks_connection *isc_to_user = spc->to_user_conn;
    uint8_t mac[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //struct isshe_socks_opt opt;
    struct bufferevent *partner = NULL; //isc_to_user->bev;
	struct evbuffer *src, *dst;
	size_t len;
    uint8_t buf[128];      // temp

	src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);
    if (isc_to_user && isc_to_user->bev) {
        partner = isc_to_user->bev;
    }
	if (!partner) {
		evbuffer_drain(src, len);
		return;
	}

	dst = bufferevent_get_output(partner);
    if (isc_to_user->status != ISSHE_SCS_ESTABLISHED) {
        // 添加消息校验码
        evbuffer_add(dst, mac, sizeof(mac));

        // TODO 不需要每次都加选项，优化：去掉后续不需要的选项
        // 添加选项
        memset(buf, 0, sizeof(buf));    // TODO 是否需要&优化
        isshe_socks_opt_init(buf);
        isshe_socks_opt_add(buf, ISSHE_SOCKS_OPT_DOMAIN, 
            isc_from_user->opts->dname_len, isc_from_user->opts->dname);
        isshe_socks_opt_add(buf, ISSHE_SOCKS_OPT_PORT, 
            sizeof(isc_from_user->opts->port), &(isc_from_user->opts->port));
        isshe_socks_opt_add(buf, ISSHE_SOCKS_OPT_USER_DATA_LEN, sizeof(len), &len);
        evbuffer_add(dst, buf, sizeof(buf));   // TODO 这里固定128字节了
        
        isc_to_user->status = ISSHE_SCS_ESTABLISHED;
    }
    printf("Debug: %p(%lu) -> %p(%lu)\n", bev, len, partner, evbuffer_get_length(dst));
    evbuffer_add_buffer(dst, src);
}

void
socks_parser_to_user_read_cb(struct bufferevent *bev, void *ctx)
{
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
    struct isshe_socks_connection *isc_from_user = spc->from_user_conn;
    struct bufferevent *partner = NULL;
	struct evbuffer *src, *dst;
	size_t len;

	src = bufferevent_get_input(bev);
	len = evbuffer_get_length(src);
    if (isc_from_user && isc_from_user->bev) {
        partner = isc_from_user->bev;
    }
	if (!partner) {
        printf("evbuffer_drain!!!\n");
		evbuffer_drain(src, len);
		return;
	}
    /*
    // 读取消息校验码
    bufferevent_read(bev, mac, sizeof(mac));
    isshe_print_buffer(mac, sizeof(mac), sizeof(mac));
    printf("\n---------isshe----------\n");
    // 读取选项
    bufferevent_read(bev, &opt, sizeof(opt.type) + sizeof(opt.len));
    */
    printf("Debug: bev = %p, partner = %p\n", bev, partner);
	dst = bufferevent_get_output(partner);
    printf("Debug: %p(%lu) <- %p(%lu)\n", partner, evbuffer_get_length(dst), bev, len);
	evbuffer_add_buffer(dst, src);
}


static void
socks_data_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    printf("-----socks_data_event_cb------");
    /*
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
    isshe_forward_data_event_cb(bev, what, spc->from_user_conn->bev);
	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
        printf("INFO: socks_parser_connection_free...\n");
        socks_parser_connection_free(spc);
	}
    */
}

void
socks_parser_common_event(
    struct bufferevent *bev, 
    struct bufferevent *partner, 
    bufferevent_data_cb read_cb,
    uint64_t bev_flag, 
    uint64_t partner_flag,
    short what,
    void *ctx)
{
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
        if (what & BEV_EVENT_ERROR) {
            if (errno) {
                perror("connection error");
            }
        }

        if (partner) {
            // 把所有数据读出来，发给partner
            read_cb(bev, ctx);
            if (evbuffer_get_length(bufferevent_get_output(partner))) {
                bufferevent_disable(partner, EV_READ);
            } else {
                socks_parser_connection_free(spc, partner_flag);
            }
        }

        socks_parser_connection_free(spc, bev_flag);
    }
}


void
socks_parser_from_user_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
    struct bufferevent *partner = NULL;
    if (spc->to_user_conn && spc->to_user_conn->bev) {
        partner = spc->to_user_conn->bev;
    }
    socks_parser_common_event(bev, partner, 
        socks_parser_from_user_read_cb,
        ISSHE_SOCKS_FLAG_FROM_USER, 
        ISSHE_SOCKS_FLAG_TO_USER, what, ctx);
}

void
socks_parser_to_user_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
    struct bufferevent *partner = NULL;
    if (spc->from_user_conn && spc->from_user_conn->bev) {
        partner = spc->from_user_conn->bev;
    }
    socks_parser_common_event(bev, partner, 
        socks_parser_to_user_read_cb,
        ISSHE_SOCKS_FLAG_TO_USER, 
        ISSHE_SOCKS_FLAG_FROM_USER, what, ctx);
}

static void
socks_data_read_cb(struct bufferevent *bev, void *ctx)
{
    struct socks_parser_connection *spc = (struct socks_parser_connection *)ctx;
    struct isshe_socks_connection *isc_from_user = spc->from_user_conn;
    struct isshe_socks_connection *isc_to_user = spc->to_user_conn;

    printf("fd: %d, ", isc_from_user->fd);
    // TODO: 这个while的内容，放到各个case里面：读取并转发/回复
    //while (evbuffer_get_length(bufferevent_get_input(bev))) {
    // 这里的握手过程还需再捋一下: 
    // 为什么到了域名解析后，就完了？
    // 是怎么知道request完成了的？
    switch(isc_from_user->status) {
        case SCS_WAITING_SELECTION_MSG:
            if (scs_selction_msg_process(bev)) {
                isc_from_user->status = SCS_WAITING_REQUEST;
            }
            break;
        case SCS_WAITING_REQUEST:
            if (scs_request_process(spc, bev)) {
                isc_from_user->status = SCS_CONNECTED;
            }
            break;
        case SCS_CONNECTED:
            // TODO: 删除握手定时器
            // 转发数据
            if (!isc_to_user->bev) {
                //connect_to_next(sc);
                struct sockaddr_in sin;

                memset(&sin, 0, sizeof(sin));
                sin.sin_family = AF_INET;
                sin.sin_addr.s_addr = htonl(INADDR_ANY);    // TODO 改成proxy的地址
                sin.sin_port = htons(spc->sp->config->ps_config->proxy_server_port);
                isc_to_user->bev = isshe_socks_connect_to_next(
                    spc->sp->evbase, (struct sockaddr*)&sin,
                    sizeof(sin), ntohs(sin.sin_port));  // TODO 这两个port的设置/使用
                if (!isc_to_user->bev) {
                    printf("connect to next failed!\n");
                    // TODO 释放连接等
                    exit(1);
                }
            }
            socks_parser_from_user_read_cb(bev, spc);
            // 转发完本次的数据后，设置为直接转发数据的函数，不需再进行协议解析
		    bufferevent_setcb(bev, socks_parser_from_user_read_cb, 
			    NULL, socks_parser_from_user_event_cb, spc);

            bufferevent_setcb(isc_to_user->bev, socks_parser_to_user_read_cb, 
                NULL, socks_parser_to_user_event_cb, (void*)spc);
            bufferevent_enable(isc_to_user->bev, EV_READ|EV_WRITE);
            break;
        default:
            break;
    }
}

// after accept
static void
socks_parser_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    struct bufferevent *bev;
    // 打印对端的信息
    printf("\nfd: %d, addr:%s, port:%d\n", fd,
    inet_ntoa(((struct sockaddr_in*)sa)->sin_addr),
    ntohs(((struct sockaddr_in*)sa)->sin_port));
            
    struct socks_parser_connection *spc = socks_parser_connection_new(fd);
    if (!spc) {
        return;
    }

    struct socks_parser *parser = (struct socks_parser *)user_data;
    bev = isshe_bufferevent_socket_new(parser->evbase, fd);

    assert(bev);

    spc->from_user_conn->bev = bev;
    spc->sp = parser;

    bufferevent_setcb(bev, socks_data_read_cb, 
        NULL, socks_data_event_cb, (void*)spc);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}

void
socks_parser_free(struct socks_parser *parser, struct isshe_socks_config *config)
{
    // TODO
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
    parser.evbase = isshe_socks_event_new();
    parser.evlistener = isshe_socks_listerner_new_bind(parser.evbase, 
        config.sp_config->socks_parser_port, 
        socks_parser_accept_cb, (void *)&parser);
    event_base_dispatch(parser.evbase);
    socks_parser_uninit(&parser);

    config_free(&config);
    socks_parser_free(&parser, &config);

    printf("done\n");
    return 0;
}