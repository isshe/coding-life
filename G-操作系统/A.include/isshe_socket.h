#ifndef _ISSHE_SOCKET_H_
#define _ISSHE_SOCKET_H_

#define LISTENQ  1024  /* Second argument to listen() */

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

#endif