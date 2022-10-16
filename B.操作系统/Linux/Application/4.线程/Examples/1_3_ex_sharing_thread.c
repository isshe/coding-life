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

#define N 2

void *thread(void *vargp);

char **ptr;

int main(int argc, char *argv[])
{
    int i;
    pthread_t tid;
    char *msgs[N] = {
        "Hello from foo",
        "Hello from bar"
    };

    ptr = msgs;

    for (i = 0; i < N; i++) {
        isshe_pthread_create(&tid, NULL, thread, (void *)&i);   // 存在竞争，i只有一个！
        //isshe_pthread_create(&tid, NULL, thread, (void *)i);
    }

    isshe_pthread_exit(NULL);
}


void *thread(void *vargp)
{
    int myid = *((int *)vargp);
    static int cnt = 0;
    printf("[%d]: %s (cnt = %d)\n", myid, ptr[myid], ++cnt);
    return NULL;
}