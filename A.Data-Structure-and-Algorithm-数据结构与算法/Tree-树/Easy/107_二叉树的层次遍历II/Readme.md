题目地址：https://leetcode.com/problems/binary-tree-level-order-traversal-ii/
题目描述：层次遍历二叉树，倒序并分层输出。
就是：
 1
 23
 4567
输出为：
 4567
 23
 1
思路：最直观的思路是先进行题目103的层次遍历，再倒序。
1. 如103
2. 倒序可以用多一个辅助空间反序复制 或者 直接在结果中翻转。

