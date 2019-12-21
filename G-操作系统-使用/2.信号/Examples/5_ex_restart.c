#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <setjmp.h>

#include "isshe_process.h"
#include "isshe_signal.h"

sigjmp_buf buf;

void handler(int sig)
{
    siglongjmp(buf, 1);
}

int main(int argc, char *argv[])
{
    isshe_signal(SIGINT, handler);

    if (!sigsetjmp(buf, 1)) {
        printf("starting...\n");
    } else {
        printf("restarting...\n");
    }

    while(1) {
        sleep(1);
        printf("processing...\n");  // 第二次ctrl+c是退出了！
    }

    exit(0);
}