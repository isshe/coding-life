#ifndef _ISSHE_FILE_H_
#define _ISSHE_FILE_H_

#include <unistd.h>
#include <fcntl.h>

#define	va_mode_t   int

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int isshe_lock_file(int fd);
int isshe_unlock_file(int fd);

int isshe_open(const char *pathname, int oflag, ...);
void isshe_close(int fd);
void isshe_unlink(const char *pathname);

#endif