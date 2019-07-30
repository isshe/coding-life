[TOC]

# 内核编码规范
这是一个简短的文档，描述了 linux 内核的首选代码风格。代码风格是因人而异的， 
而且我不愿意把自己的观点强加给任何人，但这就像我去做任何事情都必须遵循的原则 
那样，我也希望在绝大多数事上保持这种的态度。请 (在写代码时) 至少考虑一下这里 
的代码风格。

首先，我建议你打印一份 GNU 代码规范，然后不要读。烧了它，这是一个具有重大象征 
性意义的动作。

不管怎样，走起：

## 1) 缩进
`Tab`是`8`个字符，因此缩进也是8个字符。有些异端运动试图将缩进变为 4 (甚至2！) 字符深，这几乎相当于尝试将圆周率的值定义为 3。
理由：缩进的背后思想是清楚地定义控制块的开始和结束位置。如果需要超过`3`个级别的缩进，那么就应该修改/修复程序。简言之，8字符使代码更容易阅读。
当函数嵌套太深时，一个额外的好处是给你警告【注意这个警告】。
为缓解switch语句多个缩进级别的首选方法是switch和case对齐，而不是二次缩进case，例如：
```c
switch (suffix) {
case 'G':
case 'g':
	mem <<= 30;
	break;
case 'M':
case 'm':
	mem <<= 20;
	break;
case 'K':
case 'k':
	mem <<= 10;
	/* fall through */
default:
	break;
}
```
除非你有隐藏的内容，否则`不要`在一行中放置多个语句：
```c
if (condition) do_this;
  do_something_everytime;
```
* 不要在一行上放置多个语句。内核编码风格非常简单，避免棘手的表达。
* 除了`注释/文档/Kconfig`外，**`空格永远不会用于缩进`**。
* 不要在行尾留下空白。

## 2) 打破长行和长字符串
* 行长限制是`80`字符。
    * 除非超过`80`字符的行显著提高可读性并且不隐藏信息，否则都应该分解为合理的块。
    * 后代总是比父母短得多，并且基本放在右边。
* 永远不要破坏用户可见的字符串，例如printk消息，因为这会破坏`grep`的能力。

## 3. 大括号和空格
大括号在右边：(适用于`if, switch, for, while, do`)
```c
if (x is true) {
	we do y
}
```
大括号在新一行：
```c
int function(int x)
{
	body of function
}
```
注意，右括号`}`是另起一行的，`除非`后面跟着相同语句的延续。即`do`的`while`，或者`if`的`else if`或`else`。如下：
```c
do {
	body of do-loop
} while (condition);
```
和
```c
if (x == y) {
	..
} else if (x > y) {
	...
} else {
	....
}
```
不要使用不必要的大括号：（单语句时）
```c
if (condition)
	action();
```
和
```c
if (condition)
	do_this();
else
	do_that();
```
如果条件语句只有一个分支是单语句则`不适用`:
```c
if (condition) {
	do_this();
	do_that();
} else {
	otherwise();
}
```
当循环中包含包含多个单语句时，使用大括号：
```c
while (condition) {
	if (test)
		do_something();
}
```

## 3.1 空格
内核中空格的使用依赖(大多时候)函数与关键字的使用。
* 大多时候会在关键字`后`使用空格，如`if, switch, case, for, do, while`
    * 值得注意的例外是：看着像函数的`sizeof/typeof/alignof/__attribute__`。
使用空格：
```
if () {
	do_something();
}
```
使用():
```c
s = sizeof(struct file);
```
但不要——在括号旁边加空格，如：
```c
s = sizeof( struct file );
```
声明指针或者返回指针时，`*`靠近变量名：
```c
char *linux_banner;
unsigned long long memparse(char *ptr, char **retptr);
char *match_strdup(substring_t *s);
```
在大多数二元三元运算符周围(两侧)使用一个空格，如以下的运算符：
```c
=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :
```
但一元运算符后不加空格：
```c
&  *  +  -  ~  !  sizeof  typeof  alignof  __attribute__  defined
```
前后都不要空格：
```c
++  --
```
数据结构的`.`和`->`周围也不用空格。
行尾不要留有空格。


## 4) 命名
全局变量（仅在真正需要时才使用）/全局函数需要具有描述性的名称。
* 例如，定义一个统计活跃用户数的函数，应该用count_active_users()，而不应该cntusr()。
局部变量应该简要。

## 5) Typedefs
不要使用类似`vps_t`这样的类型。
将typedef用于struct和指针是一个错误，
```c
vps_t a;
```
这样的代码是什么意思呢？相反，如果是：
```c
struct virtual_container *a;
```
你就可以说明a是什么。
很多人觉得typedef有助于可读性。这是个误解，typdef仅适用于：
* 完全不透明对象：typedef主动用于**隐藏**对象的内容。
    * 例如：`pte_t`等只能使用正确的访问器进行访问的不透明对象。
* 清除整型类型：有助于避免混淆，无论是int还是long。u8/u16/u32是完美的typedef。
* 当你使用稀疏字面创建一个新类型以进行类型检查。【？？？】
    * when you use sparse to literally create a new type for type-checking.
* 在某些特殊情况下，与标准C99类型相同的新类型。【？？？】
* 在用户空间中安全使用的类型。（内核编程是内核空间）
    * 在用户空间可见的某些结构中，我们不能要求C99类型，也不能使用u32之类的。因此，我们在与用户空间共享的所有结构中使用__u32和类似的类型。
规则是：基本上，永远不用使用typedef，除非你能清楚匹配以上其中一个规则。

## 6) 函数

函数应该是短而甜，只做一件事的，并做好一件事。它们应该只占1到2屏(1屏：80x24)。
函数的最大长度与该函数的复杂度和缩进级别成`反比`。（简单的函数可以很长，但是复杂的函数反而应该短）
函数的局部变量的数量不应该超过`5-10`个。否则你应该重新考虑功能，并将其拆分成更小的部分。
在源文件中，使用`一个空行`分隔函数，如果导出该函数，则它的EXPORT宏应该紧随其后。如：
```c
int system_is_up(void)
{
	return system_state == SYSTEM_RUNNING;
}
EXPORT_SYMBOL(system_is_up);
```
在函数原型中，包含参数名称及其数据类型。
不要将extern关键字与函数原型一起使用。


## 7) 集中退出函数
虽然有些人不赞成使用，但编译器经常以`无条件跳转指令`的形式使用等效的goto语句。
当函数从多个位置退出并且必须完成一些常见工作(如清理)时，goto语句会派上用场。如果不需要清理，则直接返回。
标签名称：说明goto的作用或goto存在的原因。如，一个好名称`out_free_buffer`，goto去释放buffer。避免使用`err1:`或`err2:`，因为如果你增加或删除退出路径，你必须重新进行编号。
使用goto的基本原理是：
* `无条件语句(uncoditional statments)`更容易理解和遵循。
* 嵌套减少了。
* 防止进行修改时不更新各个出口点的错误。
* 保存编译器工作以优化冗余代码。
```c
int fun(int a)
{
	int result = 0;
	char *buffer;

	buffer = kmalloc(SIZE, GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

	if (condition1) {
		while (loop1) {
			...
		}
		result = 1;
		goto out_free_buffer;
	}
	...
out_free_buffer:
	kfree(buffer);
	return result;
}
```
一个常见的`one err bugs`：
```c
err:
	kfree(foo->bar);
	kfree(foo);
	return ret;
```
当存在退出路径foo是NULL时，会出现bug。通常这种情况的解决方法是分成两个错误标签：
```c
 err_free_bar:
	kfree(foo->bar);
 err_free_foo:
	kfree(foo);
	return ret;
```
理想情况下，你应该模拟错误以测试所有路径。


## 8) 注释
注释很好，但是也有过度注释的危险。
永远不要在注释中解释你的代码如何工作的。应该把代码写好，而不是浪费时间注释差的代码。
一般来说，注释中应该说明代码是`做什么`而不是`怎么做`。
并且，尽量不要在函数体内进行注释；如果函数太复杂而需要单独注释它的一部分，那么你应该回到`6)`去看看。你可以进行一些小注释，但是尽量避免过多。相反，将注释放在头部，告诉人们它做什么和它为什么可以做到。
注释内核API时，请使用`内核文档格式`。详见`Documentation/doc-guide/ <doc_guide>`和`scripts/kernel-doc`。
多行注释示例：
```c
/*
 * This is the preferred style for multi-line
 * comments in the Linux kernel source code.
 * Please use it consistently.
 *
 * Description:  A column of asterisks on the left side,
 * with beginning and ending almost-blank lines.
 */
```
`net/`和`drivers/net/`的多行注释稍有不同：(第一行不同)
```c
/* The preferred comment style for files in net/ and drivers/net
 * looks like this.
 *
 * It is nearly the same as the generally preferred comment style,
 * but there is no initial almost-blank line.
 */
```
对数据的注释也很重要，无论他们是基本类型还是派生类型。每行只使用一个函数声明。


## 9) 你弄得一团糟
> emacs相关，跳过

## 10) Kconfig配置文件
代码树中的所有Kconfig配置文件，缩进有所不同。config定义的行，用一个`tab`缩进，而帮助文本缩进再两个空格。
```c
  config AUDIT
	bool "Auditing support"
	depends on NET
	help
	  Enable auditing infrastructure that can be used with another
	  kernel subsystem, such as SELinux (which requires this for
	  logging of avc messages output).  Does not do system-call
	  auditing without CONFIG_AUDITSYSCALL.
```
真正危险的特性（例如对某些文件系统的写支持）应在其提示字符串中突出显示：
```c
  config ADFS_FS_RW
	bool "ADFS write support (DANGEROUS)"
	depends on ADFS_FS
	...
```
有关配置文件的完整文档，见`Documentation/kbuild/kconfig-language.rst`。

## 11) 数据结构
在创建和销毁此数据结构的单线程环境之外具有可见性的数据结构，应始终具有引用计数。内核中没有垃圾回收，因此必须使用引用计数记录所有用途。
引用计数意味着可以避免`锁定`，并允许多个用户并行访问。
请注意，`锁定`不是引用计数的代替。**锁定用于保持数据结构的一致性，而引用计数是一种内存管理技术**。两者通常都是必须的，不要相互混淆。
当存在不同的用户时，许多数据结构可以具有两个级别的引用计数。子类计数计算子类用户的数量，并在子类计数变为0时，仅减少一次全局计数。
这种`多级引用计数`可以再内存管理(struct mm_struct:mm_users/mm_count)和文件系统(struct super_block: s_count/s_active)中找到。
请记住：如果另一个线程可以找到你的数据结构，并且你没有进行引用计数，那么几乎可以肯定会有错误！

## 12) 宏、枚举和RTL
宏和枚举(enum)标签的名称是大写的。如：
```c
#define CONSTANT 0x12345
```
在定义几个相关常量时，首选枚举。
通常，内联函数优于类型函数的宏。
具有多个语句的宏应该包含在do-while块中：
```c
#define macrofun(a, b, c)			\
	do {					\
		if (a == 5)			\
			do_this(b, c);		\
	} while (0)
```
使用宏时要注意的事项：
* 影响控制流程的宏：是**非常**糟糕的。
```c
#define FOO(x)					\
	do {					\
		if (blah(x) < 0)		\
			return -EBUGGERED;	\
	} while (0)
```
它看起来像一个函数，但退出了调用函数。不要破坏那些将读取代码的人的内部解析器。【？？？】
* 依赖于具有魔术名称的局部变量的宏：
```c
#define FOO(val) bar(index, val)
```
* 带有用作左值的参数的宏：`FOO(x) = y`，如果某人将宏转为内联函数，那么他可能会打死你。
* 忘记优先级：使用表达式定义常量的宏必须将表达式括在括号中。（使用参数的宏也要注意类似问题）
```c
#define CONSTANT 0x4000
#define CONSTEXP (CONSTANT | 3)
```
* 在类似函数的宏中定义局部变量时的命名空间冲突：【？？？】
```c
#define FOO(x)				\
({					\
	typeof(x) ret;			\
	ret = calc_ret(x);		\
	(ret);				\
})
```
ret是局部变量的通用名称 - __foo_ret不太可能与现有变量发生冲突。
cpp手册详尽地讨论了宏。gcc internals手册还介绍了RTL，它在内核中经常与汇编语言一起使用。


## 13) 打印内核消息
不要使用`dont`，使用`do not`或`don't`，使消息简洁，清晰 ，明确。
内核消息不必以句号结尾。
Printing numbers in parentheses (%d) adds no value and should be avoided.【？？？】
你应该使用<linux/device.h>中的`驱动程序模型诊断宏`来确保消息与正确的设备和驱动程序匹配，并使用正确的级别标记：`dev_err(), dev_warn(), dev_info()`
对于与特定设备无关的消息，在<linux/printk.h>中定义：`pr_notice(), pr_info(), pr_warn(), pr_err()`等。
提供良好的调试信息可能是一个很大的挑战；一旦你拥有他们，它们可以为远程故障排除提供巨大的帮助。但是调试消息打印的处理方式与打印其他非调试消息的方式不同。
当其他`pr_xxx()`无条件打印是，`pr_debug()`不会；默认情况下调试信息不会被编译进来，除非定义了`DEBUG`或者设置了`CONFIG_DYNAMIC_DEBUG`。对于`dev_dbg()`也是如此。并且相关约定使用VERBOSE_DEBUG将dev_vdbg()消息添加到已由DEBUG启用的消息中。
许多子系统都有Kconfig调试选项，可以再响应的Makefile中打开`-DDEBUG`；在其他情况在特定文件中`#define DEBUG`。
当一条debug消息应该被无条件打印时，例如，如果它已经在调试相关的`#ifdef`中，可以使用`printk(KERN_DEBUG...)`。


## 14) 分配内存
内核提供以下通用内存分配器：`kmalloc()/kzalloc()/kmalloc_array()/kcalloc()/vmalloc()/vzalloc()`。详见`Documentation/core-api/memory-allocation.rst<memory_allocation>`
传递struct大小的首选形式如下：
```c
p = kmalloc(sizeof(*p), ...);
```
拼写出struct名称会损害可读性，并且当类型发生改变时，可能会引发错误。（忘记同步修改的话）
返回值是`void *`时类型转换是多余的，C语言保证从`void *`到任何其他指针的类型转换。
分配数组的首选形式是：
```c
p = kmalloc_array(n, sizeof(...), ...);
```
分配归零阵列的首选形式是：
```c
p = kcalloc(n, sizeof(...), ...);
```
两种形式都检查分配大小`n * sizeof(...)`上的溢出，如果发生则返回NULL。
这些通用分配函数在没有`__GFP_NOWARN`的情况下使用时，都会在发生故障时发出堆栈转储，因此在返回NULL时没有发出额外的失败消息。


## 15) 内联危害
似乎有一种常见的误解，即gcc有一个神奇的“让我更快”的加速选项inline。
虽然使用内联可能是合适的（例如，作为替换宏的一种方法，请参阅第12章），但通常不是。
大量使用内联关键字会导致更大的内核，从而导致整个系统整体运行速度变慢，因为CPU的`icache`占用空间更大、页面缓存的可用内存更少。
考虑一下：页面缓存未命中导致磁盘搜索，这很容易花费5毫秒。5毫秒包含**很多**的cpu周期。
一个合理的经验法则是：**内联函数不超过3行代码。**
此规则的一个例外是：已知参数是编译时常量的情况，并且由于此常量，您知道编译器将能够在编译时优化大部分函数。有关后一种情况的一个很好的示例，请参阅`kmalloc()内联函数`。
通常人们争辩说，将静态且仅使用一次的函数进行内联总是一个胜利，因为没有空间权衡。
虽然这在技术上是正确的，但是gcc能够在没有帮助的情况下自动内联这些内容，并且当第二个用户(二次使用)出现时删除内联的维护问题，超过了告诉gcc无论如何都要做的事情的暗示的潜在价值。

## 16) 函数返回值和命名
函数可以返回许多不同类型的值，其中一个最常见的是指示函数是成功还是失败的值。
这样的值可以表示为错误代码整数(-Exxx=失败，0=成功)或succeeded布尔值(0=失败，非零=成功)。
混合这两种表示形式是难以发现的错误的肥沃来源。
如果C语言包含整数和布尔值之间的强烈区分，那么编译器会为我们发现这些错误...但事实并非如此。为了帮助防止此类错误，请始终遵循以下约定：
> 如果函数的名称是操作或命令性命令，该函数应返回错误代码整数。
> 如果名字是一个谓词，该函数应该返回一个“成功”的布尔值。

例如，
add_work()是一个命令，返回0表示成功，-EBUSY表示失败。
pci_dev_present()是谓词，如果成功找到匹配的设备，返回1，否则返回0。
所有`EXPORTed函数/公共函数`都**必须**遵守此约定。`私有(静态)函数`不需要，但**建议**这样做。
返回值是`计算的实际结果`而不是`计算是否成功`的函数不受此规则的约束。通常它们通过返回一些超出范围的结果来指示失败。
典型的例子是返回指针的函数; 他们使用NULL或ERR_PTR机制来报告失败。

## 17) 使用bool
Linux内核`bool`类型是C99`_Bool`类型的别名。bool值只能求值为0或1，而对bool的隐式或显式转换会自动将值转换为true或false。
当使用bool类型!!不需要构造，这消除了一类错误。【？？？】
使用bool值时，应使用true和false定义而不是1和0。
可以在适当的时候使用bool函数返回类型和堆栈变量。鼓励使用bool来提高可读性，存储布尔值(true/flase)时，`bool`通常是比`int`更好地选择。
如果缓存行布局或值的大小很重要，请不要使用bool，因为其大小和对齐方式因编译的体系结构而异。针对对齐和尺寸优化的结构不应使用bool。
如果struct具有许多true/false值，请考虑将它们作为1位成员合并到的位域中，或使用适当的固定宽度类型（例如u8）。
函数参数也是类似的，`许多true/false`值可以合并为`单个按位flags`参数，如果调用点具有裸真/假常量(naked true/false constants)，则flags通常可以是更可读的替代。
在结构和参数中有限制地使用bool可以提高可读性。

## 18) 不要重新发明内核宏
头文件`include/linux/kernel.h`包含许多你应该使用的宏，而不是自己编写它们的一些变体。
例如，如果需要计算数组的长度，请使用宏：
```c
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
```
同样，如果需要计算某些结构成员的大小，请使用：
```c
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
```
如果需要，还有min()和max()宏可以进行严格的类型检查。请仔细阅读该头文件，看看还有哪些已定义，你不应该在代码中重复实现。

## 19) 编辑器模式行和其他需要啰嗦的事情(Editor modelines and other cruft)
Some editors can interpret configuration information embedded in source files,
indicated with special markers.  For example, emacs interprets lines marked
like this:
有一些编辑器可以解释嵌入在源文件里的由一些特殊标记标明的配置信息。比如，emacs 
能够解释被标记成这样的行：
```c
-*- mode: c -*-
```
或者像这样的：
```c
/*
Local Variables:
compile-command: "gcc -DMAGIC_DEBUG_FLAG foo.c"
End:
*/
```
Vim 能够解释这样的标记：
```c
/* vim:set sw=8 noet */
```
Do not include any of these in source files.  People have their own personal
editor configurations, and your source files should not override them.  This
includes markers for indentation and mode configuration.  People may use their
own custom mode, or may have some other magic method for making indentation
work correctly.
不要在源代码中包含任何这样的内容。每个人都有他自己的编辑器配置，你的源文件不 
应该覆盖别人的配置。这包括有关缩进和模式配置的标记。人们可以使用他们自己定制 
的模式，或者使用其他可以产生正确的缩进的巧妙方法。

## 20) 内联汇编(Inline assembly)
In architecture-specific code, you may need to use inline assembly to interface
with CPU or platform functionality.  Don't hesitate to do so when necessary.
However, don't use inline assembly gratuitously when C can do the job.  You can
and should poke hardware from C when possible.
在特定架构的代码中，你可能需要内联汇编与 CPU 和平台相关功能连接。需要这么做时 
就不要犹豫。然而，当 C 可以完成工作时，不要平白无故地使用内联汇编。在可能的情 
况下，你可以并且应该用 C 和硬件沟通。

Consider writing simple helper functions that wrap common bits of inline
assembly, rather than repeatedly writing them with slight variations.  Remember
that inline assembly can use C parameters.
请考虑去写捆绑通用位元 (wrap common bits) 的内联汇编的简单辅助函数，别去重复 
地写下只有细微差异内联汇编。记住内联汇编可以使用 C 参数。

Large, non-trivial assembly functions should go in .S files, with corresponding
C prototypes defined in C header files.  The C prototypes for assembly
functions should use ``asmlinkage``.
大型，有一定复杂度的汇编函数应该放在`.S`文件内，用相应的 C 原型定义在 C 头文 
件中。汇编函数的 C 原型应该使用`asmlinkage`。


You may need to mark your asm statement as volatile, to prevent GCC from
removing it if GCC doesn't notice any side effects.  You don't always need to
do so, though, and doing so unnecessarily can limit optimization.
你可能需要把汇编语句标记为`volatile`，用来阻止 GCC 在没发现任何副作用后就把它 
移除了。你不必总是这样做，尽管，这不必要的举动会限制优化。

When writing a single inline assembly statement containing multiple
instructions, put each instruction on a separate line in a separate quoted
string, and end each string except the last with ``\n\t`` to properly indent
the next instruction in the assembly output:
在写一个包含多条指令的单个内联汇编语句时，把每条指令用引号分割而且各占一行， 
除了最后一条指令外，在每个指令结尾加上`\n\t`，让汇编输出时可以正确地缩进下一条 
指令：
```
asm ("magic %reg1, #42\n\t"
     "more_magic %reg2, %reg3"
     : /* outputs */ : /* inputs */ : /* clobbers */);
```


## 21) 条件编译(Conditional Compilation)
Wherever possible, don't use preprocessor conditionals (#if, #ifdef) in .c
files; doing so makes code harder to read and logic harder to follow.  Instead,
use such conditionals in a header file defining functions for use in those .c
files, providing no-op stub versions in the #else case, and then call those
functions unconditionally from .c files.  The compiler will avoid generating
any code for the stub calls, producing identical results, but the logic will
remain easy to follow.
只要可能，就**不要**在 .c 文件里面使用预处理条件`#if, #ifdef`；这样做让代码更难 
阅读并且更难去跟踪逻辑。替代方案是，在头文件中用预处理条件提供给那些 .c 文件 
使用，再给 #else 提供一个空桩 (no-op stub) 版本，然后在 .c 文件内无条件地调用 
那些 (定义在头文件内的) 函数。这样做，编译器会避免为桩函数 (stub) 的调用生成 
任何代码，产生的结果是相同的，但逻辑将更加清晰。

Prefer to compile out entire functions, rather than portions of functions or
portions of expressions.  Rather than putting an ifdef in an expression, factor
out part or all of the expression into a separate helper function and apply the
conditional to that function.
最好倾向于编译整个函数，而不是函数的一部分或表达式的一部分。与其放一个 ifdef 
在表达式内，不如分解出部分或全部表达式，放进一个单独的辅助函数，并应用预处理 
条件到这个辅助函数内。

If you have a function or variable which may potentially go unused in a
particular configuration, and the compiler would warn about its definition
going unused, mark the definition as __maybe_unused rather than wrapping it in
a preprocessor conditional.  (However, if a function or variable *always* goes
unused, delete it.)
如果你有一个在特定配置中，可能变成未使用的函数或变量，编译器会警告它定义了但 
未使用，把它标记为 __maybe_unused 而不是将它包含在一个预处理条件中。(然而，如 
果一个函数或变量总是未使用，就直接删除它。)

Within code, where possible, use the IS_ENABLED macro to convert a Kconfig
symbol into a C boolean expression, and use it in a normal C conditional:
在代码中，尽可能地使用`IS_ENABLED`宏来转化某个 Kconfig 标记为 C 的布尔 
表达式，并在一般的 C 条件中使用它：
```c
if (IS_ENABLED(CONFIG_SOMETHING)) {
    ...
}
```


The compiler will constant-fold the conditional away, and include or exclude
the block of code just as with an #ifdef, so this will not add any runtime
overhead.  However, this approach still allows the C compiler to see the code
inside the block, and check it for correctness (syntax, types, symbol
references, etc).  Thus, you still have to use an #ifdef if the code inside the
block references symbols that will not exist if the condition is not met.
编译器会做常量折叠，然后就像使用`#ifdef`那样去包含或排除代码块，所以这不会带 
来任何运行时开销。然而，这种方法依旧允许 C 编译器查看块内的代码，并检查它的正 
确性 (语法，类型，符号引用，等等)。因此，如果条件不满足，代码块内的引用符号就 
不存在时，你还是必须去用`#ifdef`。

At the end of any non-trivial #if or #ifdef block (more than a few lines),
place a comment after the #endif on the same line, noting the conditional
expression used.  For instance:
在任何有意义的`#if或#ifdef`块的末尾 (超过几行的)，在`#endif`同一行的后面写下 注释，注释这个条件表达式。例如：
```c
#ifdef CONFIG_SOMETHING
...
#endif /* CONFIG_SOMETHING */
```


##  附录I) 参考(References)
----------------------
The C Programming Language, Second Edition
by Brian W. Kernighan and Dennis M. Ritchie.
Prentice Hall, Inc., 1988.
ISBN 0-13-110362-8 (paperback), 0-13-110370-9 (hardback).

The Practice of Programming
by Brian W. Kernighan and Rob Pike.
Addison-Wesley, Inc., 1999.
ISBN 0-201-61586-X.

GNU manuals - where in compliance with K&R and this text - for cpp, gcc,
gcc internals and indent, all available from http://www.gnu.org/manual/

WG14 is the international standardization working group for the programming
language C, URL: http://www.open-std.org/JTC1/SC22/WG14/

Kernel :ref:`process/coding-style.rst <codingstyle>`, by greg@kroah.com at OLS 2002:
http://www.kroah.com/linux/talks/ols_2002_kernel_codingstyle_talk/html/