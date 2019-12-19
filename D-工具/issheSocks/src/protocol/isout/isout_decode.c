
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

int isout_decode_opts_block(uint8_t *data, int len)
{
    unsigned char ckey[ISSHE_AES_BLOCK_SIZE] = "master key";
    unsigned char ivec[ISSHE_AES_BLOCK_SIZE] = "master iv";         // NOTE: ivec这个会被改变！
    //unsigned char ivec_cp[ISSHE_AES_BLOCK_SIZE];
    //memcpy(ivec_cp , ivec, ISSHE_AES_BLOCK_SIZE);
    isshe_aes_key_t key;
    int num = 0;

    printf("num = %d, ivec = %s\n", num, ivec);
    printf("src: %s\n", data);
    isshe_aes_cfb128_encrypt(data, data, len, &key, ivec, &num, ISSHE_AES_DECRYPT);
    printf("num = %d, ivec = %s\n", num, ivec);
    printf("decrypt: %s\n", data);
    return ISSHE_SUCCESS;
}

int isout_decode_opts(struct bufferevent *bev, uint8_t *buf, uint32_t read)
{
    // 一块一块解密，解密完一块，就找找是否存在结束选项标记
    isout_opt_s *opt;
    uint32_t decoded = 0;
    uint32_t i = 0;
    // TODO NOT FINISHED
    while(i < ISOUT_ALL_OPT_MAX_LEN) {
        if (decoded > read) {
            // read
            bufferevent_read(bev, buf + read, ISSHE_AES_BLOCK_SIZE);
            read += ISSHE_AES_BLOCK_SIZE;
            continue;
        }
        if (decode < read) {
            // decode
            isout_decode_opts_block(buf + decoded, ISSHE_AES_BLOCK_SIZE);
            decoded += ISSHE_AES_BLOCK_SIZE;
            continue;
        }
        opt = (isout_opt_s *)(buf + i);
        i += sizeof(opt->type) + sizeof(opt->len) + len;
        if (opt->type == ISOUT_OPT_END) {
            return i;
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
    // 读取HMAC
    bufferevent_read(inbev, hmac, sizeof(hmac));
    bufferevent_read(inbev, opts, ISSHE_AES_BLOCK_SIZE);              // TODO 先读16

    // 校验HMAC
    if (!is_valid_hmac(opts, ISSHE_AES_BLOCK_SIZE, hmac)) {
        printf("ERROR: 无效的HMAC!!!\n");
        // TODO 清理这个连接
        return ISSHE_FAILURE;
    }

    // 解密并解析选项

    // 解密数据

    // 转发数据
    
    return ISSHE_SUCCESS;
}