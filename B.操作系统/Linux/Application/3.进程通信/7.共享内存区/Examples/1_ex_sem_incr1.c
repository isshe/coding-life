#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"


#define SEM_NAME "mysem"

int count = 0;

int main(int argc, char *argv[])
{
    int     i, nloop;
    sem_t   *mutex;

    if (argc != 2) {
        isshe_error_exit("Usage: incr1 <#loops>");
    }

    nloop = atoi(argv[1]);

    // create, initialize, unlink semaphore
    mutex = isshe_sem_open(isshe_posix_ipc_name(SEM_NAME), O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    isshe_sem_unlink(isshe_posix_ipc_name(SEM_NAME));      // ???

    setbuf(stdout, NULL);       // stdout is unbuffered
    if (isshe_fork() == 0) {
        // child
        for (i = 0; i < nloop; i++) {
            isshe_sem_wait(mutex);
            printf("child: %d\n", count++);
            isshe_sem_post(mutex);
        }
        exit(0);
    }

    for (i = 0; i < nloop; i++) {
        isshe_sem_wait(mutex);
        printf("parent: %d\n", count++);
        isshe_sem_post(mutex);
    }
    exit(0);
}
