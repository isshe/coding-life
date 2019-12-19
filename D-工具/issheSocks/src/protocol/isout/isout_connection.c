
#include "isshe_common.h"

#include "isout_connection.h"
#include "isout_protocol.h"


void *isout_opts_malloc_copy(isout_opt_s *opt)
{
    void *temp = malloc(opt->len);
    if (!temp) {
        printf("ERROR: isout_opts_copy malloc!!!\n");
        exit(0);
    }

    memcpy(temp, opt->data, opt->len);
    return temp;
}

int isout_opts_parse(isout_conn_opts_s *opts, uint8_t *opts_str)
{
    isout_opt_s *opt;
    int index;

    if (!opts || !opts_str) {
        printf("ERROR: !opts && !opts_str!!!\n");
        return ISSHE_FAILURE;
    }

    do {
        opt = (isout_opt_s *)opts_str + index;
        switch (opt->type)
        {
        case ISOUT_OPT_COUNT:
            opts->count = ntohll(*(uint64_t *)opt->data);
            break;
        case ISOUT_OPT_RANDOM:
            opts->random = ntohl(*(uint32_t *)opt->data);
            break;
        case ISOUT_OPT_DOMAIN:
            opts->dname_len = opt->len;
            opts->dname = (uint8_t *)isout_opts_malloc_copy(opt);
            break;
        case ISOUT_OPT_IPV6:
            opts->ipv6_len = opt->len;
            opts->ipv6 = (uint8_t *)isout_opts_malloc_copy(opt);
            break;
        case ISOUT_OPT_IPV4:
            opts->ipv4 = ntohl(*(uint32_t *)opt->data);      // 大端转主机
            break;
        case ISOUT_OPT_PORT:
            opts->port = ntohs(*(uint16_t *)opt->data);
            break;
        case ISOUT_OPT_ADDR_TYPE:
            opts->addr_type = opt->data[0];
            break;
        case ISOUT_OPT_DATA_LEN:
            opts->user_data_len = ntohs(*(uint16_t *)opt->data);
            break;
        case ISOUT_OPT_CRYPTO_ALGO:
            break;
        case ISOUT_OPT_CRYPTO_KEY:
            break;
        case ISOUT_OPT_CRYPTO_IV:
            break;
        default:
            break;
        }
        index += sizeof(opt->type) + sizeof(opt->len) + opt->len;
    } while(opt->type != ISOUT_OPT_END);

    return ISSHE_SUCCESS;
}