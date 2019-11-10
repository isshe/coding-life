
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

void
connect_cmd_process(struct socks_connection *sc, 
    struct bufferevent *bev, struct socks_request *request)
{
    switch (request->atype)
    {
        case SOCKS_ADDR_TYPE_DOMAIN:
            sc->target_type = ISSHE_SOCKS_ADDR_TYPE_DOMAIN;
            bufferevent_read(bev, &sc->target_len, sizeof(sc->target_len));
            break;
        case SOCKS_ADDR_TYPE_IPV4:
            sc->target_type = ISSHE_SOCKS_ADDR_TYPE_IPV4;
            sc->target_len = IPV4_ADDR_LEN;
            break;
        case SOCKS_ADDR_TYPE_IPV6:
            sc->target_type = ISSHE_SOCKS_ADDR_TYPE_IPV6;
            sc->target_len = IPV6_ADDR_LEN;
            break;
        default:
            break;
    }

    // 复制到target
    int len = evbuffer_get_length(bufferevent_get_input(bev));
    if (sc->target_len + sizeof(sc->target_port) > len) {
        printf("Warning: expect len %d, get len %d\n", sc->target_len, len);
        sc->target_len = len - sizeof(sc->target_port);
    }
    sc->target = (char *)malloc(sc->target_len + 1);
    if (!sc->target) {
        printf("Error: malloc failed!\n");
        // TODO：释放内存，关闭连接
    }
    bufferevent_read(bev, sc->target, sc->target_len);
    sc->target[(int)sc->target_len] = '\0';
    printf("Debug: target: %s\n", sc->target);

    bufferevent_read(bev, &sc->target_port, sizeof(sc->target_port));
}

int scs_request_process(struct socks_connection *sc, struct bufferevent *bev)
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

struct socks_connection *
socks_connection_new(int fd)
{
   struct socks_connection *sc = malloc(sizeof(struct socks_connection));
    if (!sc) {
        printf("can not malloc socks connection, return\n");
        return NULL;
    }

    memset(sc, 0, sizeof(struct socks_connection));
    sc->fd = fd;
    sc->status = SCS_WAITING_SELECTION_MSG;

    return sc;
}


static void
connect_to_next(struct socks_connection *sc)
{
    struct bufferevent *bev_out;

    if (isshe_domain_name_resolution(sc->target, &sc->target_ai)) {
        // TODO: 考虑bev_in怎么处理
        //bufferevent_free(bev_in);
        return;
    }
    
    bev_out = bufferevent_socket_new(sc->sp->evbase, -1,
        BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);

    assert(bev_out);

    struct sockaddr_in *addr;
    addr = (struct sockaddr_in*)(sc->target_ai->ai_addr);
    addr->sin_port = sc->target_port;

    printf("isshe: connet to: %s(%d):%d\n", 
        inet_ntoa(addr->sin_addr), sc->target_ai->ai_addrlen, ntohs(addr->sin_port));
        
	//isshe_print_addrinfo(sc->target_ai);

    if (bufferevent_socket_connect(bev_out,
		(struct sockaddr*)addr, sizeof(struct sockaddr_in)) < 0) {

		perror("bufferevent_socket_connect");
		bufferevent_free(bev_out);
		//bufferevent_free(bev_in);
		return;
	}
    
    // disable linger
    
    int out_fd = bufferevent_getfd(bev_out);
    struct linger linger;
    memset(&linger, 0, sizeof(struct linger));
    if (setsockopt(out_fd, SOL_SOCKET, SO_LINGER, 
        (const void *)&linger, sizeof(struct linger)) != 0) {
        
        printf("can not disable linger!\n");
    }

    sc->bev_out = bev_out;

    bufferevent_setcb(bev_out, isshe_forward_data_read_cb, 
        NULL, isshe_forward_data_event_cb, (void*)sc->bev);
        //NULL, NULL, (void*)sc->bev);
    bufferevent_enable(bev_out, EV_READ|EV_WRITE);
}

static void
socks_data_read_cb(struct bufferevent *bev, void *ctx)
{
    struct socks_connection *sc = (struct socks_connection *)ctx;
    printf("fd: %d, ", sc->fd);
    // TODO: 这个while的内容，放到各个case里面：读取并转发/回复
    //while (evbuffer_get_length(bufferevent_get_input(bev))) {
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
            if (scs_request_process(sc, bev)) {
                sc->status = SCS_CONNECTED;
            }
            break;
        case SCS_CONNECTED:
            // TODO: 删除握手定时器
            // 转发数据
            if (!sc->bev_out) {
                connect_to_next(sc);
            }
		    bufferevent_setcb(bev, isshe_forward_data_read_cb, 
			    NULL, isshe_forward_data_event_cb, sc->bev_out);
            isshe_forward_data_read_cb(bev, sc->bev_out);
            //temp_receive_and_print_buf(bev);
            //printf("\n");
            break;
        default:
            break;
    }
	//}
}

void
socks_connection_free(struct socks_connection *sc)
{
    printf("Debug: free socks connection: %d\n", sc->fd);
    if (sc->target) {
        free(sc->target);
        sc->target = NULL;
    }

    if (sc->target_ai) {
        freeaddrinfo(sc->target_ai);
        sc->target_ai = NULL;
    }

    if (sc) {
        free(sc);
        sc = NULL;
    }
}


static void
socks_data_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    struct socks_connection *sc = (struct socks_connection *)ctx;
    isshe_forward_data_event_cb(bev, what, sc->bev_out);
	if (what & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) {
        socks_connection_free((struct socks_connection*)ctx);
	}
}

int count = 0;
// after accept
static void
socks_parser_accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    struct bufferevent *bev_in;
    // 打印对端的信息
    if (count == 1) {
        return;
    }
    //count = 1;
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
        return ;
    }

    assert(bev_in);

    sc->bev = bev_in;
    sc->sp = parser;

    bufferevent_setcb(bev_in, socks_data_read_cb, 
        NULL, socks_data_event_cb, (void*)sc);
    bufferevent_enable(bev_in, EV_READ|EV_WRITE);
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