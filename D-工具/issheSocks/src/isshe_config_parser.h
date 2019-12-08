#ifndef _ISSHE_CONFIG_PARSER_H_
#define _ISSHE_CONFIG_PARSER_H_

#include <stdint.h>

struct socks_parser_config
{
    uint8_t enable;
    uint8_t dns_enable;
    uint8_t outproto;
    uint16_t port;
};

struct proxy_server_config
{
    uint8_t enable;
    uint8_t dns_enable;
    uint8_t inproto;
    uint8_t outproto;
    uint16_t port;
};

struct isshe_socks_config
{
    int remote_dns;
    struct socks_parser_config *sp_config;
    struct proxy_server_config *ps_config;
};

void config_parse(struct isshe_socks_config *config);
void config_print(struct isshe_socks_config *config);
void config_free(struct isshe_socks_config *config);

#endif