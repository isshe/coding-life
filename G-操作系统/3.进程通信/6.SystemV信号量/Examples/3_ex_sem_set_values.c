#include "isshe_common.h"
#include "isshe_ipc.h"
#include "isshe_unistd.h"
#include "isshe_error.h"

int main(int argc, char *argv[])
{
    int semid, nsems, i;
    struct semid_ds seminfo;
    unsigned short *ptr;
    union semun arg;

    if (argc < 2) {
        isshe_error_exit("Usage: semsetvalues <pathname> [ values ... ]");
    }

    semid = isshe_semget(isshe_ftok(argv[1], 0), 0, 0);
    arg.buf = &seminfo;
    isshe_semctl(semid, 0, IPC_STAT, arg);
    nsems = arg.buf->sem_nsems;

    //???: 信号量集有几个信号量，就要要几个值。（分别设置每个信号量的值）
    // 2: 1(./bin) + 1(pathname)
    if (argc != nsems + 2) {
        isshe_error_exit("%d semaphores in set, %d values specified", nsems, argc - 2);
    }

    ptr = isshe_calloc(nsems, sizeof(unsigned short));
    arg.array = ptr;
    for (i = 0; i < nsems; i++) {
        ptr[i] = atoi(argv[i+2]);
    }
    semctl(semid, 0, SETALL, arg);

    exit(0);
}