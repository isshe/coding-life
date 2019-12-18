#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "isout_protocol.h"
#include "isshe_common.h"

/*
 * 一次加一个，返回加的这个选项的长度。
 * 预期用法是：
 * buf = [], i = 0;
 * i += append(buf + i);
 */
int isout_opt_append(uint8_t *buf, uint8_t type, uint8_t len, const void *data)
{
    if (!buf) {
        printf("ERROR: buf is NULL!!!\n");
        return ISSHE_FAILURE;
    }

    isout_opt_s *opt = (isout_opt_s *)buf;
    opt->type = type;
    opt->len = len;
    if (len) {
        memcpy(opt->data, data, opt->len);
    }
    return sizeof(type) + sizeof(len) + len;
}


void isout_opt_init_with_end(uint8_t *buf)
{
    isout_opt_s opt;
    
    opt.type = ISOUT_OPT_END;
    opt.len = 0;

    memcpy(buf, &opt, sizeof(opt.type) + sizeof(opt.len));
}

// TODO 不够健壮，只考虑了正常已初始化的情况
int isout_opt_find(uint8_t *buf, uint8_t type)
{
    int i;
    isout_opt_s *opt;
    struct isout_opt *opt;

    i = 0;
    while(i < ISOUT_ALL_OPT_MAX_LEN) {
        opt = (isout_opt_s *)(buf + i);
        if (opt->type == type) {
            return i;
        } else if (opt->type == ISOUT_OPT_END) {
            return ISSHE_FAILURE;
        }
        i += opt->len + sizeof(opt->len) + sizeof(opt->type);
    } 
}


int isout_opt_find_end(uint8_t *buf)
{
    return isout_opt_find(buf, ISOUT_OPT_END);
}


int isout_opts_len(uint8_t *buf)
{
    isout_opt_s opt;
    return isout_opt_find_end(buf) + sizeof(opt.len) + sizeof(opt.type);
}

/*
 * 插入到end选项之前
 */
int isout_opt_insert(uint8_t *buf, uint8_t type, uint8_t len, const void *data)
{
    int end_pos = isout_opt_find_end(buf);
    if (end_pos == ISSHE_FAILURE) {
        printf("find end error!!!\n");
        return ;
    }

    end_pos += isout_opt_append(buf + end_pos, type, len, data);
    end_pos += isout_opt_append(buf + end_pos, ISOUT_OPT_END, 0, NULL);
    return end_pos
}
