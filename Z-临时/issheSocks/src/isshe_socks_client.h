#ifndef _ISSHE_SOCKS_CLIENT_H_
#define _ISSHE_SOCKS_CLIENT_H_

#define BUFFER_LEN 1500
#define MAX_DOMAIN_NAME 64      // 63 + '\0'

enum socks_client_status {
    SOCKS_CLIENT_STATUS_UNKNOWN = 0,
    SOCKS_CLIENT_STATUS_WAITING_SELECTION_MSG,
    SOCKS_CLIENT_STATUS_WAITING_REQUEST,
    SOCKS_CLIENT_STATUS_CONNECTED,
};

struct socks_client {
    int proxy_fd;
    int tunnel_fd;
    int status;
    struct event_base *base;
    struct addrinfo *addr_info;
};

struct socks_client_config {
    int tunnel_server_addr;
    short tunnel_server_port;

    //int tunnel_port;

};

#endif