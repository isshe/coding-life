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

int main(int argc, char *argv[])
{
    int     i, id;
    struct shmid_ds buff;
    unsigned char *ptr;

    if (2 != argc) {
        isshe_error_exit("usage: ./a.out <pathname>");
    }

    id = isshe_shmget(isshe_ftok(argv[1], 0), 0, ISSHE_SVSHM_MODE);
    ptr = isshe_shmat(id, NULL, 0);
    isshe_shmctl(id, IPC_STAT, &buff);

    printf("buff.shm_segsz = %ld", buff.shm_segsz);
    for (i = 0; i < buff.shm_segsz; i++) {
        *ptr++ = i % 256;
    }

    exit(0);
}