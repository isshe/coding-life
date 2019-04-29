#include <errno.h>
#include <unistd.h>

#include "isshe_unistd.h"
#include "isshe_error.h"

long isshe_pathconf(const char *pathname, int name)
{
    long    val;

    errno = 0;  /* in case pathconf() does not change this */
    if ( (val = pathconf(pathname, name)) == -1) {
        if (errno != 0) {
            isshe_sys_error_exit("pathconf error");
        }
        else {
            isshe_sys_error_exit("pathconf: %d not defined", name);
        }
    }
    return(val);
}

long isshe_sysconf(int name)
{
    long    val;

    errno = 0;  /* in case sysconf() does not change this */
    if ( (val = sysconf(name)) == -1) {
        if (errno != 0) {
            isshe_sys_error_exit("sysconf error");
        }
        else {
            isshe_sys_error_exit("sysconf: %d not defined", name);
        }
    }
    return(val);
}