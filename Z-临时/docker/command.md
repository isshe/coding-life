
[TOC]

Docker 命令
---

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
> 如: `docker image ls ubuntu:18.04`

### 只列出镜像ID
> docker image ls -q
批量操作时会比较有用.

## 删除本地镜像
> docker image rm [选项] <镜像1> [<镜像2> ...]
`<镜像>`可以是`镜像短ID`、`镜像长ID`、`镜像名`或者`镜像摘要`.
删除行为分为两类，一类是`Untagged`，另一类是`Deleted`; 镜像可以拥有多个tag,因此只有所有tag都被删除后,才会delete.

## 构建镜像
> docker build [OPTIONS] PATH | URL | -
示例：`docker build -t nginx:v3 .`

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
docker run --name webserver -d -p 80:80 nginx
--name: 为容器命名
-d: 
-p: 映射端口
```

## 查看正在运行的容器

## 容器运行过程中进入容器
> docker exec
示例: `docker exec -it ubuntu:18.04 bash`

## 查看容器的改动
> docker diff <容器>
示例: `docker diff ubuntu:18.04`

## 把改动后的容器制作成镜像
> docker commit [选项] <容器ID或容器名> [<仓库名>[:<标签>]]

示例:
```
docker commit \
    --author "isshe <i.sshe@foxmail.com>" \
    --message "随便改了些东西" \
    ubuntu:18.04 \
    ubuntu:18.04.v2
```

## 退出容器
> exit
