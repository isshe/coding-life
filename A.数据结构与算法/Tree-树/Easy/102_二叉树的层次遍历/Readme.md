题目地址：https://leetcode.com/problems/binary-tree-level-order-traversal/
题目描述：层次遍历二叉树并且分行打印。（分行打印是难点）
思路：普通层次遍历是用一个队列，要求分行打印，可以用两个队列来实现。
1. 先把 root 节点放入 queueu1 队列中，
2. quueue1 出队一个节点，把它的子节点（也就是下一层）放在另一个队列 queueu2。当 queue1 为空以后，打印这一层。
3. 轮到 queue2 出队。把子节点存 queue1 中。如此反复。
