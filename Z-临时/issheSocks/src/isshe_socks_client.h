#ifndef _ISSHE_SOCKS_CLIENT_H_
#define _ISSHE_SOCKS_CLIENT_H_

#define BUFFER_LEN 1500

enum socks_client_status {
    SOCKS_CLIENT_STATUS_UNKNOWN = 0,
    SOCKS_CLIENT_STATUS_WAITING_SELECTION_MSG,
    SOCKS_CLIENT_STATUS_WAITING_REQUEST,
    SOCKS_CLIENT_STATUS_CONNECTED,
};

struct socks_client {
    int proxy_fd;
    int tunnel_fd;
    struct event_base *base;
    int status;
};

struct socks_client_config {
    int tunnel_server_addr;
    short tunnel_server_port;

    //int tunnel_port;

};

#endif