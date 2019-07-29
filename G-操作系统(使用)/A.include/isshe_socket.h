#ifndef _ISSHE_SOCKET_H_
#define _ISSHE_SOCKET_H_

#define LISTENQ  1024  /* Second argument to listen() */
#define SA  struct sockaddr

int isshe_socket(int domain, int type, int protocol);

int isshe_setsockopt(int s, int level, int optname, const void *optval, int optlen);

int isshe_bind(int sockfd, struct sockaddr *my_addr, int addrlen);

int isshe_listen(int s, int backlog);

int isshe_accept(int s, struct sockaddr *addr, socklen_t *addrlen);

int isshe_connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

/*
 * 一些协议无关包裹函数
 */
int isshe_getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res);

int isshe_getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags);

void isshe_freeaddrinfo(struct addrinfo *res);

const char * isshe_inet_ntop(int af, const void *src, char *dst, socklen_t size);

int isshe_inet_pton(int af, const char *src, void *dst);

/*
 * open_client_fd - 在<hostname，port>处打开与服务器的连接，并返回准备读取和写入的套接字描述符。
 *                  这个函数是可重入和协议无关的。
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
int open_client_fd(char *hostname, char *port);

/*
 * open_listen_fd - 打开并返回port上的listening套接字; 这个函数是可重入和协议无关的
 * 错误返回:
 *  -2： getaddrinfo error
 *  -1： with errno set for other errors.
 */
int open_listen_fd(char *port);

int isshe_open_client_fd(char *hostname, char *port);
int isshe_open_listen_fd(char *port);

ssize_t isshe_sendto(int sockfd, const void *buf, size_t len, int flags,
              const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t isshe_recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen);
#endif