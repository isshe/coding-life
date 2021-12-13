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
    int     c, id, oflag;
    char    *ptr;
    size_t  length;

    oflag = ISSHE_SVSHM_MODE | IPC_CREAT;
    while((c = isshe_getopt(argc, argv, "e")) != -1) {
        switch (c) {
            case 'e':
                oflag |= IPC_EXCL;
                break;
        }
    }

    if (optind != argc - 2) {
        isshe_error_exit("usage: shmget [ -e ] <pathname> <length>");
    }
    printf("argv[optind] = %s\n", argv[optind]);
    length = atoi(argv[optind + 1]);

    id = isshe_shmget(isshe_ftok(argv[optind], 0), length, oflag);
    ptr = isshe_shmat(id, NULL, 0);
    printf("shm id = %d, ptr = %p\n", id, ptr);

    exit(0);
}