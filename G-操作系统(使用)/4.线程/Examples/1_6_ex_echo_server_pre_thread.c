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
#include "isshe_sbuf.h"

#define NTHREADS 4
#define SBUFSIZE 16

void echo(int connfd);
void *thread(void *vargp);
void handler(int signo);

isshe_sbuf_t sbuf;
pthread_t threads[NTHREADS];

int main(int argc, char *argv[])
{
    int listenfd, connfd, i;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    //pthread_t tid;

    if (2 != argc) {
        isshe_error_exit("usage: %s <port>", argv[0]);
    }
    listenfd = isshe_open_listen_fd(argv[1]);

    isshe_sbuf_init(&sbuf, SBUFSIZE, sizeof(int));
    isshe_signal(SIGINT, handler);

    for (i = 0; i < NTHREADS; i++) {
        isshe_pthread_create(&threads[i], NULL, thread, NULL);
    }
    while (1) {
        clientlen = sizeof(struct sockaddr_in);     // 每次都更新
        connfd = isshe_accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        isshe_sbuf_insert(&sbuf, &connfd);
    }

    exit(0);
}


static int echo_byte_cnt;
static sem_t *echo_mutex;

static void init_echo_cnt(void)
{
    //isshe_sem_unlink("echo_mutex");
    echo_mutex = isshe_sem_open("echo_mutex", O_CREAT, ISSHE_FILE_MODE, 1);
    echo_byte_cnt = 0;
}

void echo(int connfd)
{
    int n;
    char buf[ISSHE_MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    isshe_pthread_once(&once, init_echo_cnt);

    isshe_rio_readinitb(&rio, connfd);
    while((n = isshe_rio_readlineb(&rio, buf, ISSHE_MAXLINE)) != 0) { //line:netp:echo:eof
        isshe_sem_wait(echo_mutex);
        echo_byte_cnt += n;
        printf("thread %d received %d (%d total) bytes on fd %d\n",
            (int)pthread_self(), n, echo_byte_cnt, connfd);
        isshe_sem_post(echo_mutex);
        isshe_rio_writen(connfd, buf, n);
    }
}

void *thread(void *vargp)
{
    int connfd;

    printf("thread id = %ld\n", (long)isshe_pthread_self());

    isshe_pthread_detach(pthread_self());
    while(1) {
        isshe_sbuf_remove(&sbuf, &connfd);
        echo(connfd);
        isshe_close(connfd);
        printf("%ld: closed: %d\n", (long)isshe_pthread_self(), connfd);
    }

    return NULL;
}

void handler(int sig)
{
    int i;

    // 退出线程要在销毁sbuf之前
    for (i = 0; i < NTHREADS; i++) {
        isshe_pthread_cancel(threads[i]);
    }

    isshe_sbuf_destroy(&sbuf);

    exit(0);
}