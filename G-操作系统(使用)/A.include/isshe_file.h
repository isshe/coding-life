#ifndef _ISSHE_FILE_H_
#define _ISSHE_FILE_H_

#include <unistd.h>
#include <sys/stat.h>

#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int isshe_lock_file(int fd);
int isshe_unlock_file(int fd);

// 简化fcntl锁相关的调用
int lock_reg(int, int, int, off_t, int, off_t); /* {Prog lockreg} */
pid_t lock_test(int, int, off_t, int, off_t); /* {Prog locktest} */

// 几个记录锁的宏
#define read_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define	readw_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define	write_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define	writew_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define	un_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
#define	is_read_lockable(fd, offset, whence, len) \
            (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define	is_write_lockable(fd, offset, whence, len) \
            (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

// 失败退出
void isshe_lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);
pid_t isshe_lock_test(int fd, int type, off_t offset, int whence, off_t len);

#define isshe_read_lock(fd, offset, whence, len) \
            isshe_lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define	isshe_readw_lock(fd, offset, whence, len) \
            isshe_lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define	isshe_write_lock(fd, offset, whence, len) \
            isshe_lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define	isshe_writew_lock(fd, offset, whence, len) \
            isshe_lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define	isshe_un_lock(fd, offset, whence, len) \
            isshe_lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
#define	isshe_is_read_lockable(fd, offset, whence, len) \
            (isshe_lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define	isshe_is_write_lockable(fd, offset, whence, len) \
            (isshe_lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

int isshe_open(const char *pathname, int oflag, ...);
void isshe_close(int fd);
void isshe_unlink(const char *pathname);
ssize_t isshe_read(int fd, void *ptr, size_t nbytes);
void isshe_write(int fd, void *ptr, size_t nbytes);
ssize_t isshe_readline(int fd, void *ptr, size_t maxlen);
off_t isshe_lseek(int fd, off_t offset, int whence);
void isshe_ftruncate(int fd, off_t length);
void isshe_fstat(int fd, struct stat *ptr);

#endif