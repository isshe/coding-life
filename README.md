# Coding-life

keep reading,
keep writing,
keep coding,
keep breathing...

正在进行（On-CPU）

- OpenResty Xray 相关

持续中（Off-CPU）

- [C 语言 CPU 高占用分析](R.经典案例/CPU/c-high-cpu/C语言CPU高占用分析.md)
  - [SystemTap 最佳实践 - 获取调用栈](J.调试/动态追踪/SystemTap/最佳实践/003-c-stack-trace/README.md)
- [Valgrind 用户手册](J.调试/静态追踪/Valgrind/user-manual)
- [OpenResty 核心技术点汇总](K.工具/OpenResty/使用与实现/000-core-tech.md)
- [Luajit 核心技术点汇总](K.工具/Luajit/设计与实现/000-core-tech.md)
- [Nginx 的阶段解析](K.工具/Nginx/Nginx源码分析/4-nginx-phase.md)

下一步

- [OpenResty 时间更新机制](K.工具/OpenResty/使用与实现/021-time-sync.md)
- [SystemTap 最佳实践 - 介绍](J.调试/动态追踪/SystemTap/最佳实践/001-introduction/README.md)
- [SystemTap 最佳实践 - 安装](J.调试/动态追踪/SystemTap/最佳实践/002-installation/README.md)
- [如何生成火焰图？](J.调试/动态追踪/如何生成火焰图.md)
- [Luajit 垃圾回收机制](K.工具/Luajit/设计与实现/004-garbage-collection.md)
- [Nginx timer 是如何实现的？](K.工具/Nginx/Nginx源码分析/8-nginx-timer.md)
- [Nginx 热更新](K.工具/Nginx/Nginx源码分析/10-binary-upgrade.md)
- [Nginx 内存分配](K.工具/Nginx/Nginx源码分析/11-slab.md)
- [Nginx 内存共享](K.工具/Nginx/Nginx源码分析/12-nginx-shm.md)
- [动态链接是如何工作的？](B.操作系统/设计与实现/how-dynamic-linking-work.md)
- [Nginx 是如何进行 DNS 解析的？](K.工具/Nginx/Nginx源码分析/7-nginx-dns-resolve.md)
- [Luajit 中 yield 和 resume 的实现](K.工具/Luajit/设计与实现/2.yield-resume.md)
- [Core Dump 分析](Q.问题解决/core-dump/README.md)
- [内存泄漏分析](Q.问题解决/memory-leak/README.md)

## 索引

### 数据库

- [如何排查、优化 PostgreSQL 的慢查询？](F.数据库/4.PostgreSQL/慢查询分析/README.md)
- [如何管理 TimescaleDB 的清理策略？](F.数据库/4.PostgreSQL/TimescaleDB/README.md)
- [unexpected chunk number X (expected X) for toast value XXX in pg_toast_XXX 错误处理](F.数据库/4.PostgreSQL/unexpected-chunk-number.md)

### Linux 系统

- [如何查找并清理 Linux 系统中多余的文件？](B.操作系统/Linux/存储空间占用情况分析.md)
- [如何排查磁盘空间泄漏？](B.操作系统/Linux/存储空间占用情况分析.md)

### OpenResty

- [cosocket](K.工具/OpenResty/使用与实现/014-cosocket.md)
- [OpenResty 的管道](K.工具/OpenResty/使用与实现/017-pipe.md)
- [OpenResty Lua 代码缓存](K.工具/OpenResty/使用与实现/018-ngx_lua_cache.md)
- [OpenResty 共享内存](K.工具/OpenResty/使用与实现/019-ngx_shared_dict.md)

### 网络编程

- [I/O 多路复用与非阻塞连接](B.操作系统/Linux/Application/7.IO多路复用/IO多路复用与非阻塞连接/README.md)

### 源码阅读

- [lua-nginx-module](https://github.com/isshe/lua-nginx-module)
- [lua-resty-core](https://github.com/isshe/lua-resty-core)
- [valgrind](https://github.com/isshe/valgrind)

### 测试

- [性能测试要点及常见问题](T.测试/性能测试.md)

### 问题排查/解决

**CPU 问题**

**内存问题**

**网络问题**

[TCP 连接被中断](Q.问题解决/network/TCP连接被中断.md)

**其他问题**

- [Failed to create /init.scope control group: Read-only file system](Q.问题解决/OS/RockyLinux9/failed-to-create-init-scope-control-group.md)

### 最佳实践

> 尚不完善，因此放后面
> [关于最佳实践](M.方法论/关于最佳实践.md)

- [SystemTap 最佳实践](J.调试/动态追踪/SystemTap/最佳实践)

## 更多说明

- 算法和数据结构：包含常见的算法和数据结构的介绍、实现示例、时间复杂度和空间复杂度分析等。
- 操作系统：涵盖操作系统的基本概念、进程管理、内存管理、文件系统；操作系统的使用等。
- 计算机网络：包括网络基础知识、TCP/IP 协议栈、HTTP、DNS、WebSocket 等。
- 编程语言：各个语言的基础知识、语法、常见问题、最佳实践等。
- 设计模式：介绍常见的软件设计模式，如单例模式、观察者模式、工厂模式等。
- 数据库：包括关系型数据库（如 MySQL、PostgreSQL）和 NoSQL 数据库（如 MongoDB、Redis），涵盖数据库设计、查询语言、性能优化等。
- 计算机安全：涵盖常见的安全概念、攻击类型、防御策略、加密算法等。
- 前端开发：HTML、CSS、JavaScript、前端框架（如 React、Vue.js）等。
- 后端开发（暂无对应目录）：涵盖常见的后端开发技术，如 RESTful API、服务器框架（如 Flask、Django）、身份验证、数据库连接等。
- 编译原理：编译原理的学习和实践。
- 调试：静态追踪、动态追踪、调试技巧等。
- 测试：包括单元测试、集成测试、性能测试等。
- 工具：涵盖常见的开发工具和框架，如 Git、Docker、libevent等。
- 软件工程（暂无对应目录）：包括软件开发生命周期、版本控制、团队协作、项目管理等。
- 云计算（暂无对应目录）：涵盖云服务提供商（如 AWS、Azure、Google Cloud）的基本知识、服务、部署和扩展等。
- 人工智能和机器学习（暂无对应目录）：包括机器学习算法、深度学习框架、数据预处理、模型评估等。
- 方法论：总结方法——处理特定事物的通用方法。
- 自然语言：英语等自然语言的学习。
- 问题解决：总结学习、工作中遇到的一些较为常见、网上不太容易找到答案的问题。
- 经典案例：学习、工作中遇到的经典案例。可以从 0 到 1 地学习某个知识点或技能。
- Think：个人所思所想、人生观、价值观等。
- Blog：博客管理目录
- Temp：临时目录，存在一些待整理的内容。
