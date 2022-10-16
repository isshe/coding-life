#include "isshe_common.h"
#include "isshe_rio.h"
#include "isshe_error.h"

/****************************************
 * 健壮I/O函数：标准I/O的包装函数
 ****************************************/
/*
 * rio_readn - 健壮地读取n个字节 (unbuffered/无缓冲)
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) { /* Interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            }
            else {
                return -1;      /* errno set by read() */
            }
        }
        else if (nread == 0) {
            break;              /* EOF */
        }
        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft);         /* Return >= 0 */
}

/*
 * rio_writen - 健壮地写n个字节 (unbuffered/无缓冲)
 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR) { /* Interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */
            }
            else {
                return -1;       /* errno set by write() */
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }

    return n;
}

/*
 * rio_read - 这是Unix read()的包装函数。(非导出函数)
 *      它将min(n, rio_cnt)字节从内部缓冲区传输到用户缓冲区。
 *      其中n是用户请求字节数，rio_cnt是内部缓冲区的未读字节数。
 *      如果内部缓冲区为空，通过调用read()重新填充内部缓冲区。
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) /* Interrupted by sig handler return */
            return -1;
        }
        else if (rp->rio_cnt == 0) { /* EOF */
            return 0;
        }
        else {
            rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
        }
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if (rp->rio_cnt < n) {
        cnt = rp->rio_cnt;
    }

    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

/*
 * rio_readinitb - 将一个描述符和读缓冲区、重置缓冲区关联。
 */
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/*
 * rio_readnb - 健壮地读n个字节 (buffered/缓冲)
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) {
            return -1;          /* errno set by read() */
        }
        else if (nread == 0) {
            break;              /* EOF */
        }

        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

/*
 * rio_readlineb - 健壮地读一行 (buffered/缓冲)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return 0; /* EOF, no data read */
            }
            else {
                break;    /* EOF, some data was read */
            }
        } else {
            return -1;	  /* Error */
        }
    }
    *bufp = 0;
    return n-1;
}

/****************************************
 * 健壮I/O的包装函数
 ****************************************/
ssize_t isshe_rio_readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if ((n = rio_readn(fd, ptr, nbytes)) < 0) {
        isshe_sys_error_exit("rio_readn error");
    }
    return n;
}

void isshe_rio_writen(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n) {
        isshe_sys_error_exit("rio_writen error");
    }
}

void isshe_rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
}

ssize_t isshe_rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0) {
        isshe_sys_error_exit("rio_readnb error");
    }
    return rc;
}

ssize_t isshe_rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0) {
        isshe_sys_error_exit("rio_readlineb error");
    }
    return rc;
}
