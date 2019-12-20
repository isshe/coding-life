




#include "isout_encode.h"
#include "isshe_common.h"
#include "isout_connection.h"
#include "isshe_aes_cfg128.h"


int isout_opts_to_string(uint8_t *buf, isout_conn_opts_s *opts, uint64_t flag)
{
    int i = 0;

    // 这两个放前面！HMAC生成依赖这两个的随机性
    i += isout_opt_append(buf + i, ISOUT_OPT_COUNT, sizeof(opts->count), opts->count);
    i += isout_opt_append(buf + i, ISOUT_OPT_RANDOM, sizeof(opts->random), opts->random);
    if (flag & ISOUT_OPTS_FLAG_DNAME) {
        if (!opts->dname_len) {
            printf("ERROR: DNAME LEN = 0!!!\n");
            exit(0);    // TODO
        }
        i += isout_opt_append(buf + i, ISOUT_OPT_DOMAIN, opts->dname_len, opts->dname);
    }

    if (flag & ISOUT_OPTS_FLAG_IPV6) {
        if (!opts->ipv6_len) {
            printf("ERROR: IPV6 LEN = 0!!!\n");
            exit(0);    // TODO
        }
        i += isout_opt_append(buf + i, ISOUT_OPT_IPV6, opts->ipv6_len, opts->ipv6);
    }

    if (flag & ISOUT_OPTS_FLAG_IPV4) {
        i += isout_opt_append(buf + i, ISOUT_OPT_IPV4, sizeof(opts->ipv4), opts->ipv4);
    }

    if (flag & ISOUT_OPTS_FLAG_ADDR_TYPE) {
        i += isout_opt_append(buf + i, ISOUT_OPT_ADDR_TYPE, sizeof(opts->addr_type), opts->addr_type);
    }

    if (flag & ISOUT_OPTS_FLAG_PORT) {
        i += isout_opt_append(buf + i, ISOUT_OPT_PORT, sizeof(opts->port), opts->port);
    }

    i += isout_opt_append(buf + i, ISOUT_OPT_END, 0, NULL);

    // NOTE: padding一下
    i += ISSHE_AES_BLOCK_SIZE - (i % ISSHE_AES_BLOCK_SIZE);

    return i;
}

int isout_encode_opts(uint8_t *data, int len)
{
    unsigned char ckey[ISSHE_AES_BLOCK_SIZE] = "master key";
    unsigned char ivec[ISSHE_AES_BLOCK_SIZE] = "master iv";         // NOTE: ivec这个会被改变！
    //unsigned char ivec_cp[ISSHE_AES_BLOCK_SIZE];
    //memcpy(ivec_cp , ivec, ISSHE_AES_BLOCK_SIZE);
    isshe_aes_key_t key;
    int num;
    int index;
    
    assert((len % ISSHE_AES_BLOCK_SIZE) == 0);          // Note

    isshe_aes_set_encrypt_key(ckey, ISSHE_AES_BLOCK_SIZE_BIT, &key);
    for (index = 0; index < len; index += ISSHE_AES_BLOCK_SIZE) {
        printf("num = %d, ivec = %s\n", num, ivec);
        printf("src: %s\n", data);
        isshe_aes_cfb128_encrypt(data + index, data + index, len, &key, ivec, &num, ISSHE_AES_ENCRYPT);
        printf("num = %d, ivec = %s\n", num, ivec);
        printf("encrypt: %s\n", data);
    }

    return ISSHE_SUCCESS;
}

int isout_encode_data(uint8_t *data, int len)
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
    isshe_aes_cfb128_encrypt(data, data, len, &key, ivec, &num, ISSHE_AES_ENCRYPT);
    printf("num = %d, ivec = %s\n", num, ivec);
    printf("encrypt: %s\n", data);
    return ISSHE_SUCCESS;
}

int isout_encode_hmac(uint8_t *data, int len, uint8_t *result)
{
    uint8_t userpwd[32] = "abc";    // 实际是用户密码的md5
    isshe_hmac_sha256(data, len, userpwd, strlen(userpwd), result); // TODO!!!
    return ISSHE_SUCCESS;
}

// TODO 性能可能很差！多搞了两次内存复制，以及花了很多内存
int isout_encode(isession_s *session)
{
    isout_connection_s *outconn = (isout_connection_s *)session->out;
    isout_connection_s *inconn = (isout_connection_s *)session->in;
    struct bufferevent *outbev = outconn->bev;
    struct bufferevent *inbev = inconn->bev;
    uint8_t opts[ISOUT_ALL_OPT_MAX_LEN];
    uint8_t hmac[ISOUT_HMAC_LEN];
    uint8_t *data;
    int data_len;
    int opts_len;

    // 生成明文选项
    memset(opts, 0, ISOUT_ALL_OPT_MAX_LEN);
    opts_len = isout_opts_to_string(opts, &outconn->opts, outconn->opts_flag);

    // 生成加密选项
    isout_encode_opts(opts, opts_len);

    assert(opts_len > ISSHE_AES_BLOCK_SIZE);
    // 生成HMAC
    isout_encode_hmac(opts, ISSHE_AES_BLOCK_SIZE, hmac);  // TODO: 16

    // 生成加密数据
    data_len = evbuffer_get_length(bufferevent_get_input(inbev));
    data = (uint8_t *)malloc(data_len);
    if (!data) {
        printf("ERROR: malloc data error!\n");
        exit(0);
    }
    bufferevent_read(inbev, data, data_len);
    data = isout_encode_data(data, data_len);

    // 转发HMAC/加密选项/加密数据
    bufferevent_write(outbev, hmac, sizeof(hmac));
    bufferevent_write(outbev, opts, opts_len);
    bufferevent_write(outbev, data, data_len);

    free(data);
    return ISSHE_SUCCESS;
}
