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

#ifdef __bsdi__
#define va_mode_t   int
#else
#define va_mode_t   mode_t
#endif

#define ISSHE_SUCCEED   0
#define ISSHE_FAILED    (-1)
#define ISSHE_TRUE      1
#define ISSHE_FALSE     0
#define ISSHE_MAXLINE   4096

#ifndef	PATH_MAX                /* should be in <limits.h> */
#define	PATH_MAX        1024    /* max # of characters in a pathname */
#endif

/* default permissions for new files */
#define	ISSHE_FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* default permissions for new directories */
#define	ISSHE_DIR_MODE  (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define	min(a,b)    ((a) < (b) ? (a) : (b))
#define	max(a,b)    ((a) > (b) ? (a) : (b))

#endif
