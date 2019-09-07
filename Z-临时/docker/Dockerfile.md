
[TOC]

Dockerfile
---

Dockerfile 是一个文本文件，其内包含了一条条的`指令(Instruction)`.
**每一条指令构建一层**，因此每一条指令的内容，就是描述该层应当如何构建。

# FROM
指定基础镜像, 如：`FROM nginx`

# RUN
执行命令；构建容器的一层。
每一个`RUN`都是启动一个容器、执行命令、然后提交存储层文件变更。

示例：
```
RUN apt-get update
```

# COPY
COPY——复制文件。默认情况下，源文件的权限、修改时间等都会被保留。

> COPY [--chown=<user>:<group>] <源路径>... <目标路径>
> COPY [--chown=<user>:<group>] ["<源路径1>",... "<目标路径>"]

示例：
```
COPY package.json /usr/src/app/
COPY hom* /mydir/
COPY hom?.txt /mydir/
COPY --chown=55:mygroup files* /mydir/
```

# ADD
ADD——增强版COPY。
增强点：
* `源路径`可以是一个URL。Docker引擎会去下载此链接的文件放到`目的路径`去，文件默认权限是`600`。
    * 此功能通常后续需要用RUN，会多一层。因此不推荐使用。
* `源路径`是`tar`压缩文件时，会解压到`目的路径`。
注意：
* `ADD`指令会令镜像构建缓存失效，从而可能会令镜像构建变得比较缓慢。
* `COPY`和`ADD`的使用原则：
    * 所有文件的复制，都使用`COPY`。
    * 需要自动解压缩时，使用`ADD`。

# CMD
CMD指令用于指定默认的容器主进程的启动命令。
CMD指令的格式和RUN类似，也是两种格式：（推荐用第二种）
> CMD <命令>
> CMD ["可执行文件", "参数1", "参数2"...]

参数列表格式：
> CMD ["参数1", "参数2"...]

在指定了`ENTRYPOINT`指令后，用`CMD`指定具体的参数。

# ENTRYPOINT
`ENTRYPOINT`的格式和`RUN`指令格式一样，分为`exec`格式和`shell`格式。

> <ENTRYPOINT> "<CMD>"

当指定`ENTRYPOINT`后，`CMD`的内容将作为参数传递给`ENTRYPOINT`，而不是直接执行命令。
更多信息见[entrypoint](https://yeasy.gitbooks.io/docker_practice/image/dockerfile/entrypoint.html)

# ENV
ENV——设置环境变量。
> ENV <key> <value>
> ENV <key1>=<value1> <key2>=<value2>...

# ARG
ARG——构建参数和`ENV`的效果一样，都是设置环境变量。
不同的是，`ARG`所设置的构建环境的环境变量，在将来容器运行时是不会存在这些环境变量的。
> ARG <参数名>[=<默认值>]

# VOLUME
VOLUME——定义匿名卷。
> VOLUME ["<路径1>", "<路径2>"...]
> VOLUME <路径>

容器运行时应该尽量保持容器存储层不发生写操作，对于数据库类需要保存动态数据的应用，其数据库文件应该保存于卷(volume)中
示例：
```
VOLUME /data
# 使用了 mydata 这个命名卷挂载到了 /data 这个位置
# 替代了 Dockerfile 中定义的匿名卷的挂载配置。
docker run -d -v mydata:/data xxxx  #???
```

# EXPOSE
EXPOSE——声明运行时容器提供服务的端口。只是声明，不会开启。
> EXPOSE <端口1> [<端口2>...]

用处：
* 帮助镜像使用者理解这个镜像服务的守护端口，以方便配置映射。
* 在运行时使用随机端口映射时，也就是`docker run -P`时，会自动随机映射`EXPOSE`的端口。【？？？】

# WORKDIR
WORKDIR——指定工作目录（或者称为当前目录）。
> WORKDIR <工作目录路径>

# USER
USER——指定当前用户（切换到指定用户）。改变之后各层执行`RUN/CMD/ENTRYPOINT`等命令的身份。
USER只是切换，不会创建用户。
> USER <用户名>[:<用户组>]

示例：
```
RUN groupadd -r redis && useradd -r -g redis redis
USER redis
RUN [ "redis-server" ]
```

# 


