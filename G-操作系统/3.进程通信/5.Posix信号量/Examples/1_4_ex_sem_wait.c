
#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    sem_t *sem;
    int val;

    if (argc != 2) {
        isshe_error_exit("Usage: semwait <name>");
    }

    sem = isshe_sem_open(argv[1], 0);
    isshe_sem_wait(sem);
    isshe_sem_getvalue(sem, &val);
    printf("sem value = %d\n", val);

    pause();        // blocks until kill
    exit(0);
}