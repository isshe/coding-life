

> 待整理

选项：

- `--gen-suppressions=yes`：生成 suppress 文件最简单的方法
- `--suppressions=/path/to/file.suppress`：指定 suppress 文件

- 不同的检测工具输出的错误信息不同，因此可以指定 suppress 文件。
- 可以指定多个 suppress 文件

## suppression 结构

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

- 第一行：name_of_suppression——名称。
- 第二行：tool_name——工具名，多个则通过逗号隔开。然后跟“:”再跟 suppression 类别。
- 接下来一行：可选的额外信息（optional extra info for some suppression types），例如 Memcheck 工具的 Param。
- 剩余行：导致错误的函数调用链，最多 24 行，可以通过选项 `TODO` 来修改。

**重要提示**：C++ 函数的名称必须是变形后的。手动编写抑制规则，需使用 `--demangle=no` 选项在错误消息中获取变形后的名称，如 ZN9QListView4showEv。这是 GNU C 编译器内部使用的形式，也是抑制文件中必须使用的形式。等效的原型名称是 `QListView::show()`。
