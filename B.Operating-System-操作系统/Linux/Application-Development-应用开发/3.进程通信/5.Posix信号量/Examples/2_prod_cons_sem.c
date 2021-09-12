

#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_pthread.h"
#include "isshe_ipc.h"

#define NBUFF 10
#define SEM_MUTEX   "mutex.temp"
#define SEM_NEMPTY  "nempty.temp"
#define SEM_NSTORED "nstored.temp"

int nitems;
struct {
    int buff[NBUFF];
    sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void *arg)
{
    int i;

    for (i = 0; i < nitems; i++)
    {
        isshe_sem_wait(shared.nempty);  // 有空间
        isshe_sem_wait(shared.mutex);
        shared.buff[i%NBUFF] = i;
        isshe_sem_post(shared.mutex);
        isshe_sem_post(shared.nstored); // 内容数量+1
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
    pthread_t tid_produce, tid_consume;

    if (argc != 2) {
        isshe_error_exit("Usage: xxx <#ittems>");
    }

    nitems = atoi(argv[1]);

    shared.mutex = isshe_sem_open(SEM_MUTEX, O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    shared.nempty = isshe_sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, ISSHE_FILE_MODE, NBUFF);
    shared.nstored = isshe_sem_open(SEM_NSTORED, O_CREAT | O_EXCL, ISSHE_FILE_MODE, 0);

    isshe_pthread_create(&tid_produce, NULL, produce, NULL);
    isshe_pthread_create(&tid_consume, NULL, consume, NULL);

    isshe_pthread_join(tid_produce, NULL);
    isshe_pthread_join(tid_consume, NULL);

    isshe_sem_unlink(SEM_MUTEX);
    isshe_sem_unlink(SEM_NEMPTY);
    isshe_sem_unlink(SEM_NSTORED);

    exit(0);
}