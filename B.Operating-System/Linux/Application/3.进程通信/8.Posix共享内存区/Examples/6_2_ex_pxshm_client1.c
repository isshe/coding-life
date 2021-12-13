#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"
#include "isshe_unistd.h"
#include "isshe_file.h"

struct shmstruct {
    int count;
};

sem_t *mutex;

int main(int argc, char *argv[])
{
    int fd, i, nloop;
    pid_t pid;
    struct shmstruct *ptr;

    if (argc != 4) {
        isshe_error_exit("usage: server1 <shmname> <semname> <#loops>");
    }

    nloop = atoi(argv[3]);

    // shm
    fd = isshe_shm_open(isshe_posix_ipc_name(argv[1]), O_RDWR, ISSHE_FILE_MODE);
    ptr = isshe_mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    isshe_close(fd);

    // sem
    mutex = isshe_sem_open(isshe_posix_ipc_name(argv[2]), 0);
    pid = getpid();
    for (i = 0; i < nloop; i++) {
        isshe_sem_wait(mutex);
        printf("pid %ld: %d\n", (long)pid, ptr->count++);
        isshe_sem_post(mutex);
    }

    exit(0);
}