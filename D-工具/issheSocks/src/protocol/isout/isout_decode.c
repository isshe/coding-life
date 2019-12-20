
#include "isout_decode.h"
#include "isout_encode.h"
#include "isshe_aes_cfg128.h"
#include "isout_protocol.h"
#include "isout_connection.h"
#include "isshe_common.h"


int is_valid_hmac(uint8_t *data, uint32_t len, uint8_t *hmac)
{
    uint8_t expect_hmac[ISOUT_HMAC_LEN];
    isout_encode_hmac(data, len, expect_hmac);
    if (memcmp(hmac, expect_hmac, ISOUT_HMAC_LEN) != 0) {
        return ISSHE_FALSE;
    }
    return ISSHE_TRUE;
}


int isout_decode_data(uint8_t *data, int len)
{
    unsigned char ckey[ISSHE_AES_BLOCK_SIZE] = "session key";
    unsigned char ivec[ISSHE_AES_BLOCK_SIZE] = "session iv";        // NOTE: ivec这个会被改变！
    //unsigned char ivec_cp[ISSHE_AES_BLOCK_SIZE];
    //memcpy(ivec_cp , ivec, ISSHE_AES_BLOCK_SIZE);
    isshe_aes_key_t key;
    int num = 0;

    isshe_aes_set_encrypt_key(ckey, ISSHE_AES_BLOCK_SIZE_BIT, &key);

    printf("num = %d, ivec = %s\n", num, ivec);
    printf("src: %s\n", data);
    isshe_aes_cfb128_encrypt(data, data, len, &key, ivec, &num, ISSHE_AES_DECRYPT);
    printf("num = %d, ivec = %s\n", num, ivec);
    printf("decrypt: %s\n", data);
    return ISSHE_SUCCESS;
}


int isout_decode_opts_block(uint8_t *data, int len, isshe_aes_key_t *key, uint8_t *ivec, int *num)
{

    printf("num = %d, ivec = %s\n", num, ivec);
    printf("src: %s\n", data);
    isshe_aes_cfb128_encrypt(data, data, len, &key, ivec, &num, ISSHE_AES_DECRYPT);
    printf("num = %d, ivec = %s\n", num, ivec);
    printf("decrypt: %s\n", data);
    return len;
}

int isout_decode_opts(struct bufferevent *bev, uint8_t *buf)
{
    // 一块一块解密，解密完一块，就找找是否存在结束选项标记
    isout_opt_s *opt;
    uint32_t decoded = 0;
    uint32_t checked = 0;

    unsigned char ckey[ISSHE_AES_BLOCK_SIZE] = "master key";
    unsigned char ivec[ISSHE_AES_BLOCK_SIZE] = "master iv";
    isshe_aes_key_t key;
    int num = 0;

    isshe_aes_set_encrypt_key(ckey, ISSHE_AES_BLOCK_SIZE_BIT, &key);

    decoded += isout_decode_opts_block(buf + decoded, ISSHE_AES_BLOCK_SIZE, &key, ivec, &num);
    while(checked < ISOUT_ALL_OPT_MAX_LEN) {
        if (checked > decoded) {
            // read && decode
            // TODO 判断长度是否够
            bufferevent_read(bev, buf + decoded, ISSHE_AES_BLOCK_SIZE);
            decoded += isout_decode_opts_block(buf + decoded, ISSHE_AES_BLOCK_SIZE, &key, ivec, &num);
            continue;
        }
        opt = (isout_opt_s *)(buf + checked);
        checked += sizeof(opt->type) + sizeof(opt->len) + opt->len;
        if (opt->type == ISOUT_OPT_END) {
            return checked;
        }
    }

    return ISSHE_FAILURE;
}


int isout_decode(isession_s *session)
{
    isout_connection_s *outconn = (isout_connection_s *)session->out;
    isout_connection_s *inconn = (isout_connection_s *)session->in;
    struct bufferevent *outbev = outconn->bev;
    struct bufferevent *inbev = inconn->bev;
    uint8_t opts[ISOUT_ALL_OPT_MAX_LEN];
    uint8_t hmac[ISOUT_HMAC_LEN];
    uint8_t *data;
    uint32_t data_len;
    uint32_t opts_len;

    // TODO 长度校验，不符合长度，就返回，等待数据到来。具体实现还需再考虑。（断点续跑- -）
    // 读取HMAC及部分加密选项
    bufferevent_read(inbev, hmac, sizeof(hmac));
    bufferevent_read(inbev, opts, ISSHE_AES_BLOCK_SIZE);// TODO 先读16

    // 校验HMAC
    if (!is_valid_hmac(opts, ISSHE_AES_BLOCK_SIZE, hmac)) {
        printf("ERROR: 无效的HMAC!!!\n");
        // TODO 清理这个连接
        return ISSHE_FAILURE;
    }

    // 解密
    opts_len = isout_decode_opts(inbev, opts);
    if (opts_len == ISSHE_FAILURE) {
        printf("ERROR: isout_decode_opts\n");
        // TODO 清理这个连接
        return ISSHE_FAILURE;
    }

    // 解析选项
    isout_opts_parse(inbev->opts, opts);

    if (inbev->opts.user_data_len) {
        // 分配内存，读数据
        uint8_t *udata = (uint8_t *)malloc(inbev->opts.user_data_len);
        if (!udata) {
            printf("ERROR: isout_decode 分配内存失败！！！\n");
            exit(0);
        }

        // TODO 判断长度是否够
        bufferevent_read(inbev, udata, inbev->opts.user_data_len);

        // 解密数据
        isout_decode_data(udata, inbev->opts.user_data_len);

        // 转发数据
        bufferevent_write(outbev, udata, inbev->opts.user_data_len);
    }

    
    return ISSHE_SUCCESS;
}