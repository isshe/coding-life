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

怎么入手呢？先看看是怎么共享内存是定义的吧。

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
        \- TODO
    \- if typ == 4 then: 值的类型只能是字符串(4)、数字(3)、布尔值(1)，如果是字符串时，会检查返回的值的指针是否和传进去的一致，如果不一致，表示是值太大，动态分配的内存，复制成 Lua 字符串后，会调用 C.free() 进行释放。
```


### ngx.shared.DICT.set


### ngx.shared.DICT.lpush

列表系列的接口则还是通过 C 接口注入的方式。

```
ngx_http_lua_inject_shdict_api
```

## 总结
