

handle_events()
{
    int i, n, timeout = TIMEOUT;

    n = poll(pfd, nfds, timeout);
    if (n <= 0) {
        goto error_or_timeout;  // error or timeout
    }

    // else
    for (i = 0; n != 0; i++) {
        if (pfdi.revents == 0) {
            continue;   // 没有事件
        }

        n--;
        if (pfdi.revents & (POLLERR | POLLNVAL)) {
            // error
        }
        if (pfdi.revents & POLLIN) {
            readable_fd(pfdi.fd);                   // read
        }

        if (pfdi.revents & POLLOUT) {
            writeable_fd(pfdi.fd);                  // write
        }
    }

    //...
}


update_fd(int fd, int action, int events)
{
    if (action == ADD) {
        pfdfd.fd = fd;
        pfdfd.evnets = evnets;
    } else {
        pfdfd.fd = -1;
    }
}