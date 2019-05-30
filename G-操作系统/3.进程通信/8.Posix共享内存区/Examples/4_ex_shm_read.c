#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_file.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    int i, fd;
    struct stat stat;
    unsigned char *ptr;

    if (argc !=2 ) {
        isshe_error_exit("Usage: shmwrite <name>");
    }

    fd = isshe_shm_open(argv[1], O_RDWR, ISSHE_FILE_MODE);
    isshe_fstat(fd, &stat);
    ptr = isshe_mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    isshe_close(fd);

    isshe_info("stat.st_size = %d", stat.st_size);

    for (i = 0; i < stat.st_size; i++, ptr++) {
        if ( *ptr != (i % 256) ) {
            isshe_info("ptr[%d] = %d", i, *ptr);
        }
    }

    exit(0);
}