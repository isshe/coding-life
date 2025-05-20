# Linux 内核中的宏

- `__randomize_layout`：​​结构体布局随机化​​（Structure Layout Randomization）相关的**编译时**特性，旨在通过随机化内核数据结构的成员排列顺序，增加攻击者利用内存破坏漏洞（如缓冲区溢出、Use-after-Free 等）的难度。
- `__designated_init`：必须显式初始化。示例：

    ```c
    // 标记结构体必须使用指定初始化器
    struct critical_data __designated_init {
        int id;
        void *private;
        unsigned long flags;
    };

    // 合法初始化（明确指定成员）
    struct critical_data valid = { .id = 42, .private = NULL, .flags = 0 };

    // 非法初始化（依赖顺序，编译时报错或警告）
    struct critical_data invalid = { 42, NULL, 0 }; // 可能触发警告/错误
    ```
