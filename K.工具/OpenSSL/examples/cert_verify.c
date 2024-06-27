#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// 自定义验证回调函数
int custom_verify_callback(int ok, X509_STORE_CTX *ctx) {
    if (!ok) {
        int err = X509_STORE_CTX_get_error(ctx);
        if (err == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT ||
            err == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN) {
            // 跳过自签名证书的验证
            return 1;
        }
    }
    return ok;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <cert_file> <ca_file>\n", argv[0]);
        return 1;
    }

    const char *cert_file = argv[1];
    const char *ca_file = argv[2];

    // 初始化 OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // 读取证书
    FILE *fp = fopen(cert_file, "r");
    if (!fp) {
        fprintf(stderr, "Error opening certificate file\n");
        return 1;
    }
    X509 *cert = PEM_read_X509(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!cert) {
        fprintf(stderr, "Error reading certificate\n");
        return 1;
    }

    // 创建证书存储
    X509_STORE *store = X509_STORE_new();
    if (!store) {
        fprintf(stderr, "Error creating X509_STORE\n");
        X509_free(cert);
        return 1;
    }

    // 加载 CA 证书
    if (!X509_STORE_load_locations(store, ca_file, NULL)) {
        fprintf(stderr, "Error loading CA file\n");
        X509_free(cert);
        X509_STORE_free(store);
        return 1;
    }

    // 创建验证上下文
    X509_STORE_CTX *ctx = X509_STORE_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Error creating X509_STORE_CTX\n");
        X509_free(cert);
        X509_STORE_free(store);
        return 1;
    }

    // 初始化验证上下文
    if (!X509_STORE_CTX_init(ctx, store, cert, NULL)) {
        fprintf(stderr, "Error initializing X509_STORE_CTX\n");
        X509_STORE_CTX_free(ctx);
        X509_free(cert);
        X509_STORE_free(store);
        return 1;
    }

    // 设置自定义验证回调函数
    X509_STORE_CTX_set_verify_cb(ctx, custom_verify_callback);

    // 验证证书
    int ret = X509_verify_cert(ctx);

    if (ret == 1) {
        printf("Certificate verification successful\n");
    } else if (ret == 0) {
        int err = X509_STORE_CTX_get_error(ctx);
        printf("Certificate verification failed: %s\n", X509_verify_cert_error_string(err));
    } else {
        printf("Certificate verification error\n");
    }

    // 清理资源
    X509_STORE_CTX_free(ctx);
    X509_free(cert);
    X509_STORE_free(store);
    ERR_free_strings();
    EVP_cleanup();

    return 0;
}
