#include "7_1_ex_pxshm_server2_client2.h"

int main(int argc, char *argv[])
{
    int fd, index, lastnoverflow, temp;
    long offset;
    struct shmstruct *ptr;
    sem_t *mutex, *nempty, *nstored, *noverflowmutex;

    if (argc != 2) {
        isshe_error_exit("usage: server1 <shmname>");
    }

    // shm
    shm_unlink(isshe_posix_ipc_name(argv[1]));
    fd = isshe_shm_open(isshe_posix_ipc_name(argv[1]), O_RDWR | O_CREAT | O_EXCL, ISSHE_FILE_MODE);
    isshe_ftruncate(fd, sizeof(struct shmstruct));
    ptr = isshe_mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    isshe_close(fd);

    // 初始化偏移到数组中
    for (index = 0; index < NMESG; index++) {
        ptr->msgoff[index] = index * MESGSIZE;
    }

    sem_unlink(isshe_posix_ipc_name(MUTEX_NAME));
    mutex = isshe_sem_open(isshe_posix_ipc_name(MUTEX_NAME), O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    sem_unlink(isshe_posix_ipc_name(NEMPTY_NAME));
    nempty = isshe_sem_open(isshe_posix_ipc_name(NEMPTY_NAME), O_CREAT | O_EXCL, ISSHE_FILE_MODE, NMESG);
    sem_unlink(isshe_posix_ipc_name(NSTORED_NAME));
    nstored = isshe_sem_open(isshe_posix_ipc_name(NSTORED_NAME), O_CREAT | O_EXCL, ISSHE_FILE_MODE, 0);
    sem_unlink(isshe_posix_ipc_name(NFMUTEX_NAME));
    noverflowmutex = isshe_sem_open(isshe_posix_ipc_name(NFMUTEX_NAME), O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);

    index = 0;
    lastnoverflow = 0;
    for (; ;) {
        isshe_sem_wait(nstored);
        isshe_sem_wait(mutex);

        offset = ptr->msgoff[index];
        printf("index = %d: %s\n", index, &ptr->msgdata[offset]);
        if (++index >= NMESG) {
            index = 0;
        }
        isshe_sem_post(mutex);
        isshe_sem_post(nempty);

        isshe_sem_wait(noverflowmutex);
        temp = ptr->noverflow;
        isshe_sem_post(noverflowmutex);

        if (temp != lastnoverflow) {
            printf("noverflow = %d\n", temp);
            lastnoverflow = temp;
        }
    }

    exit(0);
}