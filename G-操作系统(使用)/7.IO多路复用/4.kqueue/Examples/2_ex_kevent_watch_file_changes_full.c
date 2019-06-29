#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_stdio.h"
#include "isshe_unistd.h"
#include "isshe_file.h"
#include "isshe_ipc.h"
#include "isshe_pthread.h"
#include "isshe_process.h"
#include "isshe_signal.h"
#include "isshe_socket.h"
#include "isshe_time.h"
#include "isshe_rio.h"
#include "isshe_rpc.h"
#include "isshe_sbuf.h"

int main()
{
    int n;
    struct timespec nullts = {0, 0};
    struct kevent ev;
    int kq = isshe_kqueue();
    int fd;

    fd = isshe_open("test.temp", O_RDWR | O_CREAT, ISSHE_FILE_MODE);

    EV_SET(&ev, fd, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR,
        NOTE_RENAME | NOTE_WRITE | NOTE_DELETE | NOTE_ATTRIB, 0, 0);

    isshe_kevent(kq, &ev, 1, NULL, 0, &nullts);

    for ( ; ; ) {
        n = isshe_kevent(kq, NULL, 0, &ev, 1, NULL);
        //n = isshe_kevent(kq, &ev, 1, &ev, 1, NULL);       // 把两个kevent写到一起，是不可以监听到rename和delete的。
        if (n > 0) {
            printf("The file was ");

            if (ev.fflags & NOTE_RENAME) {
                printf("renamed");
            }

            if (ev.fflags & NOTE_WRITE) {
                printf("written");
            }

            if (ev.fflags & NOTE_DELETE) {
                printf("deleted");
                break;
            }

            if (ev.fflags & NOTE_ATTRIB) {
                printf("chmod/chowned");
            }

            printf("\n");
        }
    }
    isshe_close(fd);

    exit(0);
}
