#ifndef _ISSHE_TUNNEL_CLIENT_H_
#define _ISSHE_TUNNEL_CLIENT_H_

/*
{
    pair1:{
        tunnel1:
        tunnel2:
        tunnel3:
    },
    pair2:{
        tunnel1:
        tunnel2:
    }
}
*/

struct tunnel
{
    int fd;
    int pair_addr;
    int pair_port;
}

struct tunnel_client_pair
{
    int ntun;
    struct tunnel *tunnels;
}

struct tunnel_client
{
    int npair;
    struct tunnel_client_pair *pairs;

}

#endif