
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_pthread.h"

#define MAXNITEMS       1000000
#define MAXNTHREADS     100

int nitems;
int buff[MAXNITEMS];

struct {
    pthread_mutex_t mutex;
    int nput;
    int nval;
} put = {
    PTHREAD_MUTEX_INITIALIZER
};

struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int nready;
} nready = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_COND_INITIALIZER
};

void *produce(void *arg)
{
    int dosignal;
    for (; ;) {
        isshe_pthread_mutex_lock(&put.mutex);
        if (put.nput >= nitems) {
            isshe_pthread_mutex_unlock(&put.mutex);
            return NULL;
        }

        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;
        isshe_pthread_mutex_unlock(&put.mutex);

        isshe_pthread_mutex_lock(&nready.mutex);
        if (nready.nready == 0) {
            dosignal = 1;
        }
        nready.nready++;
        isshe_pthread_mutex_unlock(&nready.mutex);

        // 为了避免信号发送后，接收到信号的线程立马启动，但因为没有获取到锁，又睡下去；
        // 把信号发送方到锁外
        if (dosignal) {
            isshe_pthread_cond_signal(&nready.cond);
        }

        *((int *)arg) += 1;
    }

}

void *consume(void *arg)
{
    int i = 0;
    for (i = 0; i < nitems; i++) {
        isshe_pthread_mutex_lock(&nready.mutex);
        while(nready.nready == 0) {
            // 这里会持有互斥锁进行等待吗？不会持有，等待前会释放该互斥锁
            isshe_pthread_cond_wait(&nready.cond, &nready.mutex);
        }
        nready.nready--;
        isshe_pthread_mutex_unlock(&nready.mutex);

        if (buff[i] != i) {
            printf("buff[%d] = %d\n", i, buff[i]);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i, nthreads, count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS], tid_consume;

    if (argc != 3) {
        isshe_error_exit("Usage: prodcons <#items> <#threads>");
    }

    nitems = min(atoi(argv[1]), MAXNITEMS);
    nthreads = min(atoi(argv[2]), MAXNTHREADS);

    // 创建消费者和生产者
    for (i = 0; i < nthreads; i++) {
        count[i] = 0;
        isshe_pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }
    isshe_pthread_create(&tid_consume, NULL, consume, NULL);

    for (i = 0; i < nthreads; i++) {
        isshe_pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }
    isshe_pthread_join(tid_consume, NULL);

    exit(0);
}

