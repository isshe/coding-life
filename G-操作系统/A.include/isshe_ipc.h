#ifndef _ISSHE_IPC_H_
#define _ISSHE_IPC_H_

/*
 * Posix IPC对象使用路径名进行标识，此函数的作用就是生成完整路径名
 */
char *isshe_posix_ipc_name(const char *);

/* system v 4个消息队列函数*/
int isshe_msgget(key_t key, int flag);
void isshe_msgctl(int, int, struct msqid_ds *);
void isshe_msgsnd(int, const void *, size_t, int);
ssize_t isshe_msgrcv(int, void *, size_t, int, int);

#endif