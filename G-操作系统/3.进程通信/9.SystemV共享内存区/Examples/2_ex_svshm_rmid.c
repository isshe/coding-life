#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_unistd.h"
#include "isshe_ipc.h"
#include "isshe_file.h"
#include "isshe_stdio.h"
#include "isshe_signal.h"
#include "isshe_pthread.h"
#include "isshe_process.h"
#include "isshe_time.h"

int main(int argc, char *argv[])
{
    int     id;

    if (argc != 2) {
        isshe_error_exit("usage: shmrmid <pathname>");
    }

    id = isshe_shmget(isshe_ftok(argv[1], 0), 0, ISSHE_SVSHM_MODE);
    printf("shm id = %d\n", id);
    isshe_shmctl(id, IPC_RMID, NULL);

    exit(0);
}