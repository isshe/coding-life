
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_pthread.h"

#define MAXNITEMS       1000000
#define MAXNTHREADS     100

// 只读
int nitems;
struct {
    pthread_mutex_t mutex;
    int buff[MAXNITEMS];
    int nput;
    int nval;
} shared = {
    PTHREAD_MUTEX_INITIALIZER
};

void *produce(void *arg)
{
    for (; ;) {
        pthread_mutex_lock(&shared.mutex);
        if (shared.nput >= nitems) {
            pthread_mutex_unlock(&shared.mutex);
            return NULL;
        }

        shared.buff[shared.nput] = shared.nval;
        shared.nput++;
        shared.nval++;
        pthread_mutex_unlock(&shared.mutex);
        *((int *)arg) += 1;
    }

}

void *consume(void *arg)
{
    int i = 0;
    for (i = 0; i < nitems; i++) {
        if (shared.buff[i] != i) {
            printf("buff[%d] = %d\n", i, shared.buff[i]);
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

    for (i = 0; i < nthreads; i++) {
        count[i] = 0;
        isshe_pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }

    for (i = 0; i < nthreads; i++) {
        isshe_pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }

    isshe_pthread_create(&tid_consume, NULL, consume, NULL);
    isshe_pthread_join(tid_consume, NULL);

    exit(0);
}

