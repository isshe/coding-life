```c
// 值的类型
enum {
	SHDICT_TNIL = 0, /* nil */
	SHDICT_TBOOLEAN = 1, /* bool */
	SHDICT_TNUMBER = 3, /* number */
	SHDICT_TSTRING = 4, /* string */
	SHDICT_TLIST = 5, /* list，这是一个特殊的双端队列类型 */
};

// 这个结构总是和 ngx_rbtree_node_t 交集在一起
// [...parent][color][value_type...]
// [...parent] 是 ngx_rbtree_node_t 的部分
// [value_type...] 是 ngx_http_lua_shdict_node_t 的部分
typedef struct {
    u_char                       color;
    uint8_t                      value_type;        // 值的类型
    u_short                      key_len;           // 键的长度
    uint32_t                     value_len;         // 值的长度
    uint64_t                     expires;           // 过期时间
    ngx_queue_t                  queue;             // LRU 队列节点
    uint32_t                     user_flags;        // 用户设置的标志
    u_char                       data[1];           // 键字符串的第一个字符，key 其实就存在这里（从这里开始），通过和 key_len 表示完整的 key
} ngx_http_lua_shdict_node_t;


typedef struct {
    ngx_queue_t                  queue;             // 在双端队列上的位置
    uint32_t                     value_len;         // 值长度
    uint8_t                      value_type;        // 值类型 string 或者是 number
    u_char                       data[1];           // 值的第一个字符，和 value_len 表示完整的值
} ngx_http_lua_shdict_list_node_t;


typedef struct {
    ngx_rbtree_t                  rbtree;           // 红黑树的根节点
    ngx_rbtree_node_t             sentinel;         // 红黑树的哨兵节点
    ngx_queue_t                   lru_queue;        // LRU 队列的哨兵节点
} ngx_http_lua_shdict_shctx_t;


typedef struct {
    ngx_http_lua_shdict_shctx_t  *sh;
    ngx_slab_pool_t              *shpool;           // slab 内存池
    ngx_str_t                     name;             // shared dict 名称
    ngx_http_lua_main_conf_t     *main_conf;        // main 配置
    ngx_log_t                    *log;              // log 对象
} ngx_http_lua_shdict_ctx_t;


typedef struct {
    ngx_log_t                   *log;               // log 对象
    ngx_http_lua_main_conf_t    *lmcf;              // main 配置
    ngx_cycle_t                 *cycle;
    ngx_shm_zone_t               zone;              // 共享内存对象
} ngx_http_lua_shm_zone_ctx_t;
```
