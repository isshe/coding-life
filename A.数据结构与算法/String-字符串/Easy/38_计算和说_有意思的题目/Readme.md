题目地址：https://leetcode.com/problems/count-and-say/
题目描述：给一个数 n，输出第 n 个字符串
字符串如：1, 11, 21, 1211, 111221, ...
输入 1，返回 1，输入 2，返回 11.
特点是：11 是 1（前一个字符串）个 1，21 是 2 个 1。
也就是计算前一个字符串的元素个数当作当前字符串的内容。

思路：用循环或递归。
