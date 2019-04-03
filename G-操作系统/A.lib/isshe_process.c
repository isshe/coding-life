#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <string.h>

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