#ifndef _ISSHE_SOCKS_H_
#define _ISSHE_SOCKS_H_

#include <stdint.h>

enum isshe_socks_opt_type
{
    ISSHE_SOCKS_OPT_AUTH_INFO = 0,
    ISSHE_SOCKS_OPT_VERSION,
    ISSHE_SOCKS_OPT_PROTOCOL,       // TCP/UDP
    ISSHE_SOCKS_OPT_ADDR_TYPE,
    ISSHE_SOCKS_OPT_ADDR,
    ISSHE_SOCKS_OPT_PORT,
    ISSHE_SOCKS_OPT_CRYPTO,         // 加密算法，没有指定就按配置文件的来
    ISSHE_SOCKS_OPT_END = 255,
}

struct isshe_socks_opt
{
    uint8_t type;
    uint8_t len;
    uint8_t data;
}

struct isshe_socks_request
{
    uint8_t *auth_info;
    uint8_t version;
    uint8_t addr_type;
    uint8_t addr_len;   // bytes
    uint8_t addr;
}



struct isshe_socks_reply
{

}

#endif