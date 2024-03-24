题目地址：https://leetcode.com/problems/partition-list/
题目描述：给一个链表和一个数 n，把链表改成小于 n 的在左边，大于等于在右边。
思路：
1. 用两个节点（是节点不是指针）a, b。
2. 把小于 n 的连在 a-next 下边，大于等于连在 b->next 下边。
3. 最后把 a->next 为头结点的链表和 b->next 为头结点的链表连接起来，即可。
