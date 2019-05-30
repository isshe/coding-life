#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_file.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    if (argc !=2 ) {
        isshe_error_exit("Usage: shmunlink <name>");
    }

    isshe_shm_unlink(argv[1]);

    exit(0);
}