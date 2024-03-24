
[TOC]

modprobe
---

modprobe - 向 Linux 内核添加或从内核移除模块。

modprobe 智能地从 Linux 内核添加或删除模块：
* 注意，模块名称中`-`和`_`没有区别（自动执行下划线转换）。
* modprobe 在模块目录`/lib/modules/'uname -r'`(符号冲突了，用`'`代替) 中查找所有模块和其他文件。
    * 除了`/etc/modprobe.d`目录是的可选配置文件。详见`modprobe.d`
* modprobe 还将以`<module>.<option>`的形式使用内核命令行上指定的模块选项。
    * `modprobe.blacklist=<module>`的形式使用黑名单。
* 如果在 modulename 之后给出了任何参数，它们将被传递给内核。
    * 除了配置文件中列出的选项。

# 用法
```bash
modprobe [-v] [-V] [-C config-file] [-n] [-i] [-q] [-b] [modulename] [module parameters...]

modprobe [-r] [-v] [-n] [-i] [modulename...]

modprobe [-c]

modprobe [--dump-modversions] [filename]
```

# 选项
```bash
-a, --all
   加载命令行中指定的所有模块。

-b, --use-blacklist
   此选项使modprobe将配置文件（如果有）中的黑名单命令也应用于模块名称。 它通常由udev(7)使用。

-C, --config
   此选项会覆盖缺省配置目录（/etc/modprobe.d）
   此选项通过安装或删除命令传递给 MODPROBE_OPTIONS环境变量中 的其他modprobe命令。

-c, --showconfig
   输出config目录中的有效配置并退出。

--dump-modversions
   打印出模块所需的模块版本信息列表。 分发版通常使用此选项，以便使用模块版本控制deps(依赖？)打包Linux内核模块。

-d, --dirname
   模块的根目录，默认是`/`。

--first-time
   通常，如果告知插入已存在的模块或删除不存在的模块，modprobe将成功（并且不执行任何操作）。 
   这是简单脚本的理想选择; 然而，更复杂的脚本通常想知道modprobe是否真的做了一些事情：
   这个选项使modprobe失败，因为它实际上没有做任何事情。

--force-vermagic
   每个模块都包含一个包含重要信息的小字符串，例如内核和编译器版本。
   如果模块无法加载并且内核抱怨`version magic`不匹配，则可以使用此选项将其删除(抱怨)。
   当然，这个检查是为了保护你的，所以这个使用选项是危险的，除非你知道你在做什么。
   
   这适用于插入的任何模块：命令行上的模块（或别名）以及它所依赖的任何模块。

--force-modversion
   当使用CONFIG_MODVERSIONS集编译模块时，会创建模块使用（或由模块提供）的每个接口的版本的详细说明。
   如果模块无法加载并且内核抱怨模块不同意某个接口的版本，则可以使用“--force-modversion”来完全删除版本信息。 
   当然，这项检查是为了保护您的，所以使用此选项是危险的，除非您知道自己在做什么。
   
   这适用于插入的任何模块：命令行上的模块（或别名）以及它所依赖的任何模块。

-f, --force
   尝试从模块中删除任何可能阻止加载的版本信息：这与使用--force-vermagic和--force-modversion相同。 
   当然，这些检查是为了您的保护，所以使用此选项是危险的，除非您知道自己在做什么。
   
   这适用于插入的任何模块：命令行上的模块（或别名）以及它所依赖的任何模块。

-i, --ignore-install, --ignore-remove
   此选项使modprobe忽略命令行中，指定的模块的配置文件（如果有）中的安装和删除命令（任何相关模块仍然受配置文件中为它们设置的命令的限制）。 
   当使用此选项时，无论是否仅使用--ignore-install或--ignore-remove中的一个或其他（而不是两个）更具体地进行请求，将忽略安装和删除命令。 
   详见modprobe.d(5)。

-n, --dry-run, --show
   除了实际插入或删除模块（或运行安装或删除命令）之外，此选项可以执行所有操作。 
   与-v结合使用，可用于调试问题。 由于历史原因，--dry-run和--show实际上意味着相同的事情并且可以互换。

-q, --quiet
   使用此标志，如果您尝试删除或插入无法找到的模块（并且不是别名或安装/删除命令），modprobe将不会打印错误消息。
   但是，它仍将以非零退出状态返回。 
   内核使用它来机会性地探测可能存在的正在使用request_module的模块。

-R, --resolve-alias
   打印与别名匹配的所有模块名称。这对于调试模块别名问题很有用。

-r, --remove
   这个选项使modprobe删除而不是插入一个模块。
   如果它依赖的模块也没有使用，modprobe讲尝试移除它们。
   不像插入可以在命令行上指定多个模块。（意思是-r指定移除一个？）
   （在删除模块时指定模块参数没有意义）
   通常没有理由删除模块，但有些错误的(buggy)模块需要它。 您的发行版内核可能尚未构建为支持删除模块。

-S, --set-version
   设置内核版本，而不是使用uname(2)来决定内核版本（它决定了在哪里找到模块）。

--show-depends
   显示模块的依赖，包括模块它自己。
   列出模块（或别名）的依赖关系，包括模块本身。 
   这会生成一组(可能为空)模块文件名，每行一个，每个以“insmod”开头，通常由发行版使用，以确定生成initrd/initramfs映像时要包含哪些模块。 
   应用的安装命令以“install”为前缀。
   它不运行任何安装命令。
   请注意，modinfo(8) 可用于从模块本身提取模块的依赖关系，但不知道别名或安装命令。

-s, --syslog
   此选项会导致错误消息通过syslog机制（如级别为LOG_NOTICE的LOG_DAEMON）而不是标准错误。
   当stderr不可用时，也会自动启用此功能。
   
   此选项通过安装或删除命令传递给MODPROBE_OPTIONS环境变量中的其他modprobe命令。

-V, --version
   显示此程序的版本并退出。

-v, --verbose
   打印程序正在做什么的信息。
   通常modprobe只打印出错的信息。

   此选项通过安装或删除命令传递给MODPROBE_OPTIONS环境变量中的其他modprobe命令。

```
# 示例
## 加载模块
> modprobe vfat

## 卸载模块
> modprobe -r vfat

## 查看模块的配置文件
> modprobe -r

输出类似：
```c
alias ip6t_conntrack xt_conntrack
alias ip_conntrack nf_conntrack_ipv4
alias symbol:__nf_conntrack_confirm nf_conntrack
alias symbol:ct_sip_parse_request nf_conntrack_sip
```
* symbol：应该是说这个是`nf_conntrack`中的符号（也就是函数）。【？？？】

# 疑问
* modprobe -r 输出的含义？

# 相关
* modprobe.d
* insmod
* rmmod
* lsmod
* modinfo

# 参考