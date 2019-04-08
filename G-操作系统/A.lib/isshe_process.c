#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/wait.h>

#include "isshe_process.h"
#include "isshe_common.h"

pid_t isshe_fork(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        isshe_error("Fork error");
    }

    return pid;
}

void print_exit_status(int status)
{
    if (WIFEXITED(status)) {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
        WCOREDUMP(status) ? "(core file generated)" : "");
#else
        "");
#endif
    } else if (WIFSTOPPED(status)) {
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}