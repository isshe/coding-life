# OpenResty 共享内存

目的：

- 相关 Lua API 如 `ngx.shared.DICT.get` 是如何使用的？
- 如何定义 lua_shared_dict 的，lua_shared_dict 做了哪些工作？
- 都有哪些相关的数据结构，都是什么作用？
- 详细跟踪各类 Lua 接口（set、get 每类一个）的实现，了解共享内存的设计/实现细节。
- 这些 Lua 接口是否是原子的？是否需要考虑竞争问题？原子性是如何保证的？

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
    \- if (lmcf->shdict_zones == NULL): 检查 main 配置，是否已经有这个存储所有 shared dict 的数组了，如果没有，就分配并初始化。
    \- name = value[1]: 获取并检查 shared dict 名称
    \- size = ngx_parse_size(&value[2]): 获取并检查 shared dict 大小，单位字节。
```

### ngx.shared.DICT.get


### ngx.shared.DICT.set

## 总结
