// 分段求和

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

#define MAXTHREADS 32

void *sum(void *vargp);

long psum[MAXTHREADS];
long nelems_per_thread;

int main(int argc, char *argv[])
{
    long i, nelems, log_nelems, nthreads, result = 0;
    pthread_t tid[MAXTHREADS];
    int myid[MAXTHREADS];

    if (3 != argc) {
        // nthread 需要是2的幂
        // log_nelems 是元素个数的对数
        isshe_error_exit("usage: %s <nthreads> <log_nelems>", argv[0]);
    }
    nthreads = atoi(argv[1]);
    log_nelems = atoi(argv[2]);

    nelems = (1L << log_nelems);
    nelems_per_thread = nelems / nthreads;

    for (i = 0; i < nthreads; i++) {
        myid[i] = i;
        isshe_pthread_create(&tid[i], NULL, sum, &myid[i]);
    }

    for (i = 0; i < nthreads; i++) {
        isshe_pthread_join(tid[i], NULL);
    }

    for (i = 0; i < nthreads; i++) {
        result += psum[i];
    }

    if (result != (nelems * (nelems - 1))/2) {
        printf("Error: result=%ld\n", result);
    }

    exit(0);
}

void *sum(void *vargp)
{
    int myid = *((int *)vargp);
    long start = myid * nelems_per_thread;
    long end = start + nelems_per_thread;
    long i, sum = 0;

    for (i = start; i < end; i++) {
        sum += i;
    }

    psum[myid] = sum;

    return NULL;
}