
#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    int c, flags;
    sem_t *sem;
    unsigned int value;

    flags = O_RDWR | O_CREAT;
    value = 1;

    while((c = isshe_getopt(argc, argv, "ei:")) != -1) {
        switch(c) {
            case 'e':
                flags |= O_EXCL;
                break;
            case 'i':
                value = atoi(optarg);
                break;
        }
    }

    if (optind != argc - 1) {
        isshe_error_exit("Usage: semcreate [ - e ] [ -i initalvalue ] <name>");
    }

    sem = isshe_sem_open(argv[optind], flags, ISSHE_FILE_MODE, value);
    isshe_sem_close(sem);

    exit(0);
}