[TOC]

tcpdump
---

# 常用命令
## 查看历史记录
> yum history list all

## 回退
> yum history undo <ID> -y --setopt=protected_multilib=false

## 安装
> yum install <pkg>

## 查看那些包包含指定内容
> yum whatprovides abc*
