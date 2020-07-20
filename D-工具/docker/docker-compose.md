[TOC]

Docker Compose
---
Docker Compose 是 Docker 官方编排（Orchestration）项目之一，用于快速的部署分布式应用。


# yml
* 以 `key: value`的方式指定配置；
* 多个配置以`缩进+换行`来进行。
* 不要使用 TAB 制表符。
示例：
```yml
version: '3.1'
services:
  nginx:
    restart: always
    build:
      context: ../   # dockerfile文件目录
      dockerfile: Dockerfile # dockerfile 名
    image: daocloud.io/library/nginx:1.13.2
    container_name: nginx
    ports:
      - 80:80
      - 8080:8080
    # enviroment:
    volumes:
      - /opt/isshe/data/nginx:/root
```

# 启动
> docker-compose up -d
`-d`: 后台启动


# 停止/启动/重启
> docker-compose stop/start/restart


# 停止并删除
> docker-compose down

# 查看日志
> docker-compose logs -f

# 重新构建自定义镜像
> docker-compose build
> docker-compose up --build -d
