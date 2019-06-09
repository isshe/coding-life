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

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char *argv[])
{
    int listenfd, *connfdp, port;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid;

    if (2 != argc) {
        isshe_error_exit("usage: %s <port>\n", argv[0]);
    }
    port = atoi(argv[1]);

    listenfd = isshe_open_listenfd(port);



    exit(0);
}