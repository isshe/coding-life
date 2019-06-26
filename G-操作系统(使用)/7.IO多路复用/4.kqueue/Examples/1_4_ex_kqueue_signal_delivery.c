int i, n;
struct timespec nullts = {0, 0};

EV_SET(&ev, SIGHUP, EVFILT_SIGNAL, EV_ADD | EV_ENABLE, 0, 0, 0);

kevent(kq, &ev, 1, NULL, 0, &nullts);

for (; ; ) {
    n = kevent(kq, NULL, 0, &ev, 1, NULL);
    if (n > 0) {
        printf("signal %d delivered %d timesn", ev.ident, ev.data);
    }
}