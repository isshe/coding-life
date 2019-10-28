
#include "isshe_config_parser.h"

void
config_parse(struct isshe_socks_config *config)
{
    config->socks_parser_port = 27758;
}

void
config_print(struct isshe_socks_config *config)
{
    printf("-------------isshe socks config--------------");
    printf("socks parser port: %d\n", config->socks_parser_port);
    printf("---------------------------------------------");
}