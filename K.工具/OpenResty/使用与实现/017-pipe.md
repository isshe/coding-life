管道
---

> ngx_http_lua_pipe_init
> 目前了解到主要是用于实现 lua-resty-shell 之类的库，后续探究一番。

目的：

- 有哪些使用场景？
- 如何使用？在 OR 里面是如何使用的？
- 由操作系统 shell 执行的字符串形式的命令行，会阻塞吗？
- 如何实现的？

## 使用

### spawn —— 创建对象

- 语法：`proc, err = pipe_module.spawn(args, opts?)`

- 作用：创建并返回一个新的子流程实例。

- 参数：
    - args：数组形式的命令或者是字符串形式的命令。
      - 数组形式示例：{"ls", "-l"}
      - 字符串形式示例："ls -l"。当使用此模式时，命令将由操作系统 shell 执行，就像 os.execute 一样。
    - opts：
      - merge_stderr：如果是 `true`，将合并 stderr 和 stdout 的输出，相当于 `2>&1`。
      - buffer_size：读取操作的缓冲区大小，默认是 4096 字节。
      - environ：环境变量。如果当前系统不支持环境变量，则返回 nil 和 "environ option not supported"。示例：`{"PATH=/tmp/bin", "CWD=/tmp/work"}`
      - write_timeout：写超时时间，单位是毫秒。默认是 10000，设置为 0 则永不超时。
      - stdout_read_timeout：标准输出读超时，单位是毫秒，默认是 10000，设置为 0 则永不超时。
      - stderr_read_timeout：标准错误读超时，单位是毫秒，默认是 10000，设置为 0 则永不超时。
      - wait_timeout：等待超时，单位是毫秒，默认是 10000，设置为 0 则永不超时。

- 返回值：
  - 失败：返回 nil 和错误信息
  - 成功：返回 pipe 对象





