// 和5.3（同步/记录上锁与文件锁）的第一、二个示例对应。
// 和5.3_ex对应。

#include "isshe_common.h"
#include "isshe_file.h"
#include "isshe_unistd.h"
#include "isshe_ipc.h"
#include "isshe_error.h"

#define SEQFILE "seqno.temp"
//#define LOCK_PATH "pxsem_lock"
#define LOCK_PATH "/tmp/sv_sem_lock"
#define SEM_NUM 0
#define MAX_TRIES 10

int     initflag, semid;
struct sembuf postop, waitop;

void my_lock(int fd)
{
    int oflag, i;
    union semun arg;
    struct semid_ds seminfo;

    // 创建并初始化
    if (initflag == 0) {
        oflag = IPC_CREAT | IPC_EXCL | ISSHE_SVSEM_MODE;
        semid = semget(isshe_ftok(LOCK_PATH, 0), 1, oflag);     // 非包裹函数
        if (semid >= 0) {
            arg.val = 1;
            isshe_semctl(semid, SEM_NUM, SETVAL, arg);          // 设置为1
        } else if (errno == EEXIST) {                           // 不是创建进程，发现存在了，再次获取
            semid = isshe_semget(isshe_ftok(LOCK_PATH, 0), 1, ISSHE_SVSEM_MODE);    // 不指定IPC_CREAT | IPC_EXCL
            arg.buf = &seminfo;

            for (i = 0; i < MAX_TRIES; i++) {                   // 尝试这么多次
                isshe_semctl(semid, SEM_NUM, IPC_STAT, arg);    //去sem_otime的值
                if (arg.buf->sem_otime != 0) {
                    goto init;                                  // 跳过去初始化
                }
                sleep(1);
            }

            // 报错了
            isshe_error_exit("semget OK, but semaphore not initialized");
        } else {
            // 其他错误
            printf("---isshe---: %d\n", errno);
            isshe_sys_error_exit("semget error %d", errno);
        }
init:
        initflag = 1;

        postop.sem_num = SEM_NUM;         // 信号量ID
        postop.sem_op = 1;          // +1
        postop.sem_flg = SEM_UNDO;

        waitop.sem_num = SEM_NUM;
        waitop.sem_op = -1;         // -1, <=0，就会阻塞
        waitop.sem_flg = SEM_UNDO;
    }

    isshe_semop(semid, &waitop, 1);
}

void my_unlock(int fd)
{
    isshe_semop(semid, &postop, 1);
}

int main(int argc, char *argv[])
{
    int fd;
    long i, seqno;
    pid_t pid;
    ssize_t n;
    char line[ISSHE_MAXLINE + 1];

    pid = getpid();
    fd = isshe_open(SEQFILE, O_RDWR, ISSHE_FILE_MODE);

    for (i = 0; i < 20; i++) {
        my_lock(fd);

        isshe_lseek( fd, 0L, SEEK_SET);
        n = isshe_read(fd, line, ISSHE_MAXLINE);
        line[n] = '\0';

        n = sscanf(line, "%ld\n", &seqno);
        printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long)pid, seqno);

        seqno++;

        snprintf(line, sizeof(line), "%ld\n", seqno);
        isshe_lseek(fd, 0L, SEEK_SET);
        isshe_write(fd, line, strlen(line));

        my_unlock(fd);
    }

    exit(0);
}