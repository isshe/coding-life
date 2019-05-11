#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_file.h"
#include "isshe_unistd.h"

#define PATH_PID_FILE "pid.temp"

int main(int argc, char *argv[])
{
    int pidfd;
    char line[ISSHE_MAXLINE];

    pidfd = isshe_open(PATH_PID_FILE, O_RDWR | O_CREAT, ISSHE_FILE_MODE);

    if (write_lock(pidfd, 0, SEEK_SET, 0) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            isshe_error_exit("unable to lock %s, is %s already running?", PATH_PID_FILE, argv[0]);
        } else {
            isshe_sys_error_exit("unable to lock %s", PATH_PID_FILE);
        }
    }

    snprintf(line, sizeof(line), "%ld\n", (long)getpid());
    isshe_ftruncate(pidfd, 0);
    isshe_write(pidfd, line, strlen(line));

    // do what you want

    pause();
}