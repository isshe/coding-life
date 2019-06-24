#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_stdio.h"
#include "isshe_unistd.h"
#include "isshe_file.h"
#include "isshe_ipc.h"
#include "isshe_pthread.h"
#include "isshe_process.h"
#include "isshe_signal.h"
#include "isshe_socket.h"
#include "isshe_time.h"
#include "isshe_rio.h"
#include "isshe_rpc.h"
#include "isshe_sbuf.h"

#define MAX_EVENTS 10

int main(int argc, char *argv[])
{
#ifdef __linux__
    int     i, j, maxi, listenfd, connfd, sockfd, epollfd;
    int     nready;
    ssize_t n;
    char    buf[MAXLINE];
    socklen_t   chilen;
    struct sockaddr_in cliaddr;
    int client[OPEN_MAX];
    struct epoll_event ev, events[MAX_EVENTS];

    if (2 != argc) {
        isshe_error_exit("usage: %s <port>", argv[0]);
    }

    listenfd = isshe_open_listen_fd(argv[1]);

    epollfd = isshe_epoll_create(0);

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    isshe_epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

    maxi = 0;
    for (i = 1; i < OPEN_MAX; i++) {
        client[i] = -1;
    }

    for ( ; ; ) {
        nready = isshe_epoll_wait(epollfd, events, MAX_EVENTS, -1, NULL);
        for (i = 0; i < nready; i++) {
            if (events[i].data.fd == listenfd) {
                chilen = sizeof(cliaddr);
                connfd = isshe_accept(listenfd, (SA *)&cliaddr, &chilen);
                //setnonblocking(connfd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connfd;
                isshe_epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev);

                for (j = 0; j < OPEN_MAX; j++) {
                    if (client[j] < 0) {
                        client[j] = connfd;
                        break;
                    }
                }

                if (i == OPEN_MAX) {
                    isshe_error("too many clients");
                } else {
                    if (i > maxi) {
                        maxi = i;
                    }
                }
            } else {
                sockfd = events[i].data.fd;
                if ( ( n = isshe_read(sockfd, buf, MAXLINE)) < 0 ) {
                    if (errno == ECONNRESET) {
                        isshe_close(sockfd);
                        for (j = 0; j < maxi; j++) {
                            if (client[j] == sockfd) {
                                client[j] = -1;
                            }
                        }
                    } else {
                        isshe_sys_error("(%d)read error", client[i]);
                    }
                } else if (n == 0) {
                    isshe_close(sockfd);
                    for (j = 0; j < maxi; j++) {
                        if (client[j] == sockfd) {
                            client[j] = -1;
                        }
                    }
                    isshe_info("closed %d", sockfd);
                } else {
                    isshe_write(sockfd, buf, n);
                }
            }
        } // for (ready)
    }   // for(;;)
#endif

    exit(0);
}