题目地址：https://leetcode.com/problems/binary-tree-right-side-view/  
题目描述：返回一棵二叉树的右视图。  
思路：使用右中序遍历，当树高高于结果数组 res 长度 reslen 的时候，则更新 reslen 并压入数据的 res。
