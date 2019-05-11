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
ssize_t isshe_read(int fd, void *ptr, size_t nbytes);
void isshe_write(int fd, void *ptr, size_t nbytes);
ssize_t isshe_readline(int fd, void *ptr, size_t maxlen);
off_t isshe_lseek(int fd, off_t offset, int whence);
void isshe_ftruncate(int fd, off_t length);

#endif