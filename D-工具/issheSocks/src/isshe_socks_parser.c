
#include "isshe_socks_parser.h"
#include "isshe_config_parser.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

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

static void print_buffer(char *buf, int buf_len, int print_len)
{
    size_t n;
    size_t i;

    n = buf_len > print_len ? print_len : buf_len;
    for (i=0; i<n; ++i) {
        printf("%d(%x), ", buf[i], buf[i]);
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

int scs_selction_msg_process(struct bufferevent *bev)
{
    printf("---isshe---: scs_selction_msg_process---\n");
    int len = evbuffer_get_length(bufferevent_get_input(bev));
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

    // TODO: 剩下的全部读出来

    struct socks_selection_msg_reply reply;
    memset(&reply, 0, sizeof(reply));
    reply.version = DEFAULT_SOCKS_VERSION;
    bufferevent_write(bev, &reply, sizeof(reply));

    return 1;
}

int scs_request_process(struct bufferevent *bev)
{
    printf("---isshe---: scs_request_process---\n");
    int len = evbuffer_get_length(bufferevent_get_input(bev));
    struct socks_request request;
    struct socks_reply reply;
    printf("len = %d, sizeof(msg) = %lu\n", len, sizeof(request));
    if (len < sizeof(request)) {
        return 0;
    }

    bufferevent_read(bev, &request, sizeof(request));
    if (!is_socks_request(&request)) {
        printf("scs_request_process: 有问题\n");
        return 1;
    }
    // TODO: 剩下的全部读出来
    /*
    switch (request.cmd) {

    }
    */
    memset(&reply, 0, sizeof(reply));
    reply.version = DEFAULT_SOCKS_VERSION;
    reply.atype = SOCKS_ADDR_TYPE_IPV4;
    bufferevent_write(bev, &reply, sizeof(reply));
    return 1;
}

void temp_receive_and_print_buf(struct bufferevent *bev)
{
    char buf[BUFFER_LEN] = {0};
    size_t n;
    memset(buf, 0, sizeof(buf));
    n = bufferevent_read(bev, buf, sizeof(buf));
    if (n <= 0) {
        return;
    }
    print_buffer(buf, n, 10);
}

void
socks_data_read_process(struct bufferevent *bev, void *ctx)
{
    struct socks_connection *sc = (struct socks_connection *)ctx;
    printf("fd: %d, ", sc->fd);
    // TODO: 这个while的内容，放到各个case里面：读取并转发/回复
    while (evbuffer_get_length(bufferevent_get_input(bev))) {
        // 这里的握手过程还需再捋一下: 
        // 为什么到了域名解析后，就完了？
        // 是怎么知道request完成了的？
		switch(sc->status) {
            case SCS_WAITING_SELECTION_MSG:
                if (scs_selction_msg_process(bev)) {
                    sc->status = SCS_WAITING_REQUEST;
                }
                break;
            case SCS_WAITING_REQUEST:
                if (scs_request_process(bev)) {
                    sc->status = SCS_CONNECTED;
                }
                break;
            case SCS_CONNECTED:
                // TODO: 删除握手定时器
                // 转发数据
                temp_receive_and_print_buf(bev);
                printf("\n");
                break;
            default:
                break;
        }
	}
}

void
socks_data_event_process(struct bufferevent *bev, short what, void *ctx)
{
    if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
        if (what & BEV_EVENT_ERROR) {
            printf("socks_data_event_process: error!\n");
        }

        bufferevent_free(bev);
    }
}

struct socks_connection *socks_connection_new(int fd)
{
   struct socks_connection *sc = malloc(sizeof(struct socks_connection));
    if (!sc) {
        printf("can not malloc socks connection, return\n");
        return NULL;
    }

    sc->fd = fd;
    sc->status = SCS_WAITING_SELECTION_MSG;
    return sc;
}

// after accept
static void
socks_parser_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    struct bufferevent *bev_in;
    // 打印对端的信息
    printf("\nfd: %d, addr:%s, port:%d\n", fd,
    inet_ntoa(((struct sockaddr_in*)sa)->sin_addr),
    ntohs(((struct sockaddr_in*)sa)->sin_port));

    struct socks_connection *sc = socks_connection_new(fd);
    if (!sc) {
        return;
    }

    struct socks_parser *parser = (struct socks_parser *)user_data;
    bev_in = bufferevent_socket_new(parser->evbase, fd, 
        BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
    if (!bev_in) {
        printf("can not new bufferevent socket, return\n");
        free(sc);
        return ;
    }

    bufferevent_setcb(bev_in, socks_data_read_process, 
        NULL, socks_data_event_process, (void*)sc);
    bufferevent_enable(bev_in, EV_READ|EV_WRITE);
}

void
socks_parser_listerner_new_bind(struct socks_parser *parser)
{
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(parser->config->socks_parser_port);

    parser->evlistener = evconnlistener_new_bind(parser->evbase, socks_parser_accept_cb, (void *)parser,
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