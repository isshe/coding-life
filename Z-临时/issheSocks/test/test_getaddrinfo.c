#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

int hostname_to_ip(const char* hostname)
{
        int ret = 0;
        struct sockaddr_in *addr;

        if(!hostname)
        {
                printf("invalid params\n");
                return -1;
        }

        struct addrinfo hints;
        struct addrinfo *res, *res_p;
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET;
        //hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_CANONNAME;
        hints.ai_protocol = 0;

        ret = getaddrinfo(hostname, NULL, &hints, &res);
        if(ret != 0)
        {
                printf("getaddrinfo: %s\n", gai_strerror(ret));
                return -1;
        }

        for(res_p = res; res_p != NULL; res_p = res_p->ai_next)
        {
                addr = (struct sockaddr_in*)(res_p->ai_addr);
                printf("ip addresss: %s\n", inet_ntoa(addr->sin_addr));
        }

        freeaddrinfo(res);
        return ret;
}

int main(void)
{
    hostname_to_ip("www.baidu.com");
}