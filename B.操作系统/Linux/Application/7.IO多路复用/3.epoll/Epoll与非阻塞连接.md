

参考示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 10

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 设置非阻塞连接
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8888);

    // 发起连接
    int ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == 0) {
        // 如果连接已经建立，直接处理连接
        // ...
    } else if (errno == EINPROGRESS) {
        // 如果连接建立中，则使用 epoll 监听连接是否可写
        int epollfd = epoll_create(1);
        if (epollfd < 0) {
            perror("epoll_create");
            exit(1);
        }

        struct epoll_event event;
        event.events = EPOLLOUT | EPOLLET;
        event.data.fd = sockfd;
        ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event);
        if (ret < 0) {
            perror("epoll_ctl");
            exit(1);
        }

        struct epoll_event events[MAX_EVENTS];
        ret = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (ret < 0) {
            perror("epoll_wait");
            exit(1);
        }

        // 检查连接是否可写
        int i;
        for (i = 0; i < ret; i++) {
            if (events[i].data.fd == sockfd && (events[i].events & EPOLLOUT)) {
                int error = 0;
                socklen_t len = sizeof(error);
                ret = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
                if (ret < 0) {
                    perror("getsockopt");
                    exit(1);
                }

                if (error != 0) {
                    // 连接失败
                    printf("connect failed: %s\n", strerror(error));
                    exit(1);
                }

                // 连接成功，处理连接
                // ...
            }
        }
    } else {
        // 连接失败
        perror("connect");
        exit(1);
    }

    close(sockfd);

    return 0;
}
```
