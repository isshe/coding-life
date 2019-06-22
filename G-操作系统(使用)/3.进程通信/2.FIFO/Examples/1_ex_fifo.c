#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "isshe_error.h"
#include "isshe_common.h"
#include "isshe_process.h"
#include "isshe_file.h"

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

void client(int readfd, int writefd)
{
    printf("client...\n");
}

void server(int readfd, int writefd)
{
    printf("server...\n");
}

int main(int argc, char *argv[])
{
    int readfd, writefd;
    pid_t childpid;

    if ((mkfifo(FIFO1, ISSHE_FILE_MODE) < 0) && (errno != EEXIST)) {
        isshe_error_exit("can't create %s", FIFO1);
    }

    if ((mkfifo(FIFO2, ISSHE_FILE_MODE) < 0) && (errno != EEXIST)) {
        unlink(FIFO1);
        isshe_error_exit("can't create %s", FIFO2);
    }

    if ( (childpid = isshe_fork()) == 0 ) {
        // child
        readfd = isshe_open(FIFO1, O_RDONLY, 0);
        writefd = isshe_open(FIFO2, O_WRONLY, 0);
        server(readfd, writefd);
        exit(0);
    }

    // parent, 这里顺序要注意！
    writefd = isshe_open(FIFO1, O_WRONLY, 0);
    readfd = isshe_open(FIFO2, O_RDONLY, 0);

    client(readfd, writefd);

    isshe_waitpid(childpid, NULL, 0);

    isshe_close(readfd);
    isshe_close(writefd);

    isshe_unlink(FIFO1);
    isshe_unlink(FIFO2);

    exit(0);
}