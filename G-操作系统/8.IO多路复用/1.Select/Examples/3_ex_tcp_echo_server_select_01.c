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
    int     i, maxi, maxfd, listenfd, connfd, sockfd;
    int     nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set  rset, allset;
    char    buf[MAXLINE];
    socklen_t   chilen;
    struct  sockaddr_in cliaddr;

    if (2 != argc) {
        isshe_error_exit("usage: %s <port>", argv[0]);
    }

    listenfd = isshe_open_listen_fd(argv[1]);
    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for ( ; ; ) {
        rset = allset;
        nready = isshe_select(maxfd + 1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)) {
            chilen = sizeof(cliaddr);
            connfd = isshe_accept(listenfd, (SA *)&cliaddr, &chilen);

            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }

            if (i == FD_SETSIZE) {
                isshe_error("too many clients");
            } else {
                FD_SET(connfd, &allset);
                if (connfd > maxfd) {
                    maxfd = connfd;
                }
                if (i > maxi) {
                    maxi = i;
                }
            }
            if (--nready <= 0) {
                continue;
            }
        }

        for (i = 0; i <= maxi; i++) {
            if ( (sockfd = client[i]) < 0 ) {
                continue;
            }
            if (FD_ISSET(sockfd, &rset)) {
                if ( ( n = isshe_read(sockfd, buf, MAXLINE)) == 0 ) {
                    isshe_close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
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