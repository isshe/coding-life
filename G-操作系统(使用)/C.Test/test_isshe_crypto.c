#include <stdio.h>
#include <string.h>

#include "isshe_aes_cfg128.h"

void test_aes_cfb128()
{
    unsigned char ckey[] =  "helloworldkey";
    unsigned char ivec[] = "goodbyworldkey";
    unsigned char ivec_cp[ISSHE_AES_BLOCK_SIZE];
    int bytes_read;
    unsigned char indata[ISSHE_AES_BLOCK_SIZE * 2];
    //unsigned char outdata[ISSHE_AES_BLOCK_SIZE];
    //unsigned char decryptdata[ISSHE_AES_BLOCK_SIZE];
    isshe_aes_key_t key;
    //isshe_aes_key_t dkey;

    memcpy(ivec_cp , ivec, ISSHE_AES_BLOCK_SIZE);

    isshe_aes_set_encrypt_key(ckey, 128, &key);
    //isshe_aes_set_decrypt_key(ckey, 128, &dkey);
    int num = 0;
    strcpy((char*)indata , "0123456789abcdef0123456789abcde");
    bytes_read = sizeof(indata);

    //isshe_aes_encrypt(indata, outdata, &key);
    printf("num = %d, ivec = %s\n", num, ivec);
    printf("src: %s\n", indata);
    isshe_aes_cfb128_encrypt(indata, indata, bytes_read, &key, ivec, &num, ISSHE_AES_ENCRYPT);
    printf("num = %d, ivec = %s\n", num, ivec);
    printf("encrypt: %s\n", indata);
    num = 0;
    //isshe_aes_decrypt(outdata, decryptdata, &dkey);
    printf("num = %d, ivec = %s\n", num, ivec_cp);
    isshe_aes_cfb128_encrypt(indata, indata, bytes_read, &key, ivec_cp, &num, ISSHE_AES_DECRYPT);
    printf("num = %d, ivec = %s\n", num, ivec_cp);
    printf("decrypt: %s\n", indata);
}

int main()
{
    test_aes_cfb128();
}