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
    int fd;
    struct shmstruct *ptr;

    if (argc != 3) {
        isshe_error_exit("usage: server1 <shmname> <semname>");
    }

    // shm
    shm_unlink(isshe_posix_ipc_name(argv[1]));
    fd = isshe_shm_open(isshe_posix_ipc_name(argv[1]), O_RDWR | O_CREAT | O_EXCL, ISSHE_FILE_MODE);
    isshe_ftruncate(fd, sizeof(struct shmstruct));
    ptr = isshe_mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    isshe_close(fd);

    // sem
    sem_unlink(isshe_posix_ipc_name(argv[2]));
    mutex = isshe_sem_open(isshe_posix_ipc_name(argv[2]), O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    isshe_sem_close(mutex);

    exit(0);
}