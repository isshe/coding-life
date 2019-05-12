
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//  System V message queue
int isshe_msgget(key_t key, int flag)
{
    int rc;

    if ( (rc = msgget(key, flag)) == -1) {
        isshe_sys_error_exit("msgget error");
    }

    return(rc);
}

void isshe_msgctl(int id, int cmd, struct msqid_ds *buf)
{
    if (msgctl(id, cmd, buf) == -1) {
        isshe_sys_error_exit("msgctl error");
    }
}

void isshe_msgsnd(int id, const void *ptr, size_t len, int flag)
{
    if (msgsnd(id, ptr, len, flag) == -1){
        isshe_sys_error_exit("msgsnd error");
    }
}

ssize_t isshe_msgrcv(int id, void *ptr, size_t len, int type, int flag)
{
    ssize_t	rc;

    if ( (rc = msgrcv(id, ptr, len, type, flag)) == -1){
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

void isshe_sem_close(sem_t *sem)
{
    if (sem_close(sem) == -1) {
        isshe_sys_error_exit("sem_close error");
    }
}

void isshe_sem_unlink(const char *pathname)
{
    if (sem_unlink(pathname) == -1) {
        isshe_sys_error_exit("sem_unlink error");
    }
}

void isshe_sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) == -1) {
        isshe_sys_error_exit("sem_init error");
    }
}

void isshe_sem_destroy(sem_t *sem)
{
    if (sem_destroy(sem) == -1) {
        isshe_sys_error_exit("sem_destroy error");
    }
}

void isshe_sem_wait(sem_t *sem)
{
    if (sem_wait(sem) == -1) {
        isshe_sys_error_exit("sem_wait error");
    }
}

int isshe_sem_trywait(sem_t *sem)
{
    int     rc;

    if ( (rc = sem_trywait(sem)) == -1 && errno != EAGAIN) {
        isshe_sys_error_exit("sem_trywait error");
    }

    return(rc);
}

void isshe_sem_post(sem_t *sem)
{
    if (sem_post(sem) == -1) {
        isshe_sys_error_exit("sem_post error");
    }
}

void isshe_sem_getvalue(sem_t *sem, int *valp)
{
    if (sem_getvalue(sem, valp) == -1) {
        isshe_sys_error_exit("sem_getvalue error");
    }
}
