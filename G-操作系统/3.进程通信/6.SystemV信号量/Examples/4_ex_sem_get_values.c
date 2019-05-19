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

    if (argc != 2) {
        isshe_error_exit("Usage: semsetvalues <pathname>");
    }

    semid = isshe_semget(isshe_ftok(argv[1], 0), 0, 0);
    arg.buf = &seminfo;
    isshe_semctl(semid, 0, IPC_STAT, arg);
    nsems = arg.buf->sem_nsems;

    ptr = isshe_calloc(nsems, sizeof(unsigned short));
    arg.array = ptr;
    semctl(semid, 0,GETALL, arg);
    for (i = 0; i < nsems; i++) {
        printf("semval[%d] = %d\n", i, ptr[i]);
    }

    exit(0);
}