[TOC]

nginx
---

# nginx变量
* nginx变量的生命期是不可能跨越`请求`边界(跨请求)的。

## nginx变量群
详细参见: [ngx_http_core](http://nginx.org/en/docs/http/ngx_http_core_module.html)模块官网文档。
* `$arg_xxx`: get参数。
* `$cookie_xxx`: 取cookie值。
* `$http_xxx`: 取请求头。
* `$sent_http_xxx`: 取响应头。


# nginx的阶段
> post-read、server-rewrite、find-config、rewrite、post-rewrite、preaccess、access、post-access、try-files、content、log
* `rewrite`: 一般用来对当前请求进行各种修改(如对URL和URL参数进行改写)；或者创建并初始化一些后续阶段可能用到的Nginx变量。
* `access`: 一般用来执行访问控制性质的任务。（如检查用户的访问权限、检查用户的来源IP是否合法等）
* `content`: 一般用来生成"内容(content)"，并输出HTTP响应。
  * 所有请求处理阶段中最为重要的一个。
  * 应当避免在同一个`location`中使用多个模块的`content`阶段指令，因为通常一个location只有其中一个模块能注册`内容处理程序`.
* `output filter`: `输出过滤器`阶段，**非11个标准的nginx阶段之一**。用于对输出/响应内容做修改。
* `post-read`: nginx读取并解析完`请求头(request headers)`之后，立即开始执行。
* `server-rewrite`:
* `find-config`: 不能注册处理程序，由nginx核心来完成请求与配置块的配对工作。
* `post-rewrite`: 不能注册处理程序，由nginx核心完成rewrite阶段要求的`内部跳转`操作（如果rewrite阶段有要求的话）。
* `preaccess`: 访问控制，主要是访问频率等。
* `post-access`: 不能注册处理程序。主要用于配置access阶段实现标准`ngx_http_core`模块提供的配置执行`satisfy`的功能.
* `try-file`: 不能注册处理程序。N个参数，依次检查N-1个参数映射的文件/目录是否存在，存在则返回。不存在则以第N个参数`内部跳转/返回指定状态码页`。
  * 当最后一个参数时，总是直接执行“内部跳转”，而不论其对应的文件系统对象是否存在。
* `log`: 记录log的阶段。


# 常见错误
* `proxy_pass`和其他`content`阶段的模块在同一个location。
  * 因为`proxy_pass`属于`content`，而同一个location通常只能注册一个`内容处理程序`。


# 疑问
* 若`location`中没有模块注册`content`内容处理程序，生成内容和输出响应应该由谁来处理？
  * ngx_index/ngx_autoindex/ngx_static
* `rewrite`(/内部跳转)之后(处于post-rewrite阶段)，重新从哪个阶段开始？
  * 回退到`find-config`阶段.
  * 在`post-rewrite`阶段发生回退。
  * rewrite/access/content之类的都会重新执行。
