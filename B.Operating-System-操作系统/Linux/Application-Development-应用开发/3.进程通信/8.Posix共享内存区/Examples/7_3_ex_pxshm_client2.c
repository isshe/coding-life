#include "7_1_ex_pxshm_server2_client2.h"

int main(int argc, char *argv[])
{
    int fd, i, nloop, nusec;
    pid_t pid;
    char mesg[MESGSIZE];
    long offset;
    struct shmstruct *ptr;
    sem_t *mutex, *nempty, *nstored, *noverflowmutex;

    if (argc != 4) {
        isshe_error_exit("usage: client2 <shmname> <#loops> <#usec>");
    }
    nloop = atoi(argv[2]);
    nusec = atoi(argv[3]);

    // shm
    fd = isshe_shm_open(isshe_posix_ipc_name(argv[1]), O_RDWR, ISSHE_FILE_MODE);
    ptr = isshe_mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    isshe_close(fd);

    mutex = isshe_sem_open(isshe_posix_ipc_name(MUTEX_NAME), 0);
    nempty = isshe_sem_open(isshe_posix_ipc_name(NEMPTY_NAME), 0);
    nstored = isshe_sem_open(isshe_posix_ipc_name(NSTORED_NAME), 0);
    noverflowmutex = isshe_sem_open(isshe_posix_ipc_name(NFMUTEX_NAME), 0);

    pid = getpid();
    for (i = 0; i < nloop; i++) {
        isshe_sleep_us(nusec);
        snprintf(mesg, MESGSIZE, "pid %ld: message %d", (long)pid, i);

        // 用sem_trywait来判断是否溢出
        if (sem_trywait(nempty) == -1) {        // 这里可以演示溢出（数据丢失）
        //if (isshe_sem_wait(nempty) == -1) {   // 这里演示阻塞等待
            if (errno == EAGAIN) {
                isshe_sem_wait(noverflowmutex);
                ptr->noverflow++;   // 溢出了
                isshe_sem_post(noverflowmutex);
                continue;
            } else {
                isshe_sys_error_exit("sem_trywait error");
            }
        }

        isshe_sem_wait(mutex);
        offset = ptr->msgoff[ptr->nput];
        if ( ++(ptr->nput) >= NMESG) {
            ptr->nput = 0;
        }
        isshe_sem_post(mutex);
        strcpy(&ptr->msgdata[offset], mesg);
        isshe_sem_post(nstored);
    }

    exit(0);
}