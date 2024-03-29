# 调试

## 相关名词/概念

- dwz："Debugging With Zero"（零调试）的是用于减小可执行文件或共享库大小的工具。它的主要目的是通过优化和压缩调试信息来减少调试文件的大小，而不会损失关键的调试功能。"dwz" 工具可以通过删除冗余的调试信息、合并重复的调试信息和应用其他优化策略，使得调试文件更加紧凑和高效。
- dbg：在软件开发和调试领域中，"dbg" 通常是指 "debug"（调试）的缩写。"dbg" 可以是一个后缀名，用于表示包含调试信息的文件。
- elf（Executable and Linkable Format）：ELF 是一种用于可执行文件、共享库和核心转储文件的标准文件格式。它定义了可执行文件和共享库的结构，包括程序的代码、数据、调试信息、符号表等。
- melf（Modified ELF）：MELF 是一种基于 ELF 文件格式的变种或修改版。
- dwarf：DWARF（Debugging With Arbitrary Record Formats，使用任意记录格式进行调试）是一种用于描述程序调试信息的标准格式。用于在可执行文件、共享库以及调试文件中存储和表示与调试相关的信息。主要用于编译器和调试器之间的交互，以及调试符号表和调试信息的生成和解析。它包含了关于程序的类型、变量、函数、源代码位置和调用关系等方面的信息。通常与 ELF 文件格式一起使用，通过将 DWARF 调试信息嵌入到 ELF 文件中，调试器可以根据这些信息提供更多的上下文和准确的调试支持，例如变量的值、函数调用堆栈和源代码级别的断点调试。
- unwind：指的是从当前函数调用栈中回溯（unwind）到上一层函数的过程。当程序执行到异常或错误情况时，调试器通常需要展开调用栈以了解程序执行的上下文和发生异常的位置。unwind 是根据 eh_frame 段的内容来的，如果这里的内容不对，那么 unwind 就大概率有问题。能 unwind 到 main（从后往前，main 是入口）则说明 unwind 和 eh_frame 的内容正确。
- symbol table：Symbol Table（符号表）是一种数据结构，用于存储程序中的符号信息，如变量、函数、类型等的名称和相关属性。符号表是编译器和调试器在程序编译和调试过程中使用的重要组成部分。
