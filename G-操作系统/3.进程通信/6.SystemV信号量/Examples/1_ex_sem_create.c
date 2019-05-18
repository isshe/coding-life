
#include "isshe_common.h"
#include "isshe_ipc.h"
#include "isshe_unistd.h"
#include "isshe_error.h"

int main(int argc, char *argv[])
{
    int c, oflag, semid, nsems;

    oflag = SVSEM_MODE | IPC_CREAT;

    while ( (c = isshe_getopt(argc, argv, "abe")) != -1 ) {
        switch (c)
        {
        case 'e':
            oflag |= IPC_EXCL;
            break;
        
        default:
            break;
        }
    }

    // -e 可以在其他参数之后
    if (optind != argc - 2) {
        isshe_error_exit("Usage: semcreate [ -e ] <pathname> <nsems>");
    }

    printf("---isshe---: optind = %d, argc = %d, %s, %s\n", optind, argc, argv[optind], argv[optind+1]);

    nsems = atoi(argv[optind + 1]);

    semid = isshe_semget(isshe_ftok(argv[optind], 0), nsems, oflag);
    printf("---isshe---: semid = %d\n", semid);

    exit(0);
}