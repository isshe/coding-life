
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

int isshe_msgget(key_t key, int flag)
{
    int rc;

    if ( (rc = msgget(key, flag)) == -1) {
        isshe_error_exit("msgget error");
    }

    return(rc);
}

void isshe_msgctl(int, int, struct msqid_ds *)
{
    if (msgctl(id, cmd, buf) == -1) {
        isshe_error_exit("msgctl error");
    }
}

void isshe_msgsnd(int, const void *, size_t, int)
{
    if (msgsnd(id, ptr, len, flag) == -1){
        isshe_error_exit("msgsnd error");
    }
}

ssize_t isshe_msgrcv(int, void *, size_t, int, int)
{
    ssize_t	rc;

    if ( (rc = msgrcv(id, ptr, len, type, flag)) == -1){
        isshe_error_exit("msgrcv error");
    }

    return(rc);
}