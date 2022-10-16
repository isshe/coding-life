#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"
#include "isshe_unistd.h"
#include "isshe_file.h"

int main(int argc, char *argv[])
{
    int     fd1, fd2, *ptr1, *ptr2;
    pid_t   childpid;
    struct stat stat;

    if (argc != 2) {
        isshe_error_exit("Usage: ./test3 <name>");
    }

    shm_unlink(isshe_posix_ipc_name(argv[1]));
    fd1 = isshe_shm_open(isshe_posix_ipc_name(argv[1]), O_RDWR | O_CREAT | O_EXCL, ISSHE_FILE_MODE);
    isshe_ftruncate(fd1, sizeof(int));
    fd2 = isshe_open("/tmp/motd", O_RDONLY);
    isshe_fstat(fd2, &stat);

    if ( (childpid = isshe_fork()) == 0 ) {
        // child
        ptr2 = isshe_mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd2, 0);
        ptr1 = isshe_mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
        printf("child: shm ptr = %p, motd ptr = %p\nd", ptr1, ptr2);

        sleep(5);
        printf("shared memory integer = %d\n", *ptr1);
        exit(0);
    }

    // parent: 父子顺序不同，以演示映射地址不同。
    // macOS: 父子进程都是从某个地址开始映射
    ptr1 = isshe_mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    ptr2 = isshe_mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd2, 0);
    printf("parent: shm ptr = %p, motd ptr = %p\n", ptr1, ptr2);

    *ptr1 = 777;
    isshe_waitpid(childpid, NULL, 0);

    exit(0);
}