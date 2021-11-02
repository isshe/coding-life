
BPF Map
---
内核空间可以直接使用 BPF Map 变量；
用户空间则使用文件描述符。

# 创建
* 方法一

```c
union bpf_attr my_map {
    .map_type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(int),
    .value_size = sizeof(int),
    .max_entries = 100,
    .map_flags = BPF_F_NO_PREALLOC,
};

int fd = bpf(BPF_MAP_CREATE, &my_map, sizeof(my_map));

```

* 方法二

```c
int fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(int), \
    sizeof(int), 100, BPF_F_NO_PREALOC);
```

* 方法三

```c
struct bpf_map_def SEC("maps") my_map = {
    .type        = BPF_MAP_TYPE_HASH,
    .key_size    = sizeof(int),
    .value_size  = sizeof(int),
    .max_entries = 100,
    .map_flags   = BPF_F_NO_PREALLOC,
};
```



# 更新

* 内核空间

```c
int key, value, result;
key = 1, value = 1234;
result = bpf_map_update_elem(&my_map, &key, &value, BPF_ANY);
if (result == 0)
    printf("Map updated with new element\n");
else
    printf("Failed to update map with new value: %d (%s)\n", result, strerror(errno));
```

* 用户空间

```c
int key, value, result;
key = 1, value = 1234;
result = bpf_map_update_elem(map_data[0].fd, &key, &value, BPF_ANY);
if (result == 0)
  printf("Map updated with new element\n");
else
  printf("Failed to update map with new value: %d (%s)\n",
    result, strerror(errno));
```


# 获取
* 内核空间

```c
int key, value, result; // value is going to store the expected element's value
key = 1;
result = bpf_map_lookup_elem(&my_map, &key, &value);
if (result == 0)
  printf("Value read from the map: '%d'\n", value);
else
  printf("Failed to read value from the map: %d (%s)\n",
      result, strerror(errno));
```

* 用户空间
```c
int key, value, result; // value is going to store the expected element's value
key = 1;
result = bpf_map_lookup_elem(map_data[0].fd, &key, &value);
if (result == 0)
  printf("Value read from the map: '%d'\n", value);
else
  printf("Failed to read value from the map: %d (%s)\n",
      result, strerror(errno));
```


# 删除