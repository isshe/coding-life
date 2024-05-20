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

int main(int argc, char *argv[])
{
    int     i, maxi, listenfd, connfd, sockfd;
    int     nready;
    ssize_t n;
    char    buf[MAXLINE];
    socklen_t   chilen;
    struct  sockaddr_in cliaddr;
    struct pollfd client[OPEN_MAX];

    if (2 != argc) {
        isshe_error_exit("usage: %s <port>", argv[0]);
    }

    listenfd = isshe_open_listen_fd(argv[1]);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    maxi = 0;

    for ( ; ; ) {
        nready = isshe_poll(client, maxi + 1, INFTIM);

        if (client[0].revents & POLLRDNORM) {
            chilen = sizeof(cliaddr);
            connfd = isshe_accept(listenfd, (SA *)&cliaddr, &chilen);

            for (i = 0; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == OPEN_MAX) {
                isshe_error("too many clients");
            } else {
                client[i].events = POLLRDNORM;
                if (i > maxi) {
                    maxi = i;
                }
            }

            if (--nready <= 0) {
                continue;
            }
        }

        for (i = 0; i <= maxi; i++) {
            if ( (sockfd = client[i].fd) < 0 ) {
                continue;
            }
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ( ( n = isshe_read(sockfd, buf, MAXLINE)) < 0 ) {
                    if (errno == ECONNRESET) {
                        isshe_close(sockfd);
                        client[i].fd = -1;
                    } else {
                        isshe_sys_error("(%d)read error", client[i].fd);
                    }
                } else if (n == 0) {
                    isshe_close(sockfd);
                    client[i].fd = -1;
                    isshe_info("closed %d", sockfd);
                } else {
                    isshe_write(sockfd, buf, n);
                }

                if (--nready <= 0) {
                    break;
                }
            }
        }
    }

    exit(0);
}