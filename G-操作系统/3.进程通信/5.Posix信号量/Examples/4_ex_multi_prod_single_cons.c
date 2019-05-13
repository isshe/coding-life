#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_pthread.h"
#include "isshe_ipc.h"

#define NBUFF 10
#define MAXNTHREAD 100

#define SEM_MUTEX   "mutex.temp"
#define SEM_NEMPTY  "nempty.temp"
#define SEM_NSTORED "nstored.temp"

int nitems, nproducers;

struct {
    int buff[NBUFF];
    int nput;
    int nputval;        // 这个不用也可以吧？！
    sem_t *mutex, *nempty, *nstored;    // 后两个是为了保证buff内容不被覆盖、保证等待。（生产者和消费者进行协同）
} shared;

void *produce(void *arg)
{
    for ( ; ; )
    {
        isshe_sem_wait(shared.nempty);  // 有空间
        isshe_sem_wait(shared.mutex);

        if (shared.nput >= nitems) {
            isshe_sem_post(shared.nempty);
            isshe_sem_post(shared.mutex);
            return NULL;
        }

        // buff[i] = [0, 1, 2, ...]
        shared.buff[shared.nput % NBUFF] = shared.nput; //shared.nputval;
        shared.nput++;
        //shared.nputval++;
        isshe_sem_post(shared.mutex);
        isshe_sem_post(shared.nstored); // 内容数量+1
        *((int *) arg) += 1;
    }

    return NULL;
}

void *consume(void *arg)
{
    int i = 0;

    for (i = 0; i < nitems; i++) {
        isshe_sem_wait(shared.nstored); // 有内容
        isshe_sem_wait(shared.mutex);
        if (shared.buff[i % NBUFF] != i) {
            printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        }
        isshe_sem_post(shared.mutex);
        isshe_sem_post(shared.nempty);  // 空余数量+1
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int count[MAXNTHREAD];
    pthread_t tid_produce[MAXNTHREAD], tid_consume;

    if (argc != 3) {
        isshe_error_exit("Usage: xxx <#items> <#producers>");
    }

    nitems = atoi(argv[1]);
    nproducers = atoi(argv[2]);

    shared.mutex = isshe_sem_open(SEM_MUTEX, O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    shared.nempty = isshe_sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, ISSHE_FILE_MODE, NBUFF);
    shared.nstored = isshe_sem_open(SEM_NSTORED, O_CREAT | O_EXCL, ISSHE_FILE_MODE, 0);

    for (int i = 0; i < nproducers; i++) {
        count[i] = 0;
        isshe_pthread_create(&tid_produce[i], NULL, produce, &count[i]);
    }

    isshe_pthread_create(&tid_consume, NULL, consume, NULL);

    for (int i = 0; i < nproducers; i++) {
        isshe_pthread_join(tid_produce[i], NULL);
        printf("count[%d] = %d\n", i, count[i]);
    }

    isshe_pthread_join(tid_consume, NULL);

    isshe_sem_unlink(SEM_MUTEX);
    isshe_sem_unlink(SEM_NEMPTY);
    isshe_sem_unlink(SEM_NSTORED);

    exit(0);
}