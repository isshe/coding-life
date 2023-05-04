# 轻线程信号量

和 Linux 系统或者 Nginx 中的信号量类似，都是用于进程/协程同步。当值小于 0 时，表示需要等待资源。
不过 Nginx 中的信号量是会导致进程在等待资源时休眠的，OpenResty 的信号量（后续简称信号量）是不是也是一样呢？我们来探究一番。

目的：

- 如何使用信号量？
- 信号量是如何实现的？
- 信号量是否会导致如进程休眠之类的问题？
- 不同进程的协程是否能使用信号量？

# 使用

Lua 接口的使用都比较简单，我们直接通过一个示例来说明。

```lua
-- 引入 ngx.semaphore 模块
local semaphore = require "ngx.semaphore"

-- 新建一个 semaphore 对象
-- 此时默认资源是 0 个
-- 也可以传递参数指定信号量初始有多少资源
local sema = semaphore.new()

-- 获取资源数量
ngx.say("main thread: count: ", sema:count())  -- count: 0

-- 新协程的处理函数
local function handler()
    ngx.say("sub thread: waiting on sema...")

    -- 等待 1 个资源；超时时间是 1 秒，如果资源数量 >= 0，立即返回，如果拿不到（数量 < 0），会 yield。
    -- 相当于锁操作中的 加锁（lock）操作。
    -- 参数中的 1 表示 1 秒；最小可以为 0.001 秒
    local ok, err = sema:wait(1)  -- wait for a second at most
    if not ok then
        ngx.say("sub thread: failed to wait on sema: ", err)
    else
        ngx.say("sub thread: waited successfully.")
    end
end

-- 新建协程并立即跑 handler
local co = ngx.thread.spawn(handler)

ngx.say("main thread: sleeping for a little while...")

ngx.sleep(0.1)  -- wait a bit

ngx.say("main thread: posting to sema...")
ngx.say("main thread: count: ", sema:count())  -- count: -1

-- 释放一个资源，这里的参数 1 表示释放的资源数量。
sema:post(1)

ngx.say("main thread: count: ", sema:count())  -- count: 0
ngx.say("main thread: end.")
```

示例输出如下：

```lua
main thread: count: 0
sub thread: waiting on sema...
main thread: sleeping for a little while...
main thread: posting to sema...
main thread: count: -1
main thread: count: 0
main thread: end.
sub thread: waited successfully.
```

## 实现

不像前面文章中的做法，这里用到的 Lua 接口不是通过接口注入的方式进行的，而是用的 Lua 模块，名称为 semaphore.lua。

### 创建对象

```lua
- new
    \- if n < 0 then：检查参数，如果小于 0，就报错退出。
    \- ngx_lua_ffi_sema_new(psem, n, errmsg)：调用 C 函数创建对象。
        \- ngx_http_lua_alloc_sema：分配内存，会一次分配一批，然后从中取 1 个，其他的放着后续能快速取，不用每次都分配。
    \- ffi_gc(sem, ngx_lua_ffi_sema_gc)：绑定 gc 函数，在 sem 被释放时，调用 gc 函数。
```

这个函数主要进行以下工作：

- 检查参数合法性
- 分配内存存储 sema 对象
    - ngx_lua_ffi_sema_new 在 http 子系统中对应 ngx_http_lua_ffi_sema_new，在 stream 子系统中对应 ngx_stream_lua_ffi_sema_new。
- 绑定清理函数

## 获取资源

```lua
- wait
    \- if milliseconds < 0 then：检查参数，第二个参数是等待超时时间，单位是秒，最小支持 0.001 秒，也就是 1 毫秒。
    \- ngx_lua_ffi_sema_wait：
        \- ngx_http_lua_ffi_check_context：检查上下文是否能 yield
        \- ngx_queue_empty：如果等待队列为空并且还有资源直接返回成功
        \- if (wait_ms == 0)：如果要求不等待，也直接返回
        \- ngx_add_timer；接下来就是需要等待了，增加 timer，超时时间为调用接口时指定的时间。
        \- ngx_queue_insert_tail：把当前协程插入到信号量等待队列末尾。
    \- ok, err = co_yield()：不能在这里使用尾调用形式，因为可能需要当前函数调用的激活记录来保存对信号量对象的引用，以防止它过早地被 GC 处理。
```

这个调用主要进行以下工作：

- 检查参数合法性
- 获取资源，能获取到就直接返回，获取不到就看要不要等待，不等就直接返回超时；等就加入到等待队列中，然后 yield 出去，等待 post 操作唤醒。

## 释放资源

```lua
- post
    \- if type(self) ~= "table" or type(self.sem) ~= "cdata" then：检查 self 对象
    \- if num < 1 then：检查参数是否是 >= 1
    \- ngx_lua_ffi_sema_post
        \- sem->resource_count += n：把资源加 n
        \- ngx_post_event((&sem->sem_event), &ngx_posted_events)：如果信号量中的等待队列不为空，就把事件加到全局的 ngx_posted_events 队列中，在后续的事件循环中进行唤醒新的协程。
```

这个函数也是非常简单，主要进行以下工作：

- 检查参数合法性
- 把资源数量增加 n
- 如果有等待队列，就把事件增加到 ngx_posted_events 中，后续处理。

## 总结

- 信号量是如何实现的？

答：还是依赖 Lua 的协程和 Nginx 定时器等。新建一个信号量对象，其中记录信号量的信息；然后使用 wait 和 post 来增减资源。

- 信号量是否会导致如进程休眠之类的问题？

答：不会。使用的 Lua 协程，在没有资源时，会 yiled 出去，给其他协程执行。

- 不同进程的协程是否能使用信号量？

答：不能。可以在不同上下文、请求中进行使用，但是要求是同一个 worker 进程。如需在不同进程间同步，可以使用：lua-resty-lock。
