[TOC]

Examples
---

# 正则表达式
* ngx.re.match
```lua
local str = "abc-123"
local res = ngx.re.match(str, [[\d{3}]], "jo")
```

* ngx.re.find
> from, to, err = ngx.re.find(subject, regex, options, ctx, nth)

```
local str = "abc-123"
local from, to, err = ngx.re.find(str, [[\d{3}]], "jo")
```

* ngx.re.sub
> newstr, n, err = ngx.re.sub(subject, regex, replace, options)

# ngx.config
获取系统信息
```lua
ngx.config.debug            -- 是否是 debug 版本
ngx.config.prefix()         -- 安装目录
ngx.config.nginx_version    -- nginx 版本
ngx.config.ngx_configure()  -- 编译选项
ngx.config.subsystem        -- 是否是子系统
ngx.config.ngx_lua_version  -- 所在子系统版本号
```

# 时间/日期
```lua
ngx.today()     -- 本地时间, 'yyyy-mm-dd'
ngx.localtime() -- 本地时间, 'yyyy-mm-dd hh:mm:ss'
ngx.utctime()   -- UTC 时间, 'yyyy-mm-dd hh:mm:ss'
ngx.now()       -- 精确到毫秒
ngx.time()      -- 精确到秒

-- 时间戳与字符串转换
ngx.http_time(ngx.time())
ngx.cookie_time(ngx.time())
ngx.parse_http_time(ngx.http_time(ngx.time()))

-- 更新时间
ngx.update_time()

-- 睡眠
ngx.sleep(0.1)
```

# 编解码/转义
## base64
```lua
local str = "1234"
local enc = ngx.encode_base64(str)
local dec = ngx.decode_base64(enc)

local b64 = require("ngx.base64")
local str = "=>?@"
local url_enc = b64.encode_base64url(str)       -- 适合 URL 的 base64 编码
local url_dec = b64.decode_base64url(url_enc)

-- 转义
local uri = "a + b = c #!"
local enc_uri = ngx.escape_uri(uri)
local dec_uri = ngx.unescape_uri(enc_uri)

-- 参数编解码

```
x`x`x`
# ffi
> TODO
>


# ngx.req
注意这些函数/语句，通常需要在请求中进行。非 init_by_lua/init_worker_by_lua。
```lua
ngx.req.http_version()  -- 获取 http 版本
ngx.req.get_method()    -- 获取请求方法
ngx.req.set_uri()       -- 改写请求行里的 URI

ngx.req.get_headers() 	-- 获取请求头部
-- ngx.header.content_length = 0 	-- 设置响应头部

-- 参数
ngx.req.get_uri_args(max_args)          -- 获取 URI 里的参数
ngx.req.read_body()
local args = ngx.req.get_post_args(10)  -- 获取 post 参数

local args = {a = 1, b = {'#', '%'}}
ngx.req.set_uri_args(args)              -- 设置 uri 参数

-- 读取请求体
ngx.req.read_body()
local data = ngx.req.get_body_data()
if data then
    -- do something
else
	  local filename = ngx.req.get_body_file()
    local f = io.open(filename, "r")
    data = f:read("*a") 	-- 阻塞操作
    f:close()
end

-- 手动收数据
local sock, err = ngx.req.socket()
local len = tonumber(ngx.var.http_content_length)
local data = sock:receive(len)

-- 手动发数据
ngx.header.content_length = len
ngx.send_headers()
ngx.flush(true)		-- 先清空缓冲区
local sock, err = ngx.req.socket(true)	-- 默认只读，加 true 可写
sock:send(data)
```



# 流程控制

```lua
ngx.redirect(uri, status)	-- 标准的 301、302 重定向跳转，默认 302
ngx.exec(uri, args)	-- 跳转到内部的其他 location
ngx.exit(status)	-- 立即结束请求的处理
ngx.eof()		-- 发送 EOF 标记，后续不会再发送数据了

-- 断连检测
local function cleanup()
	-- do something  
end
local ok, err = ngx.on_abort(cleanup)
```



# 子请求访问后端

```lua
-- nginx config
location = /hello {
  proxy_set_header Host $host
  proxy_pass http://xxx.com/xxx
}

-- lua
local capture = ngx.location.capture
local res = capture('/hello')
if res.status ~= ngx.HTTP_OK then
  --
end
if res.truncated then
  --
end
```



# cosocket

```lua
-- 创建对象
sock = ngx.socket.tcp()

-- 超时设置
sock:settimeout(time)
sock:settimeouts(connect_timeout, send_timeout, read_timeout)

-- 建立连接
ok, err = sock:connect(host, port)
if not ok then
  --
end

-- 集成的connect：创建对象并连接
local sock, err = ngx.socket.connect(host, port)

-- 复用连接
-- 每个后端服务器都会有一个与之对应的连接池，key是"host:port"
ok, err = sock:setkeepalive(timeout, size) 	-- 创建并放入连接池

count, err = sock:getreusedtimes() 	-- 获取复用次数

bytes, err = sock:send(data)	-- 发送数据

data, err, partial = sock:receive(opt) 	-- 接收数据
-- opt:
-- *l: 接收一行，\r 结束
-- *a: 接收所有，直到连接关闭

iterator = sock:receiveuntil(pattern) 	-- 指定模式接收数据，读取完毕返回 nil



ok, err = sock:close()

-- 对于 stream 子系统
ok, err = sock:shutdown("send")	-- 单向关闭连接，禁止写，允许读

```



# DNS-域名解析

```lua
-- Nginx 自带
resolver 8.8.8.8 valid=30s;

-- lua-resty-dns
local resolver = require("resty.dns.resolver")

r, err = resolver:new(opts) 	-- 创建DNS 解析对象
-- opts:
-- nameservers: 服务器地址数组
-- retrans: 重试次数，默认 5
-- timeout：单次超时，默认 2
-- no_recurse：递归标记，默认 false，允许递归查询。

answers, err = r:query(name, options, tries)
if not answers then
  --
end
if answers.errcode then
  --
end

-- 缓存
local lrucache = require("resty.lrucache")
local dns, err = lrucache.new(100)
-- ...
dns:set(domain, addr, ttl)
```



# upstream

```lua
local upstream = require("ngx.upstream")

local names = upstream.get_upstreams()	-- 获取 upstream 块的名字
for i, upstream_name in ipairs(names) do
	local servers, err = upstream.get_servers(upstream_name)	-- 获取 server 信息
  local peers, err = upstream.get_primary_peers(upstream_name)	-- 主服务器信息
  local peers, err = upstream.get_backup_peers(upstream_name)		-- 备服务器信息
  local ok, err = upstream.set_peer_down(upstream_name, is_backup, peer_id, down_value)	-- 上线或下线
end

-- 获取当前上游（只能在 filter 阶段使用）
local name = upstream.current_upstream_name()	-- 当前正在使用的 upstream 块
local addr = ngx.var.upstream_addr 						-- 反向代理的上游 IP 地址
```



# balancer

```lua
local balancer = require("ngx.balancer")

balancer.set_timeouts(1, 0.5, 0.5) 	-- 设置后端的连接、读写超时，单位秒
balancer.set_more_tries(2) 					-- 设置失败重试次数
balancer.set_current_peer(host, port) 	-- 设置后端服务器地址
balancer.get_last_failure()					-- 获取上次连接失败的具体原因
```



# 共享内存

> lua_shared_dict

操作类似 redis，需要再看官方文档。



# 定时器

```lua
ngx.timer.running_count() 	-- 获取当前正在运行的定时任务数量

local ok, err = ngx.timer.at(delay, handler, ...) 	-- 启动定时器，delay 秒后到期

local ok, err = ngx.timer.every(60, handler, ...) 	-- 周期定时器
```



# 进程

```lua
local process = require("ngx.process")
local str = process.type() 	-- 获取当前进程的类型
-- single: 单一进程，非 master、worker 模式
-- master：监控进程
-- worker：工作进程
-- signaller：信号进程，用 -s 参数执行的进程
-- helper：辅助进程，不对外提供服务，例如 cache 进程
-- privileged agent：特权进程，Openresty 独有的进程。

ngx.worker.count() 	-- worker 进程数量
ngx.worker.pid() 		-- 系统进程号
ngx.worker.id() 		-- 进程的内部唯一编号
ngx.worker.exiting() 	-- 是否处于 exiting 状态

ngx.process.get_master_pid() 	-- 获取 master 的进程 ID，1.13.8 以上才支持

process.signal_graceful_exit() 	-- 自杀

-- 启用特权进程，只能在 init_by_lua 阶段使用
-- 实际工作在 init_worker_by_lua 阶段进行
process.enable_privileged_agent()
```



# 线程

轻量级线程，基于 Lua 协程。spawn 后立即运行，不会像 timer 在调用点所在代码运行完毕后执行。

```lua
local spawn = ngx.thread.spawn
local  t = spawn(handler, ...) 	-- 启动线程
ok, ... = ngx.thread.wait(t, t2, ...)	-- 等待线程结束，其中一个结束便返回

local threads = {...}
ok, ... = ngx.thread.wait(unpack(threads))	-- 等待线程结束，其中一个结束便返回

ngx.thread.kill(t) 	-- 杀死线程

-- 线程同步
local semaphore = require("ngx.semaphore")

local sema = semaphore.new(0)	-- 新建一个信号量对象，资源为 0
sema:post()	-- 通知消费者线程消费
sema:wait()	-- 等待生产者线程生产

```



# HTTPS/SSL/TLS

```lua
local ssl = require("ngx.ssl")

local ver, err = ssl.get_tls1_version() 	-- 获取版本号
local ver_str, err = ssl.get_tls1_version_str() 	-- 获取版本号字符串
local name, err = ssl.server_name() 	-- 获取 SNI 主机名
local addr, addr_type, err = ssl.raw_server_addr() 	-- 服务器地址
local addr, addr_type, err = ssl.raw_client_addr() 	-- 客户端地址
-- addr_type:
-- inet
-- inet6
-- unix

-- 清除证书，为后面动态设置证书腾出空间（ssl_certificate阶段）
local ok, err = ssl.clear_certs() 	-- 清除证书

local cert, err = ssl.parse_pem_cert(pem_cert) 	-- 解析 PEM 格式的证书
local ok, err = ssl.set_cert(cert) 	-- 设置证书

local priv_key, err = ssl.parse_pem_priv_key(pem_priv_key)	-- 解析 PEM 格式的私钥
local ok, err = ssl.set_priv_key(priv_key)	-- 设置私钥

local der_cert, err = ssl.cert_pem_to_der(cert)	-- PEM 证书转换为 DER 格式

```



