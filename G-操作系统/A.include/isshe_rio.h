
#ifndef _ISSHE_RIO_H_
#define _ISSHE_RIO_H_

#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

/****************************************
 * 健壮I/O函数：标准I/O的包装函数
 ****************************************/
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/****************************************
 * 健壮I/O的包装函数
 ****************************************/
ssize_t isshe_rio_readn(int fd, void *ptr, size_t nbytes);
void isshe_rio_writen(int fd, void *usrbuf, size_t n);
void isshe_rio_readinitb(rio_t *rp, int fd);
ssize_t isshe_rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t isshe_rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

#endif