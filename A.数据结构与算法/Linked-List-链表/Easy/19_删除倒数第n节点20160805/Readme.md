题目：删除链表中倒数第 n 个节点。
题目描述：
Given a linked list, remove the nth node from the end of list and return its head.
For example，
   Given linked list: 1->2->3->4->5, and n = 2.
   After removing the second node from the end, the linked list becomes 1->2->3->5.
Note:
    Given n will always be valid.
    Try to do this in one pass.

思路：找到倒数第 n+1 个节点 pNode, pNode->next = pNode->next->next;
注意：
1. 处理一个节点和没有节点的情况。
2. 处理 n 大于链表个数的情况。
3. 处理删除的是头节点的情况。（删除尾节点不用特殊处理）
4. 

