
// 之后把这些提取到个人库里边

#include "isshe_socks.h"

#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

void print_addr(struct addrinfo *addr_info)
{
    struct sockaddr_in *addr;
    struct addrinfo *info;

    for(info = addr_info; info != NULL; info = info->ai_next)
    {
        addr = (struct sockaddr_in*)(info->ai_addr);
        printf("ip addresss: %s\n", inet_ntoa(addr->sin_addr));
    }
}

int domain_name_resolution(const char *domain_name, struct addrinfo **res)
{
    struct addrinfo hints;
    printf("domain_name = %s\n", domain_name);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;              // 协议族
    hints.ai_flags = AI_CANONNAME;
    //hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = 0;

    // 记得使用freeaddrinfo进行释放！
    return getaddrinfo(domain_name, NULL, &hints, res);
}

int isshe_domain_name_resolution(const char *domain_name, struct addrinfo **res)
{
    int rc;

    if ((rc = domain_name_resolution(domain_name, res)) != 0) {
        printf("domain name resolution error: %s\n", gai_strerror(rc));
        exit(1);
    }

    return rc;
}
