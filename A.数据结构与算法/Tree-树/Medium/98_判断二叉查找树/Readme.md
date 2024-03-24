题目地址：https://leetcode.com/problems/validate-binary-search-tree/  
题目描述：判断一棵二叉树是否是二叉查找树，是返回 true，否则返回 false。  
题目思路：中序遍历的同时，用一个指针记录前一个节点。指针指向的前一个节点和当前节点比较，如果前一个节点>当前节点，则返回 false。  
