#ifndef _ISSHE_SOCKS_PROTOCOL_H_
#define _ISSHE_SOCKS_PROTOCOL_H_

// 标准
#include <stdint.h>

// 第三方
#include <event2/event.h>
#include <event2/listener.h>


#define ISSHE_SOCKS_FLAG_TO_USER            1
#define ISSHE_SOCKS_FLAG_FROM_USER          2
#define ISSHE_SOCKS_FLAG_CONFIG             4

/*
+----------+---------+---------+
| 消息验证码 | 协议选项 | 加密数据 |
+----------+---------+---------+
* 消息验证码：128位/16字节，根据程序ID、时间等生成；防篡改，程序认证。
* 协议选项：类似DHCP选项的形式。通过某个加密算法加密。
* 加密数据：对用户数据进行加密
*/

enum isshe_socks_opt_type
{
    ISSHE_SOCKS_OPT_MAC = 0,        // Message Authentication Code
    ISSHE_SOCKS_OPT_VERSION,
    ISSHE_SOCKS_OPT_PROTOCOL,       // TCP/UDP
    ISSHE_SOCKS_OPT_DOMAIN,
    ISSHE_SOCKS_OPT_IPV4,
    ISSHE_SOCKS_OPT_IPV6,
    ISSHE_SOCKS_OPT_PORT,
    ISSHE_SOCKS_OPT_CRYPTO,         // 指定数据部分加密算法，没有指定就按配置文件的来
    ISSHE_SOCKS_OPT_USER,           // 用户名，可以用于流量统计等
    ISSHE_SOCKS_OPT_USER_DATA_LEN,
    ISSHE_SOCKS_OPT_END = 255,
};

enum isshe_socks_connection_status
{
    ISSHE_SCS_CLOSED = 0,
    ISSHE_SCS_ESTABLISHED = 1,
};



struct isshe_socks_opt
{
    uint8_t type;
    uint8_t len;
    uint8_t data[0];
};

// TODO 再考虑一下
/*
 * isshe_socks_header: 请求/应答，对称协议使用
 * isshe_socks_request: 请求，非对称协议使用
 * isshe_socks_reply: 应答，非对称协议使用
 */
struct isshe_socks_header
{
    uint8_t mac[16];
    struct isshe_socks_opt opts[0];
    //uint8_t version;
    //uint8_t addr_type;
    //uint8_t addr_len;   // bytes
    //uint8_t addr;
};

struct isshe_socks_request
{
    // 请求
    // uint8_t mac[16];
};

struct isshe_socks_reply
{
    // 应答
    // uint8_t mac[16];
    // isshe_socks_opt opts[0];
};

struct isshe_socks_opts
{
    uint8_t addr_type;  // 初始化为0， ISSHE_SOCKS_ADDR_TYPE_DOMAIN
    uint32_t ipv4;      // 初始化为0
    uint16_t port;      // 初始化为0
    uint8_t dname_len;  // 初始化为0
    uint8_t ipv6_len;   // 初始化为0
    uint8_t *dname;     // domain name
    uint8_t *ipv6;      // 初始化为NULL
    uint32_t user_data_len;  // 用户数据长度
};

struct isshe_socks_connection
{
    uint8_t status;
    int fd;
    struct bufferevent *bev;
    struct addrinfo *target_ai;
    struct isshe_socks_opts *opts;
};
/*
struct isshe_socks_connection
{
    uint8_t status;
    int fd_from_user;
    int fd_to_user;
    struct addrinfo *target_ai;
    struct isshe_socks_opts *opts;
    struct bufferevent *bev_from_user;  // 出去的流量（用户角度）
    struct bufferevent *bev_to_user;    // 进来的流量（用户角度）
};
*/

void isshe_socks_opt_init(uint8_t *buf);

void isshe_socks_opt_add(uint8_t *buf, 
    uint8_t type, uint8_t len, const void *data);

int isshe_socks_opt_len(uint8_t *buf);

void isshe_socks_opt_parse(uint8_t *buf, int buflen, 
    struct isshe_socks_opts *opts);

struct isshe_socks_connection *isshe_socks_connection_new();

void isshe_socks_connection_free(
    struct isshe_socks_connection *isc);

struct bufferevent *isshe_socks_connect_to_next(
    struct event_base *evbase,
	struct sockaddr *ai_addr,
    socklen_t ai_addrlen,
    uint16_t port);

#endif