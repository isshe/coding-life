kubectl-trace
---

kubectl-trace 是 kubectl 的一个插件。

# 安装

```shell
go get -u https://github.com/iovisor/kubectl-trace/tree/master/cmd/kubectl-trace
```

# 检查 Kubernetes 节点

示例：计算打开文件数

```shell
kubectl trace run node/node_identifier -e \
    "kprobe:do_sys_open { @opens[str(arg1)] = count() }"
```

示例：在指定的容器中运行一个程序

```shell
kubectl trace run pod/pod_identifier -n application_name -e <<PROGRAM
uretprobe:/proc/$container_pid/exe:"main.main" {
    printf("exit: %d\n", retval)
}
PROGRAM
```