
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "isshe_config_parser.h"

static struct socks_parser_config *
socks_parser_config_new()
{
    struct socks_parser_config *sp_config;
    sp_config = (struct socks_parser_config *)malloc(sizeof(struct socks_parser_config));
    if (!sp_config) {
        printf("malloc socks parser config failed! exiting...\n");
        exit(1);
    }

    return sp_config;
}

static struct proxy_server_config *
proxy_server_config_new()
{
    struct proxy_server_config *ps_config;
    ps_config = (struct proxy_server_config *)malloc(sizeof(struct proxy_server_config));
    if (!ps_config) {
        printf("malloc socks parser config failed! exiting...\n");
        exit(1);
    }

    return ps_config;
}

void
config_parse(struct isshe_socks_config *config)
{
    struct socks_parser_config *sp_config;
    struct proxy_server_config *ps_config;

    memset(config, 0, sizeof(struct isshe_socks_config));

    sp_config = socks_parser_config_new();
    sp_config->socks_parser_port = 1080;

    ps_config = proxy_server_config_new();
    ps_config->proxy_server_port = 27759;

    config->sp_config = sp_config;
    config->ps_config = ps_config;
}

void config_free(struct isshe_socks_config *config)
{
    if (config->sp_config) {
        free(config->sp_config);
    }

    if (config->tc_config) {
        free(config->tc_config);
    }

    if (config->ts_config) {
        free(config->ts_config);
    }

    if (config->ps_config) {
        free(config->ps_config);
    }
}

void
config_print(struct isshe_socks_config *config)
{
    printf("-------------isshe socks config--------------\n");
    printf("socks parser port: %d\n", config->sp_config->socks_parser_port);
    printf("proxy server port: %d\n", config->ps_config->proxy_server_port);
    printf("---------------------------------------------\n");
}