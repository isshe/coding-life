#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"
#include "isshe_unistd.h"
#include "isshe_file.h"

#define SEM_NAME "mysem"

int main(int argc, char *argv[])
{
    int     i, nloop;
    int     *ptr;
    sem_t   *mutex;

    if (argc != 2) {
        isshe_error_exit("Usage: incr3 <#loops>");
    }

    nloop = atoi(argv[1]);

    ptr = isshe_mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);

    // create, initialize, unlink semaphore
    mutex = isshe_sem_open(isshe_posix_ipc_name(SEM_NAME), O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    isshe_sem_unlink(isshe_posix_ipc_name(SEM_NAME));      // ???

    setbuf(stdout, NULL);       // stdout is unbuffered
    if (isshe_fork() == 0) {
        // child
        for (i = 0; i < nloop; i++) {
            isshe_sem_wait(mutex);
            printf("child: %d\n", (*ptr)++);
            isshe_sem_post(mutex);
        }
        exit(0);
    }

    for (i = 0; i < nloop; i++) {
        isshe_sem_wait(mutex);
        printf("parent: %d\n", (*ptr)++);
        isshe_sem_post(mutex);
    }
    exit(0);
}
