#ifndef _ISSHE_SOCKS_H_
#define _ISSHE_SOCKS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void print_addr(struct addrinfo *addr_info);

int domain_name_resolution(const char *domain_name, struct addrinfo **res);

int isshe_domain_name_resolution(const char *domain_name, struct addrinfo **res);

#endif