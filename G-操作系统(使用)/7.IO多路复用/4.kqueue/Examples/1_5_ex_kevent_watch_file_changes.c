int i, n;
struct timespec nullts = {0, 0};

EV_SET(&ev, SIGHUP, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR,
    NOTE_RENAME | NOTE_WRITE | NOTE_DELETE | NOTE_ATTRIB, 0, 0);

kevent(kq, &ev, 1, NULL, 0, &nullts);

for (; ; ) {
    n = kevent(kq, NULL, 0, &ev, 1, NULL);
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
        }

        if (ev.fflags & NOTE_ATTRIB) {
            printf("chmod/chowned");
        }

        printf("\n");
    }
}