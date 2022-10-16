#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_socket.h"

int isshe_socket(int domain, int type, int protocol)
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0) {
        isshe_sys_error_exit("socket error");
    }

    return rc;
}

int isshe_setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0) {
        isshe_sys_error_exit("setsockopt error");
    }

    return rc;
}

int isshe_bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0) {
        isshe_sys_error_exit("bind error");
    }

    return rc;
}

int isshe_listen(int s, int backlog)
{
    int rc;

    if ((rc = listen(s,  backlog)) < 0) {
        isshe_sys_error_exit("listen error");
    }

    return rc;
}

int isshe_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0) {
        isshe_sys_error_exit("accept error");
    }

    return rc;
}

int isshe_connect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0) {
        isshe_sys_error_exit("connect error");
    }

    return rc;
}

/*
 * 协议无关包裹函数
 */
int isshe_getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res)
{
    int rc;

    if ((rc = getaddrinfo(node, service, hints, res)) != 0) {
        isshe_gai_error_exit(rc, "getaddrinfo error");
    }

    return rc;
}

int isshe_getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags)
{
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags)) != 0) {
        isshe_gai_error_exit(rc, "getnameinfo error");
    }

    return rc;
}

void isshe_freeaddrinfo(struct addrinfo *res)
{
    freeaddrinfo(res);
}

const char * isshe_inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
    const char *res;

    if ((res = inet_ntop(af, src, dst, size)) == NULL) {
        isshe_sys_error_exit("inet_ntop error");
    }

    return res;
}

int isshe_inet_pton(int af, const char *src, void *dst)
{
    int rc;

    rc = inet_pton(af, src, dst);
    if (rc == 0) {
        isshe_app_error_exit("inet_pton error: invalid dotted-decimal address");
    }
    else if (rc < 0) {
        isshe_sys_error_exit("inet_pton error");
    }

    return rc;
}

/*
 * 一些方便使用的包裹函数
 */
int open_client_fd(char *hostname, char *port)
{
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    /* 获取潜在服务器地址列表 */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV;  /* 数字化的地址/端口字符串，不能是域名 */
    hints.ai_flags |= AI_ADDRCONFIG;  /* 仅当本地系统配置了IPv4/IPv6地址，listp才返回IPv4/IPv6地址 */
    if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (%s:%s): %s\n", hostname, port, gai_strerror(rc));
        return -2;
    }

    /* 遍历列表，找一个可以成功连接上的服务器 */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; /* Socket failed, try the next */

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
            break; /* Success */
        }

        if (close(clientfd) < 0) { /* Connect failed, try another */  //line:netp:openclientfd:closefd
            fprintf(stderr, "open_client_fd: close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) {   /* All connects failed */
        return -1;
    }

    /* The last connect succeeded */
    return clientfd;
}

int open_listen_fd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval=1;

    /* 获取潜在服务器地址列表 */
    /* AI_PASSIVE: 如果getaddrinfo第一个参数为NULL，则返回可以用于bind和accept的地址 */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* 流套接字：TCP... */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;            /* 数字化的地址/端口字符串，不能是域名 */
    if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
        return -2;
    }

    /* 遍历列表，绑定一个可以绑定的 */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            continue;  /* Socket failed, try the next */
        }

        /* 消除bind的"Address already in use"错误 */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    //line:netp:csapp:setsockopt
                   (const void *)&optval , sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        if (close(listenfd) < 0) { /* Bind failed, try the next */
            fprintf(stderr, "open_listen_fd close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) {   /* No address worked */
        return -1;
    }

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
        return -1;
    }

    return listenfd;
}

int isshe_open_client_fd(char *hostname, char *port)
{
    int rc;

    if ((rc = open_client_fd(hostname, port)) < 0) {
        isshe_sys_error_exit("open_client_fd error");
    }

    return rc;
}

int isshe_open_listen_fd(char *port)
{
    int rc;

    if ((rc = open_listen_fd(port)) < 0) {
        isshe_sys_error_exit("open_listen_fd error");
    }

    return rc;
}

ssize_t isshe_sendto(int sockfd, const void *buf, size_t len, int flags,
              const struct sockaddr *dest_addr, socklen_t addrlen)
{
    ssize_t rc;

    if ((rc = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0) {
        isshe_sys_error_exit("sendto error");
    }

    return rc;
}

ssize_t isshe_recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen)
{
    ssize_t rc;

    if ((rc = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0) {
        isshe_sys_error_exit("recvfrom error");
    }

    return rc;
}

