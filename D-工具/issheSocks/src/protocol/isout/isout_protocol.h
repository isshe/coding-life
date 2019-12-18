/*
 * 定义/处理协议本身
 */

#ifndef _ISSHE_ISOUT_PROTOCOL_H_
#define _ISSHE_ISOUT_PROTOCOL_H_

// 标准
#include <stdint.h>

#define ISOUT_ALL_OPT_MAX_LEN   1024

/*
+------------+----------+----------+
| 消息验证码 | 协议选项 | 加密数据 |
+------------+----------+----------+
*/

typedef struct {
    uint8_t type;
    uint8_t len;
    uint8_t data[0];
} isout_opt_s;

typedef enum {
    ISOUT_OPT_COUNT,
    ISOUT_OPT_DATA_PROTOCOL,
    ISOUT_OPT_DATA_LEN,
    ISOUT_OPT_IPV4,
    ISOUT_OPT_IPV6,
    ISOUT_OPT_DOMAIN,
    ISOUT_OPT_PORT,
    ISOUT_OPT_CRYPTO_ALGO,
    ISOUT_OPT_CRYPTO_KEY,
    ISOUT_OPT_CRYPTO_IV,
    ISOUT_OPT_END = 255,
};

int isout_opt_append(uint8_t *buf, uint8_t type, uint8_t len, const void *data);

void isout_opt_init_with_end(uint8_t *buf);

int isout_opts_len(uint8_t *buf);

int isout_opt_find_end(uint8_t *buf);

int isout_opt_insert(uint8_t *buf, uint8_t type, uint8_t len, const void *data);


#endif