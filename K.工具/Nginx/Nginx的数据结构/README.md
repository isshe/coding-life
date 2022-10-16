
Nginx 的数据结构
---

此文档的目的：简要记录各种数据结构的适用场景，后续需要用到时，再细查。

- ngx_list_t：链表容器，在 Nginx 中使用频繁，例如用于存储 HTTP 头部。
    * ngx_list_part_t: 用于描述 ngx_list_t 中的元素，是一个数组，拥有连续的内存。
![](ngx_list_t.png)
- ngx_table_elt_t：用于存储 key/value 对。也可以是 ngx_hash_t 结构中的成员。
- ngx_buf_t：Nginx 处理大数据的关键数据结构。它既应用于内存数据也应用于磁盘数据。
- ngx_chain_t：与 ngx_buf_t 配合使用的链表结构。
- ngx_command_t：用于定义一个配置指令。
- ngx_cycle_t：存储由特定配置创建的运行时上下文，每次 reload 都重新创建一个 cycle，新 cycle 创建成功后删除旧的 cycle。当前的 cycle 被 ngx_cycle 引用并被 worker 进程继承。
    - 详见：http://nginx.org/en/docs/dev/development_guide.html#cycle
- ngx_listening_t：用于定义监听端口，每个 ngx_listening_t 表示 1 个监听端口。
