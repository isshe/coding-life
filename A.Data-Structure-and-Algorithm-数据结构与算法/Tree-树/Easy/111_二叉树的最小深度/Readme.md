题目地址：https://leetcode.com/problems/minimum-depth-of-binary-tree/
题目描述：求一棵二叉树的最小深度
思路：
1. 一个关键点是如何获取第一个遍历的页节点的深度。（这个代码中用的是一个flag，感觉不够好）
2. 保存最小深度到一个变量min中。当遍历到页节点的时候就比较当前深度和min哪个更小。
