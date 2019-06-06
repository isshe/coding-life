#ifndef _ISSHE_RPC_H_
#define _ISSHE_RPC_H_

#include <rpc/rpc.h>

CLIENT *isshe_clnt_create(char *host,
    unsigned long prog, unsigned long vers, char *proto);

bool_t isshe_clnt_control(CLIENT *cl, int req, char *info);

#endif