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

#define	MAX_NIDS    4096

int main(int argc, char *argv[])
{
    int     i, j, shmid[MAX_NIDS];
    void    *addr[MAX_NIDS];
    unsigned long size;

    // 检查一个进程能创建多少个共享内存区
    for (i = 0; i < MAX_NIDS; i++) {
        shmid[i] = shmget(IPC_PRIVATE, 1024, ISSHE_SVSHM_MODE | IPC_CREAT);
        if (shmid[i] == -1) {
            printf("%d identifiers open at once\n", i);
            break;
        }
    }

    // 删除前面创建的
    for(j = 0; j < i; j++) {
        isshe_shmctl(shmid[j], IPC_RMID, NULL);
    }

    // 检查一个进程能连接/依附(attach)多少个共享内存区
    for (i = 0; i < MAX_NIDS; i++) {
        shmid[i] = isshe_shmget(IPC_PRIVATE, 1024, ISSHE_SVSHM_MODE | IPC_CREAT);
        addr[i] = shmat(shmid[i], NULL, 0);
        if (addr[i] == (void *)-1) {
            printf("%d shared memory segments attached at once\n", i);
            isshe_shmctl(shmid[i], IPC_RMID, NULL); // the one that failed
            break;
        }
    }

    for(j = 0; j < i; j++) {
        isshe_shmdt(addr[j]);
        isshe_shmctl(shmid[j], IPC_RMID, NULL);
    }

    // 检查可以创建的最小的共享内存区
    for (size = 1; ; size++) {
        shmid[0] = shmget(IPC_PRIVATE, size, ISSHE_SVSHM_MODE | IPC_CREAT);
        if (shmid[0] != -1) {   // 检查第一个成功的
            printf("minimun size of shared memory segment = %lu\n", size);
            isshe_shmctl(shmid[0], IPC_RMID, NULL);
            break;
        }
    }

    // 检查可以创建的最大的共享内存区
    for (size = 65536; ; size += 4096) {
        shmid[0] = shmget(IPC_PRIVATE, size, ISSHE_SVSHM_MODE | IPC_CREAT);
        if (shmid[0] == -1) {   // 检查第一个失败的
            printf("maximun size of shared memory segment = %lu\n", size - 4096);
            break;
        }

        isshe_shmctl(shmid[0], IPC_RMID, NULL);
    }

    exit(0);
}