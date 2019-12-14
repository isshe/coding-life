#ifndef _ISSHE_COMMON_H_
#define _ISSHE_COMMON_H_

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>        // For bool type
#include <endian.h>

#if defined(__bsdi__) || defined(__APPLE__)
#include <sys/syslimits.h>  // for OPEN_MAX

#define va_mode_t   int
#else
#define va_mode_t   mode_t
#endif

#ifndef OPEN_MAX
#define OPEN_MAX FOPEN_MAX
#endif

#define ISSHE_SUCCESS    0
#define ISSHE_FAILURE    (-1)
#define ISSHE_TRUE      1
#define ISSHE_FALSE     0
#define MAXLINE         4096
#define ISSHE_MAXLINE   MAXLINE

#ifndef	PATH_MAX                /* should be in <limits.h> */
#define	PATH_MAX        1024    /* max # of characters in a pathname */
#endif

/* default permissions for new files */
#define	ISSHE_FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* default permissions for new directories */
#define	ISSHE_DIR_MODE  (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define	min(a,b)    ((a) < (b) ? (a) : (b))
#define	max(a,b)    ((a) > (b) ? (a) : (b))

#define isshe_memzero(buf, size) memset(buf, 0, size) 

#endif
