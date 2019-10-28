#ifndef _ISSHE_CONFIG_PARSER_H_
#define _ISSHE_CONFIG_PARSER_H_

struct isshe_socks_config
{
    uint16_t socks_parser_port;
};

void config_parse(struct isshe_socks_config *config);
void config_print(struct isshe_socks_config *config);

#endif