#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLINE 1024
#define DEF_PAGER "/usr/bin/more"   // macOS pager program

int main(int argc, char *argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    char *pager, *argv0;
    FILE *fp;

    if (argc != 2) {
        printf("Usage: a.out <pathname>");
        exit(0);
    }

    fd = fopen(argv[1], "r");
    if (fd == NULL) {
        printf("can't open %s", argv[1]);
        exit(0);
    }

    if (pipe(fd) < 0) {
        printf("pipe error");
        exit(0);
    }

    if ((pid = fork()) < 0) {
        printf("fork error");
        exit(0);
    } else if (pid > 0) {
        // parent
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    } else {
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}