#include "isshe_common.h"
#include "isshe_ipc.h"
#include "isshe_unistd.h"
#include "isshe_error.h"

void print_value(char *filepath)
{
    int semid, nsems, i;
    struct semid_ds seminfo;
    unsigned short *ptr;
    union semun arg;

    semid = isshe_semget(isshe_ftok(filepath, 0), 0, 0);
    arg.buf = &seminfo;
    isshe_semctl(semid, 0, IPC_STAT, arg);
    nsems = arg.buf->sem_nsems;

    ptr = isshe_calloc(nsems, sizeof(unsigned short));
    arg.array = ptr;
    semctl(semid, 0, GETALL, arg);
    for (i = 0; i < nsems; i++) {
        printf("semval[%d] = %d\n", i, ptr[i]);
    }

    free(ptr);
}


int main(int argc, char *argv[])
{
    int c, i, flag, semid, nops;
    struct sembuf *ptr;

    flag = 0;
    while ( (c = isshe_getopt(argc, argv, "nu") != -1) ) {
        switch (c)
        {
        case 'n':
            flag |= IPC_NOWAIT;
            break;
        
        default:
            flag |= SEM_UNDO;
            break;
        }
    }

    printf("argc = %d, optind = %d\n", argc, optind);
    if (argc - optind < 2) {
        isshe_error_exit("Usage: semops [-n] [-u] <pathname> operation ...");
    }
    semid = isshe_semget(isshe_ftok(argv[optind], 0), 0, 0);
    optind++;
    nops = argc - optind;

    ptr = isshe_calloc(nops, sizeof(struct sembuf));
    for (i = 0; i < nops; i++) {
        ptr[i].sem_num = i; // 信号量集内的信号量ID
        ptr[i].sem_op = atoi(argv[optind + i]);     // <0, 0, >0
        ptr[i].sem_flg = flag;
    }

    printf("---a---\n");
    print_value(argv[optind - 1]);
    isshe_semop(semid, ptr, nops);
    printf("---b---\n");
    print_value(argv[optind - 1]);

    exit(0);
}