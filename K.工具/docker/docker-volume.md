
Docker
---

# 数据管理

## 数据卷
数据卷——一个可供一个或多个容器使用的特殊目录。
`数据卷`是被设计用来持久化数据的，它的生命周期独立于容器。
* 可以再容器之间共享和重用；
* 对它的修改会立即生效；
* 对它的更新不会影响镜像；
* 会一直存在，不会随容器删除而删除；

### 创建数据卷
> docker volume create <卷名>

### 查看数据卷
列出所有卷：
> docker volume ls

查看指定卷：
> docker volume inspect <卷名>

### 挂载数据卷
docker run 中：
> --mount source=<卷名>,target=<容器内目标路径>

### 删除数据卷
> docker volume rm <卷名>

### 清理数据卷
无主的数据卷可能会占据很多空间，使用以下命令进行清理：
> docker volume prune

## 挂载主机文件/目录
挂载权限默认是读写，可以用 readonly 指定为只读。
docker run 中：
> --mount type=bind,source=<主机文件/目录>,target=<容器文件/目录>
> --mount type=bind,source=<主机文件/目录>,target=<容器文件/目录>,readonly