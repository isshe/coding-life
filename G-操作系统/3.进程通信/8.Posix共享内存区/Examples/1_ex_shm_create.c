#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_file.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    int     c, fd, flags;
    char    *ptr;
    off_t   length;

    flags = O_RDWR | O_CREAT;
    while((c = isshe_getopt(argc, argv, "e")) != -1) {
        switch(c) {
            case 'e':
                flags |= O_EXCL;    // 存在就报错
                break;
        }
    }

    if (optind != argc - 2) {
        isshe_error_exit("Usage: shmcreate [ -e ] <name> <length>");
    }

    length = atoi(argv[optind + 1]);
    printf("argv[optind] = %s, argv[optind + 1] = %s\n", argv[optind], argv[optind + 1]);

    fd = isshe_shm_open(argv[optind], flags, ISSHE_FILE_MODE);
    isshe_ftruncate(fd, length);

    ptr = isshe_mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    exit(0);
}