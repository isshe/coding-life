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

#define NTHREADS 4
#define SBUFSIZE 16

void echo(int connfd);
void *thread(void *vargp);

sbuf_t sbuf;

int main(int argc, char *argv[])
{
    int listenfd, *connfdp, i;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    pthread_t tid;

    if (2 != argc) {
        isshe_error_exit("usage: %s <port>", argv[0]);
    }
    listenfd = isshe_open_listen_fd(argv[1]);

    while (1) {
        clientlen = sizeof(struct sockaddr_in);     // 每次都更新
        connfdp = isshe_malloc(sizeof(int));
        *connfdp = isshe_accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        isshe_pthread_create(&tid, NULL, thread, connfdp);
    }

    exit(0);
}

void echo(int connfd)
{
    size_t n;
    char buf[ISSHE_MAXLINE];
    rio_t rio;

    isshe_rio_readinitb(&rio, connfd);
    while((n = isshe_rio_readlineb(&rio, buf, ISSHE_MAXLINE)) != 0) { //line:netp:echo:eof
        printf("server received %d bytes\n", (int)n);
        isshe_rio_writen(connfd, buf, n);
    }
}

void *thread(void *vargp)
{
    printf("thread id = %ld\n", (long)isshe_pthread_self());
    int connfd = *((int *)vargp);

    isshe_pthread_detach(pthread_self());
    isshe_free(vargp);
    echo(connfd);
    isshe_close(connfd);
    return NULL;
}