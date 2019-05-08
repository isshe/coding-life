#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "isshe_common.h"
#include "isshe_file.h"

#define SEQFILE "seqno"     // filename

void my_lock(int fd)
{
    return ;
}

void my_unlock(int fd)
{
    return ;
}

int main(int argc, char *argv[])
{
    int fd;
    long i, seqno;
    pid_t pid;
    ssize_t n;
    char line[ISSHE_MAXLINE + 1];

    pid = getpid();
    fd = isshe_open(SEQFILE, O_RDWR, ISSHE_FILE_MODE);

    for (i = 0; i < 20; i++) {
        my_lock(fd);

        isshe_lseek( fd, 0L, SEEK_SET);
        n = isshe_read(fd, line, ISSHE_MAXLINE);
        line[n] = '\0';

        n = sscanf(line, "%ld\n", &seqno);
        printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);

        seqno++;

        snprintf(line, sizeof(line), "%ld\n", seqno);
        isshe_lseek(fd, 0L, SEEK_SET);
        isshe_write(fd, line, strlen(line));

        my_unlock(fd);
    }

    exit(0);
}