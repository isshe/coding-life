
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

* 内核空间

```
result = bpf_map_delete_element(&my_map, &key);
```

* 用户空间

```
result = bpf_map_delete_element(map_data[0].fd, &key);
```

# 遍历

如果在遍历过程中删除元素，bpf_map_get_next_key 尝试查找已删除元素的键，找不到将重新开始。

* 内核空间

```
int next_key, lookup_key;
lookup_key = -1;
while(bpf_map_get_next_key(map_data[0].fd, &lookup_key, &next_key) == 0) {
  printf("The next key in the map is: '%d'\n", next_key);
  lookup_key = next_key;
}
```

* 用户空间

```
int next_key, lookup_key;
lookup_key = -1;

while(bpf_map_get_next_key(map_data[0].fd, &lookup_key, &next_key) == 0) {
  printf("The next key in the map is: '%d'\n", next_key);
  if (next_key == 2) {
    printf("Deleting key '2'\n");
    bpf_map_delete_element(map_data[0].fd &next_key);
  }

  lookup_key = next_key;
}
```

# 查找并删除元素

* 用户空间

```
int key, value, result, it;
key = 1;

for (it = 0; it < 2; it++) {
  result = bpf_map_lookup_and_delete_element(map_data[0].fd, &key, &value);
  if (result == 0)
    printf("Value read from the map: '%d'\n", value);
  else
    printf("Failed to read value from the map: %d (%s)\n", result, strerror(errno));
}
```

# 并发访问 BPF MAP

* 内核空间

BPF MAP 使用自旋锁 (spinlock) 解决竞争问题；但只能用于数组、HASH 和 cgroup 存储映射中。

> bpf_spin_lock：锁定一个元素
> bpf_spin_unlock：解锁一个元素

* 示例
```
struct concurrent_element {
  struct bpf_spin_lock semaphore;
  int count;
}

struct bpf_map_def SEC("maps") concurrent_map = {
  .type        = BPF_MAP_TYPE_HASH,
  .key_size    = sizeof(int),
  .value_size  = sizeof(struct concurrent_element),
  .max_entries = 100,
};

BPF_ANNOTATE_KV_PAIR(concurrent_map, int, struct concurrent_element);

int bpf_program(struct pt_regs *ctx) {

  int key = 0;
  struct concurrent_element init_value = {};
  struct concurrent_element *read_value;

  bpf_map_create_elem(&concurrent_map, &key, &init_value, BPF_NOEXIST);
  read_value = bpf_map_lookup_elem(&concurrent_map, &key);
  bpf_spin_lock(&read_value->semaphore);
  read_value->count += 100;
  bpf_spin_unlock(&read_value->semaphore);
}
```

