
int i, n;
struct timespec timeout = {TMOUT_SEC, TMOUT_NSEC};
void (*fcn)(struct kevent *);

n = kevent(kq, ch, nchanges, ev, nevents, &timeout);
if (n <= 0) {
    // goto error or timeout
}

for (i = 0; i < n; i++) {
    if (evi.flags & EV_ERROR) {
        // error
    }

    fcn = evi.udata;
    fcn(&evi);
}