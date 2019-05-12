#ifndef _ISSHE_IPC_H_
#define _ISSHE_IPC_H_

#ifdef __linux__
#include <sys/msg.h>
#include <semaphore.h>
#endif

#ifndef SEM_FAILED
#define SEM_FAILED  ((sem_t *)(-1))
#endif

/*
 * Posix IPC对象使用路径名进行标识，此函数的作用就是生成完整路径名
 */
char *isshe_posix_ipc_name(const char *);

/* system v 4个消息队列函数*/
int isshe_msgget(key_t key, int flag);
void isshe_msgctl(int id, int cmd, struct msqid_ds *buf);
void isshe_msgsnd(int id, const void *ptr, size_t len, int flag);
ssize_t isshe_msgrcv(int id, void *ptr, size_t len, int type, int flag);

// Posix信号量相关函数
sem_t * isshe_sem_open(const char *pathname, int oflag, ...);
void isshe_sem_close(sem_t *sem);
void isshe_sem_unlink(const char *pathname);
void isshe_sem_init(sem_t *sem, int pshared, unsigned int value);
void isshe_sem_destroy(sem_t *sem);
void isshe_sem_wait(sem_t *sem);
int isshe_sem_trywait(sem_t *sem);
void isshe_sem_post(sem_t *sem);
void isshe_sem_getvalue(sem_t *sem, int *valp);

#endif