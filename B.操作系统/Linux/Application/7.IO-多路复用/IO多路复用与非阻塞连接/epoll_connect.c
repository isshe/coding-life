#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_EVENTS 10

int main() {
    const char *hostname = "www.example.com";
    const char *port = "80";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 先获取 flags，然后设置 flags
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    // 解析域名
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    // AF_UNSPEC：不指定协议族
    hints.ai_family = AF_UNSPEC;
    // SOCK_STREAM：TCP
    hints.ai_socktype = SOCK_STREAM;
    // AI_ADDRCONFIG：只返回适用于本机网络配置的地址
    // AI_ALL：返回所有可用的地址，包括 IPv4 和 IPv6 地址
    hints.ai_flags = AI_ADDRCONFIG | AI_ALL;

    // 查询地址信息
    int status = getaddrinfo(hostname, port, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // 打印第一个地址信息
    if (result->ai_family == AF_INET) {  /* IPv4 */
        struct sockaddr_in *sin = (struct sockaddr_in *)result->ai_addr;
        char ipstr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sin->sin_addr, ipstr, INET_ADDRSTRLEN);
        printf("IPv4: %s\n", ipstr);
    }

    // result 其实是一个链表，可以逐个遍历进行连接
    // 发起连接
    int ret = connect(sockfd, result->ai_addr, result->ai_addrlen);
    if (ret == 0) {
        // 如果连接已经建立，直接处理连接
        printf("connected!\n");
    } else if (errno == EINPROGRESS) {
        // 如果连接建立中，则使用 epoll 监听连接是否可写
        printf("epoll creating...\n");
        int epollfd = epoll_create(1);
        if (epollfd < 0) {
            perror("epoll_create");
            exit(1);
        }

        struct epoll_event event;
        // EPOLLOUT：可写事件
        // EPOLLET：边缘触发，只有状态变化才触发。默认是水平触发。
        event.events = EPOLLOUT | EPOLLET;
        event.data.fd = sockfd;
        printf("epoll add event...\n");
        // EPOLL_CTL_ADD：增加监听事件
        ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event);
        if (ret < 0) {
            perror("epoll_ctl");
            exit(1);
        }

        struct epoll_event events[MAX_EVENTS];
        printf("epoll waiting...\n");
        ret = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (ret < 0) {
            perror("epoll_wait");
            exit(1);
        }

        // 检查连接是否可写
        int i;
        for (i = 0; i < ret; i++) {
            // EPOLLOUT：可写
            if (events[i].data.fd == sockfd && (events[i].events & EPOLLOUT)) {
                int error = 0;
                socklen_t len = sizeof(error);
                // 获取套接字上的错误代码，即在非阻塞模式下发起连接请求时，返回的错误状态
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
                printf("connected!\n");
            }
        }
    } else {
        // 连接失败
        perror("connect");
        exit(1);
    }

    freeaddrinfo(result);
    close(sockfd);

    return 0;
}
