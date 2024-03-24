


```c
typedef struct ngx_listening_s  ngx_listening_t;

struct ngx_listening_s {
    // socket 套接字句柄
    ngx_socket_t        fd;

    // 监听 sockaddr 地址
    struct sockaddr    *sockaddr;

    // sockaddr 地址长度
    socklen_t           socklen;    /* size of sockaddr */

    // 存储 IP 地址的字符串 addr_text 最大长度
    // 即它指定了 addr_text 所分配的内存大小
    size_t              addr_text_max_len;

    // 以字符串形式存储 IP 地址
    ngx_str_t           addr_text;

    // 套接字类型，type 是 SOCK_STREAM 表示 TCP
    int                 type;

    // TCP 实现监听时的 backlog 队列：
    // 表示允许正在通过三次握手建立 TCP 连接单还没有任何进程开始处理的连接最大个数
    int                 backlog;

    // 内核中对于这个套接字的接收缓冲区大小
    int                 rcvbuf;

    // 内核中对于这个套接字的发送缓冲区大小
    int                 sndbuf;
#if (NGX_HAVE_KEEPALIVE_TUNABLE)
    int                 keepidle;
    int                 keepintvl;
    int                 keepcnt;
#endif

    // 当新的 TCP 连接成功建立后的处理方法
    /* handler of accepted connection */
    ngx_connection_handler_pt   handler;

    // 实际上框架并不使用 servers 指针，它更多是作为一个保留指针，
    // 目前主要用于 HTTP 或者 mail 等模块，用于保存当前监听端口对应着的所有主机名
    void               *servers;  /* array of ngx_http_in_addr_t, for example */

    // log 和 logp 都是可用的日志对象的指针
    ngx_log_t           log;
    ngx_log_t          *logp;

    // 如果为新的 TCP 连接创建内存池，则内存池的初始大小应该是 pool_size
    size_t              pool_size;

    // 注：此解释是 post_accept_timeout（已删除）的：TCP_DEFER_ACCEPT 选项将在建立 TCP 连接成功且接收到用户的请求数据后，才向对监听套接字感兴趣的进程发送事件通知，而如果连接建立成功后 post_accept_timeout 秒后仍然没有收到用户数据，则内核直接丢弃连接
    // post_accept_buffer_size：连接建立后临时缓冲区的大小，windows 专用。
    /* should be here because of the AcceptEx() preread */
    size_t              post_accept_buffer_size;

    // 前一个 ngx_listening_t 结构，多个结构之前由 previous 指针组成单链表
    ngx_listening_t    *previous;

    // 当前监听句柄对应着的 nginx_connection_t 结构体
    ngx_connection_t   *connection;

    ngx_rbtree_t        rbtree;
    ngx_rbtree_node_t   sentinel;

    ngx_uint_t          worker;

    // 1：当前监听句柄有效，且执行 ngx_init_cycle 时不关闭监听端口；0：正常关闭。该标志位框架代码会自动设置
    unsigned            open:1;

    // 1：使用已有的 ngx_cycle_t 来初始化新的 ngx_cycle_t 结构体时，不关闭原先打开的监听端口（这对运行中升级程度很有用）
    // 0：正常关闭曾经打开的监听端口。该标志位框架代码会自动设置，参见 ngx_init_cycle 方法
    unsigned            remain:1;

    // 1：跳过设置当前 ngx_listening_t 结构体中的套接字
    // 0：正常初始化套接字。该标志位框架代码会自动设置
    unsigned            ignore:1;

    // 表示是否已经绑定，目前该标志位没有使用
    unsigned            bound:1;       /* already bound */

    // 表示当前监听句柄是否来自前一个进程。
    // 1：来自前一个进程。
    unsigned            inherited:1;   /* inherited from previous process */

    // 目前未使用
    unsigned            nonblocking_accept:1;

    // 1：当前结构体对应的套接字已经监听
    unsigned            listen:1;

    // 表示套接字是否阻塞，目前该标志位没有意义
    unsigned            nonblocking:1;

    // 目前该标志位没有意义
    unsigned            shared:1;    /* shared between threads or processes */

    // 1：表示 Nginx 会将网络地址转变成字符串形式的地址
    unsigned            addr_ntop:1;

    // 是否监听的是任意地址，如 IPv4:0.0.0.0 (INADDR_ANY)
    unsigned            wildcard:1;

#if (NGX_HAVE_INET6)
    unsigned            ipv6only:1;
#endif
    // 端口重用，允许多个进程/线程 bind/listen 相同的 IP/PORT，提升了新链接的分配性能
    // reuseport 也是内核解决“惊群问题”的优秀方案，详见：https://wenfh2020.com/2021/10/12/thundering-herd-tcp-reuseport/
    unsigned            reuseport:1;
    unsigned            add_reuseport:1;
    unsigned            keepalive:2;

    unsigned            deferred_accept:1;
    unsigned            delete_deferred:1;
    unsigned            add_deferred:1;
#if (NGX_HAVE_DEFERRED_ACCEPT && defined SO_ACCEPTFILTER)
    char               *accept_filter;
#endif
#if (NGX_HAVE_SETFIB)
    int                 setfib;
#endif

#if (NGX_HAVE_TCP_FASTOPEN)
    int                 fastopen;
#endif

};
```
