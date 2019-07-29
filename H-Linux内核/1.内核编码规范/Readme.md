[TOC]

# 内核编码规范

这是一个描述Linux内核完美编码风格的小文档。


## 1) 缩进
`Tabs`是`8`个字符，因此缩进也是8个字符。
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


14) Allocating memory
---------------------

The kernel provides the following general purpose memory allocators:
kmalloc(), kzalloc(), kmalloc_array(), kcalloc(), vmalloc(), and
vzalloc().  Please refer to the API documentation for further information
about them.  :ref:`Documentation/core-api/memory-allocation.rst
<memory_allocation>`

The preferred form for passing a size of a struct is the following:

.. code-block:: c

	p = kmalloc(sizeof(*p), ...);

The alternative form where struct name is spelled out hurts readability and
introduces an opportunity for a bug when the pointer variable type is changed
but the corresponding sizeof that is passed to a memory allocator is not.

Casting the return value which is a void pointer is redundant. The conversion
from void pointer to any other pointer type is guaranteed by the C programming
language.

The preferred form for allocating an array is the following:

.. code-block:: c

	p = kmalloc_array(n, sizeof(...), ...);

The preferred form for allocating a zeroed array is the following:

.. code-block:: c

	p = kcalloc(n, sizeof(...), ...);

Both forms check for overflow on the allocation size n * sizeof(...),
and return NULL if that occurred.

These generic allocation functions all emit a stack dump on failure when used
without __GFP_NOWARN so there is no use in emitting an additional failure
message when NULL is returned.

15) The inline disease
----------------------

There appears to be a common misperception that gcc has a magic "make me
faster" speedup option called ``inline``. While the use of inlines can be
appropriate (for example as a means of replacing macros, see Chapter 12), it
very often is not. Abundant use of the inline keyword leads to a much bigger
kernel, which in turn slows the system as a whole down, due to a bigger
icache footprint for the CPU and simply because there is less memory
available for the pagecache. Just think about it; a pagecache miss causes a
disk seek, which easily takes 5 milliseconds. There are a LOT of cpu cycles
that can go into these 5 milliseconds.

A reasonable rule of thumb is to not put inline at functions that have more
than 3 lines of code in them. An exception to this rule are the cases where
a parameter is known to be a compiletime constant, and as a result of this
constantness you *know* the compiler will be able to optimize most of your
function away at compile time. For a good example of this later case, see
the kmalloc() inline function.

Often people argue that adding inline to functions that are static and used
only once is always a win since there is no space tradeoff. While this is
technically correct, gcc is capable of inlining these automatically without
help, and the maintenance issue of removing the inline when a second user
appears outweighs the potential value of the hint that tells gcc to do
something it would have done anyway.


16) Function return values and names
------------------------------------

Functions can return values of many different kinds, and one of the
most common is a value indicating whether the function succeeded or
failed.  Such a value can be represented as an error-code integer
(-Exxx = failure, 0 = success) or a ``succeeded`` boolean (0 = failure,
non-zero = success).

Mixing up these two sorts of representations is a fertile source of
difficult-to-find bugs.  If the C language included a strong distinction
between integers and booleans then the compiler would find these mistakes
for us... but it doesn't.  To help prevent such bugs, always follow this
convention::

	If the name of a function is an action or an imperative command,
	the function should return an error-code integer.  If the name
	is a predicate, the function should return a "succeeded" boolean.

For example, ``add work`` is a command, and the add_work() function returns 0
for success or -EBUSY for failure.  In the same way, ``PCI device present`` is
a predicate, and the pci_dev_present() function returns 1 if it succeeds in
finding a matching device or 0 if it doesn't.

All EXPORTed functions must respect this convention, and so should all
public functions.  Private (static) functions need not, but it is
recommended that they do.

Functions whose return value is the actual result of a computation, rather
than an indication of whether the computation succeeded, are not subject to
this rule.  Generally they indicate failure by returning some out-of-range
result.  Typical examples would be functions that return pointers; they use
NULL or the ERR_PTR mechanism to report failure.


17) Using bool
--------------

The Linux kernel bool type is an alias for the C99 _Bool type. bool values can
only evaluate to 0 or 1, and implicit or explicit conversion to bool
automatically converts the value to true or false. When using bool types the
!! construction is not needed, which eliminates a class of bugs.

When working with bool values the true and false definitions should be used
instead of 1 and 0.

bool function return types and stack variables are always fine to use whenever
appropriate. Use of bool is encouraged to improve readability and is often a
better option than 'int' for storing boolean values.

Do not use bool if cache line layout or size of the value matters, as its size
and alignment varies based on the compiled architecture. Structures that are
optimized for alignment and size should not use bool.

If a structure has many true/false values, consider consolidating them into a
bitfield with 1 bit members, or using an appropriate fixed width type, such as
u8.

Similarly for function arguments, many true/false values can be consolidated
into a single bitwise 'flags' argument and 'flags' can often be a more
readable alternative if the call-sites have naked true/false constants.

Otherwise limited use of bool in structures and arguments can improve
readability.

18) Don't re-invent the kernel macros
-------------------------------------

The header file include/linux/kernel.h contains a number of macros that
you should use, rather than explicitly coding some variant of them yourself.
For example, if you need to calculate the length of an array, take advantage
of the macro

.. code-block:: c

	#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

Similarly, if you need to calculate the size of some structure member, use

.. code-block:: c

	#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))

There are also min() and max() macros that do strict type checking if you
need them.  Feel free to peruse that header file to see what else is already
defined that you shouldn't reproduce in your code.


19) Editor modelines and other cruft
------------------------------------

Some editors can interpret configuration information embedded in source files,
indicated with special markers.  For example, emacs interprets lines marked
like this:

.. code-block:: c

	-*- mode: c -*-

Or like this:

.. code-block:: c

	/*
	Local Variables:
	compile-command: "gcc -DMAGIC_DEBUG_FLAG foo.c"
	End:
	*/

Vim interprets markers that look like this:

.. code-block:: c

	/* vim:set sw=8 noet */

Do not include any of these in source files.  People have their own personal
editor configurations, and your source files should not override them.  This
includes markers for indentation and mode configuration.  People may use their
own custom mode, or may have some other magic method for making indentation
work correctly.


20) Inline assembly
-------------------

In architecture-specific code, you may need to use inline assembly to interface
with CPU or platform functionality.  Don't hesitate to do so when necessary.
However, don't use inline assembly gratuitously when C can do the job.  You can
and should poke hardware from C when possible.

Consider writing simple helper functions that wrap common bits of inline
assembly, rather than repeatedly writing them with slight variations.  Remember
that inline assembly can use C parameters.

Large, non-trivial assembly functions should go in .S files, with corresponding
C prototypes defined in C header files.  The C prototypes for assembly
functions should use ``asmlinkage``.

You may need to mark your asm statement as volatile, to prevent GCC from
removing it if GCC doesn't notice any side effects.  You don't always need to
do so, though, and doing so unnecessarily can limit optimization.

When writing a single inline assembly statement containing multiple
instructions, put each instruction on a separate line in a separate quoted
string, and end each string except the last with ``\n\t`` to properly indent
the next instruction in the assembly output:

.. code-block:: c

	asm ("magic %reg1, #42\n\t"
	     "more_magic %reg2, %reg3"
	     : /* outputs */ : /* inputs */ : /* clobbers */);


21) Conditional Compilation
---------------------------

Wherever possible, don't use preprocessor conditionals (#if, #ifdef) in .c
files; doing so makes code harder to read and logic harder to follow.  Instead,
use such conditionals in a header file defining functions for use in those .c
files, providing no-op stub versions in the #else case, and then call those
functions unconditionally from .c files.  The compiler will avoid generating
any code for the stub calls, producing identical results, but the logic will
remain easy to follow.

Prefer to compile out entire functions, rather than portions of functions or
portions of expressions.  Rather than putting an ifdef in an expression, factor
out part or all of the expression into a separate helper function and apply the
conditional to that function.

If you have a function or variable which may potentially go unused in a
particular configuration, and the compiler would warn about its definition
going unused, mark the definition as __maybe_unused rather than wrapping it in
a preprocessor conditional.  (However, if a function or variable *always* goes
unused, delete it.)

Within code, where possible, use the IS_ENABLED macro to convert a Kconfig
symbol into a C boolean expression, and use it in a normal C conditional:

.. code-block:: c

	if (IS_ENABLED(CONFIG_SOMETHING)) {
		...
	}

The compiler will constant-fold the conditional away, and include or exclude
the block of code just as with an #ifdef, so this will not add any runtime
overhead.  However, this approach still allows the C compiler to see the code
inside the block, and check it for correctness (syntax, types, symbol
references, etc).  Thus, you still have to use an #ifdef if the code inside the
block references symbols that will not exist if the condition is not met.

At the end of any non-trivial #if or #ifdef block (more than a few lines),
place a comment after the #endif on the same line, noting the conditional
expression used.  For instance:

.. code-block:: c

	#ifdef CONFIG_SOMETHING
	...
	#endif /* CONFIG_SOMETHING */


Appendix I) References
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