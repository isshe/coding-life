#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "isshe_common.h"
#include "isshe_error.h"

// 如果${PAGER}已经定义，就用PAGER，否则，就用more
#define PAGER "${PAGER:-more}"

int main(int argc, char *argv[])
{
    char line[ISSHE_MAXLINE];
    FILE *fpin, *fpout;

    if (argc != 2) {
        isshe_error_exit("Usage: a.out <pathname>");
    }

    if ((fpin = fopen(argv[1], "r")) == NULL) {
        isshe_sys_error_exit("can't open %s", argv[1]);
    }

    if ((fpout = popen(PAGER, "w")) == NULL) {
        isshe_sys_error_exit("popen error");
    }

    while (fgets(line, ISSHE_MAXLINE, fpin) != NULL) {
        if (fputs(line, fpout) == EOF) {
            isshe_sys_error_exit("fput error to pipe");
        }
    }

    if (ferror(fpin)) {
        isshe_sys_error_exit("fgets error");
    }

    if (pclose(fpout) == -1) {
        isshe_sys_error_exit("pclose error");
    }

    exit(0);
}