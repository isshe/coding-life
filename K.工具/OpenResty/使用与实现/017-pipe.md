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


### wait —— 等待当前子流程退出

- 语法：` ok, reason, status = proc:wait()`

- 作用：等待当前子流程退出，包括超时退出。

- 返回值：
  - 成功：ok 值为 true
  - 失败：ok 值为 false，reason 为一个字符串，如 "exit" 或 "signal"。
    - exit 时：status 将为 exit 的状态码。
    - signal 时，status 将为信号码。

### kill —— 向子流程发送一个信号。

- 语法：`ok, err = proc:kill(signum)`

- 作用： 向子流程发送一个信号。

- 参数：
  - signum：信号量数字。可以使用  lua-resty-signal 的 signum() 来获取信号量名称对应的信号量值。

- 返回值：
  - 成功：true
  - 失败：nil 和 错误字符串。如果是已经退出的子流程，则错误字符串为 "exited"。

### shutdown —— 关闭当前子流程的 stderr、stderr 或 stdin

- 语法：`ok, err = proc:shutdown(direction)`

- 作用：关闭子流程的 stderr、 stderr 或 stdin。

- 参数：
  - direction：stderr、stdout、stdin。

- 返回值：
  - 成功：true
    - 如果关闭一个轻线程正在等待的方向(stderr/stdout/stdin)，则返回 true。
  - 失败：nil 和错误字符串。
    - 如果指定了 merge_stderr，并且对 stderr 调用 shutdown，则返回 nil 和 "merged to stdout"。
    - 如果子流程已经退出，则返回 nil 和 "closed"。

### write —— 向子流程的 stdin 写入数据

- 语法：`nbytes, err = proc:write(data)`

- 作用：向子流程的 stdin 写入数据。

- 参数：
  - data：需要写入的数据。

- 返回值：
  - 成功：nbytes 表示成功写入的数据长度。
  - 失败：nil 和错误字符串。
    - 如果多个协程同时向当前子流程进行写入，则只有第一个成功，后续的调用都会返回 "pipe busy writing" 错误。
    - 如果写操作被 shutdown 了，将返回 nil 和 "aborted"。
    - 如果写已经退出的子流程，将返回 nil 和 "closed"。

### stdout_read_*、stderr_read_* —— 从 stderr 或 stdout 读取数据。

- 语法：

```
data, err, partial = proc:stderr_read_all()
data, err, partial = proc:stdout_read_all()
data, err, partial = proc:stderr_read_line()
data, err, partial = proc:stdout_read_line()
data, err, partial = proc:stderr_read_bytes(len)
data, err, partial = proc:stdout_read_bytes(len)
data, err = proc:stderr_read_any(max)
proc:stdout_read_any(max)
```

比较简单，不再详细说明，可参考此文档：https://github.com/openresty/lua-resty-core/blob/master/lib/ngx/pipe.md#stderr_read_all


## 实现

相关 API 并不是通过 lua-nginx-module 注入的方式来实现。详见：

- [pipe.lua](https://github.com/openresty/lua-resty-core/blob/master/lib/ngx/pipe.lua)

### 创建 pipe 对象

```
- pipe_spawn: 这部分是 Lua 代码，进行各种参数检查。
    \- ngx_http_lua_ffi_pipe_spawn: 创建 pipe 对象
        \- ngx_create_pool: 创建内存池
        \- pipe: 创建 3 个 pipe：对应 stdin、stdout、stderr；stderr 根据参数按需创建。
        \- fork: fork 进程
            \- 如果是子进程(fork 返回值是 0)
                \- CPU_SET: 重新设置进程的 CPU 亲缘性
                \- sigaction: 重置已忽略信号的处理程序成 default(SIG_DFL)
                \- sigprocmask(SIG_SETMASK, &set, NULL): 重置信号掩码
                \- ngx_close_socket(ls[i].fd): 关闭监听的套接字
                \- close(out[0]): 关闭不需要的管道套接字，如 stdin 的写（对子进程来说）；stdout 的读。
                \- dup2(in[0], STDIN_FILENO): 重定向标准输入、标准输入。
                \- close(in[0]): 关闭多余的套接字，如 stdin 的读。在 dup2 后，in[0] 会有 2 份，一份描述符是原来的 in[0]，另一份描述符是 STDIN_FILENO，因此关闭多余的 in[0]。
                \- execvpe/ngx_http_lua_execvpe/execvp：执行想要执行的命令(程序)

            \- 如果是父进程（fork 返回值是子进程的 ID）
                \- close: 关闭不需要的文件描述符，如标准输入的读，标准输出的写
                \- ngx_nonblocking: 把没关闭的文件描述符设置为非阻塞
                \- ngx_rbtree_insert: 设置好 pipe 对象，然后加入到红黑树中。
```

这个函数主要做了以下事情：

- 创建 2 或 3 个管道(pipe)，如果使用了 merge_stderr 则 2 个，否则 3 个。
- 然后 fork 进程
- 在子进程中关闭不要的 fd，并把标准输入和标准输出重定向到对应管道中
- 准备工作完成后，子进程执行执行命令
- 父进程也关闭不要的 fd，设置没关闭的 fd 成非阻塞，然后加入到红黑树中，进行管理。


### 向子流程的 stdin 写入数据

```
- proc_write: Lua 接口
    \- local rc = C.ngx_http_lua_ffi_pipe_proc_write: 进行写操作
    \- co_yield(): 如果前面调用的返回值是 FFI_AGAIN，则 yield 让出执行权
    \- rc = C.ngx_http_lua_ffi_pipe_get_write_result: 获取写结果
```
