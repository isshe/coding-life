#ifndef _ISSHE_CONFIG_PARSER_H_
#define _ISSHE_CONFIG_PARSER_H_

#include <stdint.h>

struct socks_parser_config
{
    uint16_t socks_parser_port;
};

struct tunnel_client_config
{

};

struct tunnel_server_config
{

};

struct proxy_server_config
{
    uint16_t proxy_server_port;
};

struct isshe_socks_config
{
    int remote_dns;
    // char *dns_addrs;
    struct socks_parser_config *sp_config;
    struct tunnel_client_config *tc_config;
    struct tunnel_server_config *ts_config;
    struct proxy_server_config *ps_config;
};

void config_parse(struct isshe_socks_config *config);
void config_print(struct isshe_socks_config *config);
void config_free(struct isshe_socks_config *config);

#endif