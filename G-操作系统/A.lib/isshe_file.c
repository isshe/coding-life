
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "isshe_file.h"
#include "isshe_common.h"

static int isshe_lock_unlock(int fd, short type) {
    struct flock fl;

    fl.l_type = type;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;

    return(fcntl(fd, F_SETLK, &fl));
}

int isshe_lock_file(int fd)
{
    return isshe_lock_unlock(fd, F_WRLCK);
}


int isshe_unlock_file(int fd)
{
    return isshe_lock_unlock(fd, F_UNLCK);
}