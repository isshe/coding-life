
[TOC]

Docker 命令
---

> docker 中央仓库：https://hub.daocloud.io/
> docker 教程：


# 通用
## 安装
```
curl -fsSL get.docker.com -o get-docker.sh
sudo sh get-docker.sh --mirror Aliyun
```

## 启动
```
sudo systemctl enable docker
sudo systemctl start docker
```

## 建立并把相关用户添加到组
默认情况下，docker 命令会使用`Unix socket`与`Docker引擎`通讯。而只有`root`用户和`docker组`的用户才可以访问Docker引擎的Unix socket。
```
sudo groupadd docker
sudo usermod -aG docker $USER
```

## 测试Docker
```
docker run hello-world
sudo docker run hello-world
```

## 查看Docker相关内容占用空间
> docker system df

# 镜像相关

## 拉取镜像
> docker pull [选项] [Docker Registry 地址[:端口号]/]仓库名[:标签]

```
docker pull --help
如:
docker pull ubuntu:18.04
```

## 列出镜像
### 列出全部镜像
> docker image ls -a

### 列出顶层镜像
> docker image ls

### 列出虚悬镜像
> docker image ls -f dangling=true

### 列出特定镜像
> docker image ls <REPOSITORY:TAG>

示例：
```
docker image ls ubuntu:18.04
```

### 只列出镜像ID
> docker image ls -q

批量操作时会比较有用.

## 删除本地镜像
> docker image rm [选项] <镜像1> [<镜像2> ...]

`<镜像>`可以是`镜像短ID`、`镜像长ID`、`镜像名`或者`镜像摘要`.
删除行为分为两类，一类是`Untagged`，另一类是`Deleted`; 镜像可以拥有多个tag,因此只有所有tag都被删除后,才会delete.

> docker rmi <镜像标识>

## 构建镜像
> docker build [OPTIONS] PATH | URL | -

示例：
```
docker build -t nginx:v3 .
```

## 镜像的导入导出
导出:
> dokcer save -o <导出的路径/名称> <镜像 ID>

加载：
> docker load -i <镜像文件>

修改镜像名称
> docker tag <镜像 ID> <名称:版本>


## 自定义镜像
### 创建 Dockerfile 文件，并指定自定义镜像信息：
```
from：指定当前自定义镜像依赖的环境
copy：将需要的文件复制到自定义镜像中
workdir：声明镜像的默认工作目录
cmd：需要执行的命令（在 workdir下执行，cmd 可以写多个，以最后一个为准）
```

### 构建镜像
> docker build -t <镜像名称>:[tag] .


# 容器相关

## 运行镜像/容器
> docker run

示例:
```
docker run -it --rm ubuntu:18.04 bash
-i: 交互操作
-t: 终端
--rm: 退出容器后将其删除
bash: 用于交互的shell
```
示例:
```
docker run --name webserver -d -t -p 80:80 nginx
--name: 为容器命名
-d: 后台运行容器，并打印容器ID。（detach）
-p: 映射端口，`本地:容器`
```

## 查看正在运行的容器
> docker ps

## 容器运行过程中进入容器
有两种方式：exec、attach。推荐exec，因为attach进去以后，exit会导致容器终止。

> docker exec

示例: `docker exec -it ubuntu:18.04 bash`

> docker attach <容器>

示例：`docker attach 243c`


## 查看容器的改动
> docker diff <容器>

示例:
```
docker diff ubuntu:18.04
```

## 把改动后的容器制作成镜像

> docker commit [选项] <容器ID或容器名> [<仓库名>[:<标签>]]

示例：

> docker commit -a "isshe" -m "CI environment of openresty edge" c05732dc97c3 registry.cn-hongkong.aliyuncs.com/isshe/ubuntu-openresty-edge-ci:16.04.01

## 停止容器
> docker stop <容器>：停止容器。
> docker kill <容器>：强制停止一个容器。
> docker stop $(docker ps -qa)

## 重新启动容器
> docker start <容器>

示例:
```
docker commit \
    --author "isshe <i.sshe@foxmail.com>" \
    --message "随便改了些东西" \
    ubuntu:18.04 \
    ubuntu:18.04.v2
```

## 删除容器
> docker container rm <容器>
> docker rm <容器 ID>
> docker rm $(docker ps -qa)

## 清除所有处于终止状态的容器
> docker container prune

## 退出容器
> exit

## 查看容器log信息
> docker container logs <容器>

## 导入/导出容器
> docker export <容器>
> docker import <快照>

示例：
```
# 导出容器快照到本地文件
docker export 7691a814370e > ubuntu.tar

# 导入
cat ubuntu.tar | docker import - test/ubuntu:v1.0

# 从URL或某个目录导入
docker import http://example.com/exampleimage.tgz example/imagerepo
```

## 复制文件
* 复制到容器
> docker cp <文件> <容器ID>:<容器路径>


## 查看容器运行日志
> docker logs -f <容器 ID>
`-f`:滚动查看。

## 数据卷
* 创建数据卷
> dokcer volume create <数据卷名称>
创建数据卷之后，数据默认会存放在`/var/lib/docker/volumes/<数据卷名称>/_data 目录`

* 查看数据卷详细信息
> docker volume inspect <数据卷名称>

* 查看所有数据卷
> docker volume ls

* 删除数据卷
> docker volume rm <数据卷名称>

* 应用数据卷
方法一：
> docker run -v <数据卷名称>:<容器内部的路径> <镜像 ID>
容器内部路径中的文件，会同时映射到宿主机上。

方法二：
> docker run -v <宿主机路径>:<容器内部路径> <镜像 ID>
容器内部路径中的文件，不会映射到宿主机上。
宿主机路径不存在时，会自动创建。

# 注意
## docker的load 和 import 的区别？
* docker load 导入镜像存储文件到本地镜像库；
* docker import 导入一个容器快照到本地镜像库；
* 区别：
    * 区别在于容器快照文件将丢弃所有的历史记录和元数据信息（即仅保存容器当时的快照状态），而镜像存储文件将保存完整记录，体积也要大。
    * 此外，从容器快照文件导入时可以重新指定标签等元数据信息。