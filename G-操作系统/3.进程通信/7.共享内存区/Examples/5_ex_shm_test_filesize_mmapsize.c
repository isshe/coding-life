#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"
#include "isshe_unistd.h"
#include "isshe_file.h"

#define SEM_NAME "mysem"

int main(int argc, char *argv[])
{
    int     fd, i;
    char    *ptr;           // 注意这里的类型！！！写成int，找了半天。
    size_t  filesize, mmapsize, pagesize;

    if (argc != 4) {
        isshe_error_exit("Usage: test1 <pathname> <filesize> <mmapsize>");
    }

    filesize = atoi(argv[2]);
    mmapsize = atoi(argv[3]);
    printf("filesize = %ld, mmapsize = %ld\n", (long)filesize, (long)mmapsize);

    fd = isshe_open(argv[1], O_RDWR | O_CREAT | O_TRUNC, ISSHE_FILE_MODE);
    isshe_lseek(fd, filesize - 1, SEEK_SET);
    isshe_write(fd, "", 1);
    ptr = isshe_mmap(NULL, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    isshe_close(fd);

    pagesize = isshe_sysconf(_SC_PAGESIZE);
    printf("PAGESIZE = %ld\n", (long)pagesize);

    for (i = 0; i < max(filesize, mmapsize); i += pagesize) {
        ptr[i] = 1;
        printf("1: ptr[%d] = %d\n", i, ptr[i]);
        ptr[i + pagesize - 1] = 1;
        printf("2: ptr[%d] = %d\n", (int)(i + pagesize - 1), ptr[i + pagesize - 1]);

    }

    printf("ptr[%d] = %d\n", i, ptr[i]);
    exit(0);
}
