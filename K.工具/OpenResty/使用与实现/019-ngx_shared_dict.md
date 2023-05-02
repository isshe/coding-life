# OpenResty 共享内存

目的：

- 相关 Lua API 如 `ngx.shared.DICT.get` 是如何使用的？
- 如何定义 lua_shared_dict 的，lua_shared_dict 做了哪些工作？
- 都有哪些相关的数据结构，都是什么作用？
- 详细跟踪各类 Lua 接口（set、get 每类一个）的实现，了解共享内存的设计/实现细节。
- 这些 Lua 接口是否是原子的？是否需要考虑竞争问题？原子性是如何保证的？
- ngx_http_lua_shdict_shctx_t、ngx_http_lua_shdict_ctx_t、ngx_http_lua_shm_zone_ctx_t 之间什么关系？

## 使用

使用示例：

```lua
http {
    lua_shared_dict dogs 10m;
    server {
        location / {
            content_by_lua_block {
                -- 设置
                local dogs = ngx.shared.dogs
                dogs:set("Jim", 8)

                -- 获取
                local jim_age = dogs:get("Jim")
                ngx.say(jim_age)

                -- 替换
                dogs:replace("Jim", 9)

                -- 递增, value = 10
                dogs:incr("Jim")

                -- 更新/设置超时
                dogs:expire("Jim", 10)

                -- 获取 TTL
                dogs:ttl("Jim")

                -- 删除
                dogs:delete("Jim")
            }
        }
    }
}
```

以下是所有 Lua Api 的作用：

> 详细用法见 [lua-nginx-module](https://github.com/openresty/lua-nginx-module#ngxshareddict)。

- get: 获取指定 key 的 value。如果不存在，则返回 nil。
- get_stale: 获取指定 key 的 value，即使已经过期，也正常返回。第三个返回值标识是否过期。
- set: 增加或更新 shared dict 中的键值对。
- safe_set: 如果 shared dict 满了，则返回 nil 和 "no memory"，不使用 LRU 算法自动淘汰项目。
- add: 增加键值对到 shared dict 中。如果已经存在，则返回 nil 和 "exists"
- safe_add: 如果 shared dict 满了，则返回 nil 和 "no memory"，不使用 LRU 算法自动淘汰项目。
- replace: 替换 shared dict 中指定键的值，如果键不存在，返回 "not found"。
- delete: 删除 shared dict 中指定的键值对。
- incr: 递增指定的数字型键值对的值，可以指定初始值，如果 shared dict 中没有对应的的键时，使用初识值进行初始化。
- lpush: 把数字型或字符型的值插入到指定键的列表的左边（头部）。
- rpush: 把数字型或字符型的值插入到指定键的列表的右边（尾部）
- lpop: 从指定键的列表的左边（头部）弹出一个值。
- rpop: 从指定键的列表的右边（尾部）弹出一个值。
- llen: 获取指定列表的长度。
- ttl: 获取指定键值的 TTL。
- expire: 设置指定键值的过期时间。
- flush_all: 清空 shared dict 中所有键值对（设置为过期，不是立即释放内存）
- flush_expired: 释放过期的键值对的内存。
- get_keys: 获取 shared dict 中所有的键。
- capacity: 返回 shared dict 的容量（通过 lua_shared_dict 指令指定），单位为字节。
- free_space: 返回 shared dict 的剩余空间，单位字节。

## 实现

怎么入手呢？先看看共享内存是怎么定义的吧。

### 定义共享内存（lua_shared_dict）

指令定义：

```
    { ngx_string("lua_shared_dict"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE2,
      ngx_http_lua_shared_dict,
      0,
      0,
      NULL },
```

可以看到 ngx_http_lua_shared_dict 是这个处理函数。

```lua
- ngx_http_lua_shared_dict
    \- if (lmcf->shdict_zones == NULL): 检查 main 配置，是否已经有这个存储所有 shared dict 的数组了，如果没有，就分配并初始化
    \- name = value[1]: 获取并检查 shared dict 名称
    \- size = ngx_parse_size(&value[2]): 获取并检查 shared dict 大小，单位字节
    \- ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_lua_shdict_ctx_t)): 分配 shared dict 上下文结构，并设置好 shared dict 的名称、main 配置、log 配置等
    \- zone = ngx_http_lua_shared_memory_add(): 分配共享内存
        \- if (lmcf->shm_zones == NULL): 在 main 配置中维护的 ngx_shm_zone_t* 数组
        \- zone = ngx_shared_memory_add(cf, name, (size_t) size, tag): 调用 nginx 核心的接口来创建共享内存
        \- ngx_memcpy(&ctx->zone, zone, sizeof(ngx_shm_zone_t)): 复制 zone，两份 zone 分别设置不同的 data 和 init 字段。两个 zone 的关系是：zone2 = zone1->data->zone。
        \- zp = ngx_array_push(lmcf->shm_zones): 加入到 shm_zones 数组中
        \- zone->init = ngx_http_lua_shared_memory_init: 设置初始化函数
            \- if (zone->init(zone, odata) != NGX_OK): 实际调用的是 ngx_http_lua_shdict_init_zone
            \- if (lmcf->shm_zones_inited == lmcf->shm_zones->nelts && lmcf->init_handler && !ngx_test_config): 初始化完成，并且不是测试，就执行后续的 init_handler 操作
                \- rc = lmcf->init_handler(ctx->log, lmcf, lmcf->lua)
        \- zone->data = ctx: 设置数据字段
    \- zp = ngx_array_push(lmcf->shdict_zones): 加入到 shdict_zones 数组中
    \- zone->init = ngx_http_lua_shdict_init_zone: 设置初始化函数
        \- ctx->sh = ngx_slab_alloc(ctx->shpool, sizeof(ngx_http_lua_shdict_shctx_t))
        \- ngx_rbtree_init(&ctx->sh->rbtree, &ctx->sh->sentinel,
ngx_http_lua_shdict_rbtree_insert_value): 初始化红黑树，也就是 shared dict 中的键值对是通过红黑树来组织的，查找插入都会比较快。
        \- ngx_queue_init(&ctx->sh->lru_queue): 初始化 lru 队列，同样是用来管理键值对的，空间不足时，就淘汰旧的键值对。
        \- ctx->shpool->log_ctx = ngx_slab_alloc(ctx->shpool, len): 初始化日志消息
    \- zone->data = ctx: 设置数据字段
```

这个函数主要做了以下事情：

- 获取并检查名称、大小参数
- 分配 shared dict 上下文结构
- 分配共享内存、设置相关初始化函数
- 加入到 main 配置的 shdict_zones 数组中

### ngx.shared.DICT.get

get 接口的实现，在 2019.8.1（commit ID: 947fa0088b7a0387f43eb016436a2e4462eb5746）中，把接口注入的方式改成了 FFI 的方式。这些 FFI C 接口在 lau-nginx-module 项目中，Lua 接口在 lua-resty-core 项目中。

```
- shdict_get
    \- check_zone: 检查 zone（共享内存）是否有效
    \- if key_len > 65535 then: 检查 key 的类型和长度。长度不能大于 65535
    \- local size = get_string_buf_size(): 获取一块空间来存储值，默认空间大小是 4096 字节
    \- ngx_http_lua_ffi_shdict_get: 获取值
        \- hash = ngx_crc32_short(key, key_len): 使用 CRC 算法生成一个 HASH 值
        \- ngx_shmtx_lock(&ctx->shpool->mutex): 加锁
        \- ngx_http_lua_shdict_expire(ctx, 1): 删除 1~2 个过期的数据条目，如果传值是 1，则不管过没过期，一定会删除 1 个数据条目。
            \- while (n < 3): 最多删除 3 个数据，n 是传进来的
                \- if (ngx_queue_empty(&ctx->sh->lru_queue)): 队列空了，直接返回
                \- q = ngx_queue_last(&ctx->sh->lru_queue): 获取队列中最旧的一个数据
                \- sd = ngx_queue_data(q, ngx_http_lua_shdict_node_t, queue): 获取数据
                \- if (n++ != 0): n 不等于 0，就需要检查过期时间，过期了才删除
                \- if (sd->value_type == SHDICT_TLIST): 如果是列表，则遍历列表，然后一个一个删除。
                \- ngx_queue_remove(q): 从 LRU 队列中删除
                \- ngx_rbtree_delete(&ctx->sh->rbtree, node): 从红黑树中删除
                \- ngx_slab_free_locked(ctx->shpool, node): 以加锁的方式从 slab 内存池中删除
        \- rc = ngx_http_lua_shdict_lookup(zone, hash, key, key_len, &sd): 进行检索
            \- while (node != sentinel): 在红黑树中进行查找，没到哨兵节点时，就一直找
                \- if (hash < node->key): 先检查 hash (前面 ngx_crc32_short 生成的)，hash 相等，才有可能是要找的 key。
                \- if (hash > node->key): 不是这个节点，往右找
                \- rc = ngx_memn2cmp(kdata, sd->data, klen, (size_t) sd->key_len): hash 值相等了，就比较 key 是否相同，相同证明就是要找的
                \- if (rc == 0): 找到了
                    \- ngx_queue_remove(&sd->queue)
                    \- ngx_queue_insert_head(&ctx->sh->lru_queue, &sd->queue): 刚访问过，放到队首去
                    \- return NGX_DONE/NGX_OK: 过期返回 NGX_DONE，没过期返回 NGX_OK
        \- *str_value_len = value.len: 根据不同的类型（字符串、数字、布尔），进行长度赋值
        \-  ngx_memcpy(*str_value_buf, value.data, value.len): 根据不同的类型（字符串、数字、布尔），进行数据复制
        \- *user_flags = sd->user_flags: 是否强制覆盖其他有效条目的标记
        \- ngx_shmtx_unlock(&ctx->shpool->mutex): 解锁
        \- *is_stale = (rc == NGX_DONE): 如果是获取 stale 的接口，则判断是否过期
    \- if typ == 4 then: 值的类型只能是字符串(4)、数字(3)、布尔值(1)，如果是字符串时，会检查返回的值的指针是否和传进去的一致，如果不一致，表示是值太大，动态分配的内存，复制成 Lua 字符串后，会调用 C.free() 进行释放。
```

这个接口主要进行了以下操作：

- 主要的参数检查，在 Lua 接口中进行
- 对内存池进行加锁（内存池级别）
- 尝试释放 1~2 个过期数据
- 然后在红黑树中进行搜索。如果找到，就把数据放到 LRU 队列首部，表示刚刚被访问过
- 最后把数据复制到传进来的内存中或者是另外申请一块内存（如果值太大），返回给 Lua 接口

### ngx.shared.DICT.set

```
- shdict_set
    \- shdict_store
    \- check_zone(zone): 检查共享内存是否有效
    \- if not exptime then: 检查 exptime、key 等参数是否合法
    \- if valtyp == "string" then: 检查值的类型，可以是 "string", "number", nil, "boolean"；如果不是这 4 种类型，就报错返回。
    \- ngx_lua_ffi_shdict_store
        \- hash = ngx_crc32_short(key, key_len): 生成 hash 值
        \- switch (value_type): 统一把值转换到 1 对变量中（str_value_buf 和 str_value_len），"number", nil, "boolean" 这三种类型才需要转换
        \- ngx_shmtx_lock(&ctx->shpool->mutex): 加锁
        \- ngx_http_lua_shdict_expire(ctx, 1): 释放 1~2 个过期数据条目
        \- rc = ngx_http_lua_shdict_lookup(zone, hash, key, key_len, &sd): 查找一下，看是否能找到
        \- if (op & NGX_HTTP_LUA_SHDICT_REPLACE): 如果是替换，并且没找到，就报错返回；否则就跳到替换的逻辑去执行
        \- if (op & NGX_HTTP_LUA_SHDICT_ADD): 如果是增加，但是找到并且没过期，就报错返回；找到但过期，就执行替换操作；没找到，就执行增加操作。
        \- else: 接下来是普通的 SET 操作
        \- if (rc == NGX_OK || rc == NGX_DONE): 找到并且没过期，或者是找到但过期
            \- if (value_type == LUA_TNIL): 如果是 nil，则表示是删除数据，跳到删除的逻辑去
            \- replace: 替换逻辑开始
                \- if (str_value_buf && str_value_len == (size_t) sd->value_len && sd->value_type != SHDICT_TLIST): 找到并且值的大小没变，就复用
                    \- ngx_queue_remove(&sd->queue)
                    \- ngx_queue_insert_head(&ctx->sh->lru_queue, &sd->queue): 插入到队首，表示刚访问过
                    \- ngx_memcpy(sd->data + key_len, str_value_buf, str_value_len): 把新值复制过去，然后解锁返回
                    \- ngx_shmtx_unlock(&ctx->shpool->mutex)
                \- else: 先删除，再插入
            \- remove: 删除逻辑开始
                    \- if (sd->value_type == SHDICT_TLIST): 如果是 list，则遍历删除列表内元素
                    \- ngx_queue_remove(&sd->queue): 从 LRU 队列中删除自身（不管是不是 list 类型）
                    \- ngx_rbtree_delete(&ctx->sh->rbtree, node): 从红黑树中删除自身
                    \- ngx_slab_free_locked(ctx->shpool, node): 从内存池中释放内存
        \- else: 数据没找到，直接新分配一个
        \- insert: 插入逻辑开始，数据不存在或者是新旧值的大小不匹配
            \- if (str_value_buf == NULL): 如果是 set(key, nil)，也就是删除操作，则直接解锁返回，因为前面已经删除了
            \- node = ngx_slab_alloc_locked(ctx->shpool, n): 分配内存
            \- if (node == NULL): 没内存了
                \- if (op & NGX_HTTP_LUA_SHDICT_SAFE_STORE): 如果是 safe_set，直接报错返回
                \- for (i = 0; i < 30; i++): 否则尝试 30 次强制释放数据，如果还是没能释放掉并申请到想要的大小的内存，就也报错返回
                    \- ngx_http_lua_shdict_expire
                    \- ngx_slab_alloc_locked
        \- allocated: 分配逻辑开始 —— 其实内存已经分配好，现在是进行数据赋值等操作
            \- sd->user_flags = user_flags: 设置超时时间，用户标记等
            \- p = ngx_copy(sd->data, key, key_len): 复制 key
            \- ngx_memcpy(p, str_value_buf, str_value_len): 复制 value
            \- ngx_rbtree_insert(&ctx->sh->rbtree, node): 加入到红黑树中
            \- ngx_queue_insert_head(&ctx->sh->lru_queue, &sd->queue): 加入到 LRU 队列的队首
            \- ngx_shmtx_unlock(&ctx->shpool->mutex): 解锁返回
```

这个接口主要进行了以下操作：

- 在 Lua 接口中进行参数检查，key、flag、超时时间等
- 在 C 接口中进行参数检查，值的类型只能是 "string", "number", nil, "boolean" 这 4 种
- 对内存池进行加锁（内存池级别），开始执行变更操作
- 尝试释放 1~2 个过期数据
- 然后在红黑树中进行搜索
  - 如果找到，并且
    - 传进来的值是 nil，表示删除，跳到删除的逻辑
    - 传进来的值不是 nil，旧的值也不是 list 类型，并且值的长度相等，说明可用复用，直接把新值复制过去即可
    - 是 list 类型，则进行遍历删除
    - 不是 list 类型，并且值的长度不相等，则删除旧的值，再分配新的空间存新的值
- 如果没找到，直接插入新的数据

### ngx.shared.DICT.lpush

列表系列的接口则还是通过 C 接口注入的方式。

```
ngx_http_lua_inject_shdict_api
```

## 总结
