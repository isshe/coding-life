#ifndef _ISSHE_IPC_H_
#define _ISSHE_IPC_H_

#include <sys/msg.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>    // system v
#include <sys/shm.h>    // system v


#ifndef SEM_FAILED
#define SEM_FAILED  ((sem_t *)(-1))
#endif

#ifdef __linux__
#define SEM_R (0400)
#define SEM_A (0200)

union semun {       /* define union for semctl() */
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};
#endif

/* default permissions for new SV semaphores */
/* $$.ix [SVSEM_MODE]~constant,~definition~of$$ */
#define ISSHE_SVSEM_MODE    (SEM_R | SEM_A | SEM_R>>3 | SEM_R>>6)

/* default permissions for new SV shared memory */
/* $$.ix [SVSHM_MODE]~constant,~definition~of$$ */
#define ISSHE_SVSHM_MODE    (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)


/*
 * Posix IPC对象使用路径名进行标识，此函数的作用就是生成完整路径名
 */
char *isshe_posix_ipc_name(const char *);

key_t isshe_ftok(const char *pathname, int id);

/* system v 4个消息队列函数*/
int isshe_msgget(key_t key, int flag);
void isshe_msgctl(int id, int cmd, struct msqid_ds *buf);
void isshe_msgsnd(int id, const void *ptr, size_t len, int flag);
ssize_t isshe_msgrcv(int id, void *ptr, size_t len, int type, int flag);

// Posix信号量相关函数
sem_t * isshe_sem_open(const char *pathname, int oflag, ...);
int isshe_sem_close(sem_t *sem);
int isshe_sem_unlink(const char *pathname);
int isshe_sem_wait(sem_t *sem);
int isshe_sem_trywait(sem_t *sem);
int isshe_sem_post(sem_t *sem);
// macos 已经弃用了这几个
#ifdef __linux__
void isshe_sem_getvalue(sem_t *sem, int *valp);
void isshe_sem_init(sem_t *sem, int pshared, unsigned int value);
void isshe_sem_destroy(sem_t *sem);
#endif

// System V信号量
int isshe_semget(key_t key, int nsems, int flag);
int isshe_semop(int id, struct sembuf *opsptr, size_t nops);
int isshe_semctl(int id, int semnum, int cmd, ...);

// 管道
int isshe_pipe(int *fds);

// FIFO
int isshe_mkfifo(const char *pathname, mode_t mode);

// Posix共享内存
int isshe_shm_open(const char *pathname, int oflag, mode_t mode);
int isshe_shm_unlink(const char *pathname);

// System V共享内存
int isshe_shmget(key_t key, size_t size, int flags);
void * isshe_shmat(int id, const void *shmaddr, int flags);
int isshe_shmdt(const void *shmaddr);
int isshe_shmctl(int id, int cmd, struct shmid_ds *buff);

#endif