#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "isshe_error.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        isshe_error_exit("Usage: pipeconf <pathname>");
    }

    printf("PIPE_BUF = %ld, OPEN_MAX = %ld\n", isshe_pathconf(argv[1], _PC_PIPE_BUF), isshe_sysconf(_SC_OPEN_MAX));

    exit(0);
}