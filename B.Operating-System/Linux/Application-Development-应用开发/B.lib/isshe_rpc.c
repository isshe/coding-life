#include "isshe_rpc.h"
#include "isshe_error.h"

CLIENT *isshe_clnt_create(char *host,
    unsigned long prog, unsigned long vers, char *proto)
{
    CLIENT	*cl;

    if ( (cl = clnt_create(host, prog, vers, proto)) == NULL) {
        clnt_pcreateerror(host);
        isshe_error_exit("clnt_create error");
    }

    return(cl);
}

bool_t isshe_clnt_control(CLIENT *cl, int req, char *info)
{
    bool_t ret;

    if ( (ret = clnt_control(cl, req, info)) == FALSE) {
        isshe_error_exit("clnt_control error");
    }

    return(ret);
}
