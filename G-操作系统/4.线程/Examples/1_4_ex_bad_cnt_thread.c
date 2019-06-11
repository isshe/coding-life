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

void *thread(void *vargp);

volatile int cnt = 0;

int main(int argc, char *argv[])
{
    int niters;
    pthread_t tid1, tid2;

    if (2 != argc) {
        isshe_error_exit("usage: %s <niters>", argv[0]);
    }
    niters = atoi(argv[1]);

    isshe_pthread_create(&tid1, NULL, thread, &niters);
    isshe_pthread_create(&tid2, NULL, thread, &niters);
    isshe_pthread_join(tid1, NULL);
    isshe_pthread_join(tid2, NULL);

    if (cnt != 2 * niters) {
        printf("BOOM! cnt = %d\n", cnt);
    } else {
        printf("OK! cnt = %d\n", cnt);
    }

    exit(0);
}


void *thread(void *vargp)
{
    int i = 0;
    int niters = *((int *)vargp);

    for (i = 0; i < niters; i++) {
        cnt++;
    }

    return NULL;
}