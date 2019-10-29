[TOC]

kubernetes
---

# kubectl
* kubectl get: 显示资源
* kubectl describe: 显示资源的详细信息
* kubectl logs: 打印一个pod的一个容器的log
* kubectl exec: 在一个pod的一个容器中运行一个命令

## 显示版本
```
kubectl version
```

## 显示集群信息
```
kubectl cluster-info
```

## kubectl get
```
# 获取集群中的节点
kubectl get nodes

# 获取存在的pods
kubectl get pods

# 获取当前的services
kubectl get services

# 获取deployments
kubectl get deployments

# 根据label获取pods/services
kubectl get <pods/services> -l <lable-name>
```

## kubectl describe
### 查看暴露出去的端口
```shell
kubectl describe services/<NAME>
例如：
kubectl describe services/kubernetes-bootcamp
```

### 获取标签名
```shell
# 获取deployment的标签
kubectl describe deployment
```

## 部署APP
```
kubectl create deployment <NAME> --image=<images url>
例如:
kubectl create deployment kubernetes-bootcamp --image=gcr.io/google-samples/kubernetes-bootcamp:v1
```

## 获取POD名字
```shell
export POD_NAME=$(kubectl get pods -o go-template --template '{{range .items}}{{.metadata.name}}{{"\n"}}{{end}}')
#echo Name of the Pod: $POD_NAME
```

## 暴露端口
```shell
kubectl expose <NAME> --type=<TYPE> --port <PORT>
例如：
kubectl expose deployemnt/kubernetes-bootcamp --type="NodePort" --port 8080
```


## 而是暴露出去的端口
```shell
# 获取端口
export NODE_PORT=$(kubectl get services/kubernetes-bootcamp -o go-template='{{(index .spec.ports 0).nodePort}}')
echo NODE_PORT=$NODE_PORT

curl $(ip):$NODE_PORT
```

## 设置label
```shell
kubectl label <OBJ-TYPE> <OBJ-NAME> <NEW-LABEL>
# 例如
kubectl label pod $POD_NAME app=v1
```

## 删除一个service
```shell
kubectl delete service -l <label-name>

# 确认暴露的服务无法访问：
kubectl get services
curl $(ip):$NODE_PORT

# 确认app还在运行(以监听8080为例)
kubectl exec -it $POD_NAME curl localhost:8080
```



