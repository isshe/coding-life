题目地址：https://leetcode.com/problems/kth-smallest-element-in-a-bst/  
题目描述：给一棵 BST 树和一个 k。返回 BST 树中第 k 小的元素。  
思路：中序遍历，同时判断该节点是否是第 k 个节点，是则返回。  
