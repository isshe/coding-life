/*
 * 情况：
 * 1. 单写，多读
 * 2. 多写，单读
 * 3. 多写，多读
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "isshe_error.h"
#include "isshe_common.h"

int fd[2];
pid_t pid;
char line[ISSHE_MAXLINE];
int n = 0;

void clean_line()
{
    memset(line, 0, ISSHE_MAXLINE);
}

void open_pipe_and_fork()
{
    if (pipe(fd) < 0) {
        isshe_error_exit("pipe error");
    }

    if ((pid = fork()) < 0) {
        isshe_error_exit("fork error");
    }
}


void test_single_write_multi_read()
{

    open_pipe_and_fork();

    if (pid > 0) {
        // parent
        write(fd[1], "hello world\n", 12);
        n = read(fd[0], line, 5);
        if (n < 0) {
            isshe_sys_error_exit("read error");
        }
        printf("test_single_write_multi_read: parent read(%d): %s\n", n, line);
        close(fd[0]);
        close(fd[1]);
    } else {
        // child
        close(fd[1]);
        n = read(fd[0], line, 5);
        if (n < 0) {
            isshe_sys_error_exit("read error");
        }
        printf("test_single_write_multi_read: child read(%d): %s\n", n, line);
        close(fd[0]);
        exit(0);
    }
}

void test_multi_write_single_read()
{
    open_pipe_and_fork();

    if (pid > 0) {
        // parent
        close(fd[0]);
        write(fd[1], "parent write...\n", 16);
        close(fd[1]);
    } else {
        // child
        write(fd[1], "child write...\n", 15);
        while((n = read(fd[0], line, ISSHE_MAXLINE)) > 0) {
            printf("test_multi_write_single_read: child read(%d): %s\n", n, line);
        }
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
}

void test_multi_write_multi_read()
{
    open_pipe_and_fork();

    if (pid > 0) {
        // parent
        write(fd[1], "parent write...\n", 16);
        n = read(fd[0], line, 10);
        if (n < 0) {
            isshe_sys_error_exit("read error");
        }
        printf("test_multi_write_multi_read: parent read(%d): %s\n", n, line);
        close(fd[0]);
        close(fd[1]);
    } else {
        // child
        write(fd[1], "child write...\n", 15);
        n = read(fd[0], line, 10);
        if (n < 0) {
            isshe_sys_error_exit("read error");
        }
        printf("test_multi_write_multi_read: child read(%d): %s\n", n, line);
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
}

int main(void)
{
    //test_single_write_multi_read();
    //test_multi_write_single_read();
    test_multi_write_multi_read();
}