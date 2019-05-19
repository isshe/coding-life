
#include "isshe_common.h"
#include "isshe_ipc.h"
#include "isshe_unistd.h"
#include "isshe_error.h"

int main(int argc, char *argv[])
{
    int semid;

    if (argc != 2) {
        isshe_error_exit("Usage: semrmid <pathname>");
    }

    semid = isshe_semget(isshe_ftok(argv[1], 0), 0, 0);
    isshe_semctl(semid, 0, IPC_RMID);

    exit(0);
}