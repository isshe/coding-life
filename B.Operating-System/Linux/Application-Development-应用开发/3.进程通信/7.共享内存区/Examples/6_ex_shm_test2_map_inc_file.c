#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"
#include "isshe_unistd.h"
#include "isshe_file.h"

#define FILE "inc.temp"
#define SIZE 32768

int main(int argc, char *argv[])
{
    int fd, i;
    char *ptr;

    fd = isshe_open(FILE, O_RDWR|O_CREAT|O_TRUNC, ISSHE_FILE_MODE);
    ptr = isshe_mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (i = 4096; i <= SIZE; i += 4096) {
        printf("setting file size to %d\n", i);
        isshe_ftruncate(fd, i);
        printf("ptr[%d] = %d\n", i - 1, ptr[i - 1]);
    }
    close(fd);
    exit(0);
}