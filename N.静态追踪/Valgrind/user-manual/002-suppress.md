

> 待整理

选项：

- `--gen-suppressions=yes`：生成 suppress 文件最简单的方法
- `--suppressions=/path/to/file.suppress`：指定 suppress 文件

- 不同的检测工具输出的错误信息不同，因此可以指定 suppress 文件。
- 可以指定多个 suppress 文件


suppression 示例：

```suppress
# {
#     name_of_suppression
#     tool_name1,tool_name2:supp_kind
#     (optional extra info for some suppression types)
#     caller0 name, or /name/of/so/file.so
#     caller1 name, or ditto
#     (optionally: caller2 name)
#     (optionally: caller3 name)
#  }
```

- name_of_suppression: 名称
- tool_name：工具名
- supp_kind：suppression 类型
- optional extra info for some suppression types：额外的信息，例如 Memcheck 工具的 Param。
