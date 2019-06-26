handle_events()
{
    int i, n;
    struct timespec timeout = {TMOUT_SEC, TMOUT_NSEC};

    n = kevent(kq, ch, ncahnges, ev, nevents, &timeout);
    if (n <= 0) {
        // goto error or timeout
    }

    for (i = 0; i < n; i++) {
        if (evi.flags & EV_ERROR) {
            // error
        }

        if (evi.filter == EVFILT_READ) {
            // read
        }

        if (evi.filter == EVFILT_WRITE) {
            // write
        }
    }
    //...
}

update_fd(int fd, int action, int filter)
{
    EV_SET(&chnchanges, fd, filter, action == ADD ? EV_ADD : EV_DELETE, 0, 0, 0);
    nchanges++;
}