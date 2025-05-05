# 编译运行步骤

## 编译

```bash
make
```

## 运行（加载 ko）

```bash
make insmod
```

## 查看输出

```bash
dmesg -T
```

## 卸载 ko

```bash
make rmmod
```

## 增加 pid

> 当树 3 层时，可以通过下面的命令（脚本）增长到 4 层

```bash
bash incr-pid.sh
```

## 更多

- before-grow.example.output：生长之前（3 层）的基数树示例
- after-grow.example.output：生长之后（变成 4 层）的基数树示例
