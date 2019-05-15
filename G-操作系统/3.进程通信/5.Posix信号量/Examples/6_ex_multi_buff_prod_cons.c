#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_pthread.h"
#include "isshe_ipc.h"
#include "isshe_file.h"

#define NBUFF 10
#define BUFFSIZE 4096
#define SEM_MUTEX   "mutex.temp"
#define SEM_NEMPTY  "nempty.temp"
#define SEM_NSTORED "nstored.temp"

int fd;

struct {
    struct {
        char data[BUFFSIZE];
        ssize_t n;
    }buff[NBUFF];
    sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void *arg)
{
    int i;

    for (i = 0; ; )
    {
        isshe_sem_wait(shared.nempty);  // 有空间
        isshe_sem_wait(shared.mutex);
        // critical region(临界区)
        isshe_sem_post(shared.mutex);

        shared.buff[i].n = isshe_read(fd, shared.buff[i].data, BUFFSIZE);
        if (shared.buff[i].n == 0) {    // 读完了，但是还是要通知写进程（让写进程也结束）
            isshe_sem_post(shared.nstored);
            return NULL;
        }

        if (++i >= NBUFF) {
            i = 0;
        }
        isshe_sem_post(shared.nstored); // 内容数量+1
    }

    return NULL;
}

void *consume(void *arg)
{
    int i = 0;

    for (i = 0; ; ) {
        isshe_sem_wait(shared.nstored); // 有内容
        isshe_sem_wait(shared.mutex);
        // critical region(临界区)
        isshe_sem_post(shared.mutex);

        if (shared.buff[i].n == 0) {            // 完了，不需要继续了
            return NULL;
        }

        isshe_write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n);
        if (++i >= NBUFF) {
            i = 0;
        }
        isshe_sem_post(shared.nempty);  // 空余数量+1
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid_produce, tid_consume;

    if (argc != 2) {
        isshe_error_exit("Usage: xxx <pathname>");
    }

    fd = isshe_open(argv[1], O_RDONLY);

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