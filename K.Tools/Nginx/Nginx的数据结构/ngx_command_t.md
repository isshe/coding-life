
struct ngx_command_s (ngx_command_t)
---

> src/core/ngx_conf_file.h

```c
struct ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};
```

定义 1 个配置指令。

- name：在配置文件中使用的指令名称，如 listen。
- type：按位的标志，指定指令接受的参数数量、类型和出现的上下文。
- set：定义一个处理程序来处理指令并将解析后的值存储到相应的配置中。
- conf：定义传递给目录处理程序的配置结构。(?)
    - 核心模块只有全局配置，并通过设置 NGX_DIRECT_CONF 标志来访问它。
    - HTTP、Stream 或 Mail 这样的模块创建了配置的层次结构。
- offset：定义模块配置结构中保存此指令值的字段的偏移量。
- post：用于向主处理程序传递数据或者定义主处理程序完成后调用的处理程序。
