#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_unistd.h"
#include "isshe_ipc.h"
#include "isshe_file.h"
#include "isshe_stdio.h"
#include "isshe_signal.h"
#include "isshe_pthread.h"
#include "isshe_process.h"
#include "isshe_time.h"
#include "isshe_socket.h"
#include "isshe_rio.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (3 != argc) {
        isshe_error_exit("usage: %s <host> <port>\n", argv[0]);
    }

    host = argv[1];
    port = argv[2];

    clientfd = isshe_open_client_fd(host, port);
    isshe_rio_readinitb(&rio, clientfd);

    while (isshe_fgets(buf, MAXLINE, stdin) != NULL) {
        isshe_rio_writen(clientfd, buf, strlen(buf));
        isshe_rio_readlineb(&rio, buf, MAXLINE);
        isshe_fputs(buf, stdout);
    }

    isshe_close(clientfd); //line:netp:echoclient:close
    exit(0);
}
