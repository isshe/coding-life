




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
    // 
}

int isout_server_encode(isession_s *session)
{

}

