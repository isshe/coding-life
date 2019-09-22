#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
	extern int h_errno;
	struct hostent *h;
	struct in_addr in;
	struct sockaddr_in addr_in;
	h=gethostbyname("www.baidu.com");
	if(h==NULL)
	{
		printf("%s\n",hstrerror(h_errno));
	}
	else
	{
		memcpy(&addr_in.sin_addr.s_addr,h->h_addr,4);
		in.s_addr=addr_in.sin_addr.s_addr;
		printf("host name:%s\n",h->h_name);
		printf("ip lenght:%d\n",h->h_length);//IPv4 or IPv6
		printf("type:%d\n",h->h_addrtype);
		printf("ip:%s\n",inet_ntoa(in));//将一个IP转换成一个互联网标准点分格式的字符串

	}
	return 0;
}