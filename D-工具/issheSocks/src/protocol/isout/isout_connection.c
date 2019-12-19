
#include "isout_connection.h"

/*
int isout_conn_opts_update(uint8_t *buf, uint8_t type, uint8_t len, uint8_t *all, uint8_t *sent)
{
    memcpy(sent, all, len);
    return isout_opt_append(buf, type, len, all);
}

int isout_conn_opts_update_64(uint8_t *buf, uint8_t type, uint64_t *all, uint64_t *sent)
{
    return isout_conn_opts_update(buf, type, sizeof(*all), all, sent);
}

int isout_conn_opts_update_32(uint8_t *buf, uint8_t type, uint32_t *all, uint32_t *sent)
{
    return isout_conn_opts_update(buf, type, sizeof(*all), all, sent);
}

int isout_conn_opts_update_16(uint8_t *buf, uint8_t type, uint16_t *all, uint16_t *sent)
{
    return isout_conn_opts_update(buf, type, sizeof(*all), all, sent);
}

int isout_conn_opts_update_8(uint8_t *buf, uint8_t type, uint8_t *all, uint8_t *sent)
{
    return isout_conn_opts_update(buf, type, sizeof(*all), all, sent);
}
*/