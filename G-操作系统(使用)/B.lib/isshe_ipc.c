
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include "isshe_ipc.h"
#include "isshe_error.h"
#include "isshe_common.h"

static char *posix_ipc_name(const char *name)
{
    char *dir, *dst, *slash;

    if ((dst = malloc(PATH_MAX)) == NULL) {
        return NULL;
    }

    if ((dir = getenv("PX_IPC_NAME")) == NULL) {
        dir = "/tmp/";
    }

    slash = (dir[strlen(dir) - 1] == '/') ? "" : "/";
    snprintf(dst, PATH_MAX, "%s%s%s", dir, slash, name);

    return dst;
}

char *isshe_posix_ipc_name(const char *name)
{
    char *ptr;

    ptr = posix_ipc_name(name);
    if (ptr == NULL) {
        isshe_error_exit("isshe_px_ipc_name error for %s", name);
    }

    return ptr;
}

key_t isshe_ftok(const char *pathname, int id)
{
    key_t	key;

    if ( (key = ftok(pathname, id)) == ISSHE_FAILURE) {
        isshe_sys_error_exit("ftok error for pathname \"%s\" and id %d", pathname, id);
    }
    return(key);
}

//  System V message queue
int isshe_msgget(key_t key, int flag)
{
    int rc;

    if ( (rc = msgget(key, flag)) == ISSHE_FAILURE) {
        isshe_sys_error_exit("msgget error");
    }

    return(rc);
}

void isshe_msgctl(int id, int cmd, struct msqid_ds *buf)
{
    if (msgctl(id, cmd, buf) == ISSHE_FAILURE) {
        isshe_sys_error_exit("msgctl error");
    }
}

void isshe_msgsnd(int id, const void *ptr, size_t len, int flag)
{
    if (msgsnd(id, ptr, len, flag) == ISSHE_FAILURE){
        isshe_sys_error_exit("msgsnd error");
    }
}

ssize_t isshe_msgrcv(int id, void *ptr, size_t len, int type, int flag)
{
    ssize_t	rc;

    if ( (rc = msgrcv(id, ptr, len, type, flag)) == ISSHE_FAILURE){
        isshe_sys_error_exit("msgrcv error");
    }

    return(rc);
}

// Posix semaphore
sem_t * isshe_sem_open(const char *pathname, int oflag, ...)
{
    sem_t   *sem;
    va_list ap;
    mode_t  mode;
    unsigned int    value;

    if (oflag & O_CREAT) {
        va_start(ap, oflag);        /* init ap to final named argument */
        mode = va_arg(ap, va_mode_t);
        value = va_arg(ap, unsigned int);
        if ( (sem = sem_open(pathname, oflag, mode, value)) == SEM_FAILED) {
            isshe_sys_error_exit("sem_open error for %s", pathname);
        }
        va_end(ap);
    } else {
        if ( (sem = sem_open(pathname, oflag)) == SEM_FAILED) {
            isshe_sys_error_exit("sem_open error for %s", pathname);
        }
    }
    return(sem);
}

int isshe_sem_close(sem_t *sem)
{
    int     rc;
    if ((rc = sem_close(sem)) == ISSHE_FAILURE ) {
        isshe_sys_error_exit("sem_close error");
    }

    return(rc);
}

int isshe_sem_unlink(const char *pathname)
{
    int     rc;
    if ( (rc = sem_unlink(pathname)) == ISSHE_FAILURE ) {
        isshe_sys_error_exit("sem_unlink error");
    }

    return(rc);
}

int isshe_sem_wait(sem_t *sem)
{
    int     rc;

    if ( (rc = sem_wait(sem)) == ISSHE_FAILURE ) {
        isshe_sys_error_exit("sem_wait error");
    }

    return(rc);
}

int isshe_sem_trywait(sem_t *sem)
{
    int     rc;

    if ( (rc = sem_trywait(sem)) == ISSHE_FAILURE && errno != EAGAIN) {
        isshe_sys_error_exit("sem_trywait error");
    }

    return(rc);
}

int isshe_sem_post(sem_t *sem)
{
    int     rc;

    if ( (rc = sem_post(sem)) == ISSHE_FAILURE ) {
        isshe_sys_error_exit("sem_post error");
    }

    return(rc);
}

#ifdef __linux__

void isshe_sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) == ISSHE_FAILURE) {
        isshe_sys_error_exit("sem_init error");
    }
}

void isshe_sem_destroy(sem_t *sem)
{
    if (sem_destroy(sem) == ISSHE_FAILURE) {
        isshe_sys_error_exit("sem_destroy error");
    }
}

void isshe_sem_getvalue(sem_t *sem, int *valp)
{
    if (sem_getvalue(sem, valp) == ISSHE_FAILURE) {
        isshe_sys_error_exit("sem_getvalue error");
    }
}

#endif

// system V 信号量
int isshe_semget(key_t key, int nsems, int flag)
{
    int     rc;

    if ( (rc = semget(key, nsems, flag)) == ISSHE_FAILURE) {
        isshe_sys_error_exit("semget error");
    }
    return(rc);
}

int isshe_semop(int id, struct sembuf *opsptr, size_t nops)
{
    int     rc;

    if ( (rc = semop(id, opsptr, nops)) == ISSHE_FAILURE) {
        isshe_sys_error_exit("semctl error");
    }

    return(rc);
}

int isshe_semctl(int id, int semnum, int cmd, ...)
{
    int		rc;
    va_list	ap;
    union semun	arg;

    if (cmd == GETALL || cmd == SETALL || cmd == SETVAL ||
        cmd == IPC_STAT || cmd == IPC_SET) {
        va_start(ap, cmd);		/* init ap to final named argument */
        arg = va_arg(ap, union semun);
        if ( (rc = semctl(id, semnum, cmd, arg)) == ISSHE_FAILURE) {
            isshe_sys_error_exit("semctl error");
        }
        va_end(ap);
    } else {
        if ( (rc = semctl(id, semnum, cmd)) == ISSHE_FAILURE) {
            isshe_sys_error_exit("semctl error");
        }
    }

    return(rc);
}

// 管道
int isshe_pipe(int *fds)
{
    int     rc;
    if ((rc = pipe(fds)) < 0) {
        isshe_sys_error_exit("pipe error");
    }

    return(rc);
}

// FIFO
// mkfifo -- make a fifo file
int isshe_mkfifo(const char *pathname, mode_t mode)
{
    int     rc;

    if ((rc = mkfifo(pathname, mode)) == -1) {
        isshe_sys_error_exit("mkfifo error for %s", pathname);
    }

    return(rc);
}

// posix共享内存区
int isshe_shm_open(const char *pathname, int oflag, mode_t mode)
{
    int     fd;

    if ( (fd = shm_open(pathname, oflag, mode)) == -1) {
        isshe_sys_error_exit("shm_open error for %s", pathname);
    }

    return(fd);
}

int isshe_shm_unlink(const char *pathname)
{
    int     rc;

    if ( (rc = shm_unlink(pathname)) == -1) {
        isshe_sys_error_exit("shm_unlink error");
    }

    return(rc);
}

// system V共享内存区
int isshe_shmget(key_t key, size_t size, int flags)
{
    int     rc;

    if ( (rc = shmget(key, size, flags)) == -1) {
        isshe_sys_error_exit("shmget error");
    }

    return(rc);
}

void * isshe_shmat(int id, const void *shmaddr, int flags)
{
    void    *ptr;

    if ( (ptr = shmat(id, shmaddr, flags)) == (void *) -1) {
        isshe_sys_error_exit("shmat error");
    }

    return(ptr);
}

int isshe_shmdt(const void *shmaddr)
{
    int     rc;
    if ((rc = shmdt(shmaddr)) == -1) {
        isshe_sys_error_exit("shmdt error");
    }

    return(rc);
}

int isshe_shmctl(int id, int cmd, struct shmid_ds *buff)
{
    int     rc;
    if ((rc = shmctl(id, cmd, buff)) == -1) {
        isshe_sys_error_exit("shmctl error");
    }

    return(rc);
}
