#ifndef _ISSHE_FILE_H_
#define _ISSHE_FILE_H_

#include <unistd.h>
#include <fcntl.h>

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int isshe_lock_file(int fd);
int isshe_unlock_file(int fd);


#endif