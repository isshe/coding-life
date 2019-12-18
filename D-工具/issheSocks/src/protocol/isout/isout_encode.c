




#include "isout_encode.h"
#include "isshe_common.h"
#include "isout_connection.h"

int isout_encode(isession_s *session)
{
    isout_connection_s *inconn = (isout_connection_s *)session->in;
    isout_connection_s *outconn = (isout_connection_s *)session->out;
    struct bufferevent *dst = outconn->bev;
    struct bufferevent *src = inconn->bev;

    // 生成加密选项

    // 生成HMAC

    // 生成加密数据

    // 转发HMAC/加密选项/加密数据

    return ISSHE_SUCCESS;
}

int isout_client_encode(isession_s *session)
{
    isout_connection_s *outconn = (isout_connection_s *)session->out;
    isout_conn_opts_s *sopts = &outconn->sent_opts;
    isout_conn_opts_s *aopts = &outconn->all_opts;
    uint8_t buf[ISOUT_ALL_OPT_MAX_LEN];
    int i;

    memset(buf, 0, ISOUT_ALL_OPT_MAX_LEN);
    isout_opt_append(buf, ISOUT_OPT_COUNT, sizeof(aopts->count), aopts->count);
    sopts->count = aopts->count;
    if (aopts->dname_len && sopts->dname != aopts->dname) {
        isout_opt_append(buf, ISOUT_OPT_DOMAIN, aopts->dname_len, aopts->dname);
        if (sopts->dname) {
            free(sopts->dname);
        }
        sopts->dname = aopts->dname;
        sopts->dname_len = aopts->dname_len;
    }
    if (aopts->ipv6_len && sopts->ipv6 != aopts->ipv6) {
        isout_opt_append(buf, ISOUT_OPT_IPV6, aopts->ipv6_len, aopts->ipv6);
        if (sopts->ipv6) {
            free(sopts->ipv6);
        }
        sopts->ipv6 = aopts->ipv6;
        sopts->ipv6_len = aopts->ipv6_len;
    }

    if (aopts->addr_type != sopts->addr_type) {
        isout_opt_append(buf)
    }

}

int isout_server_encode(isession_s *session)
{

}

