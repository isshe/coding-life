题目地址：https://leetcode.com/problems/partition-list/    
题目描述：给一个链表和一个数n， 把链表改成小于n的在左边，大于等于在右边。  
思路：  
1. 用两个节点（是节点不是指针）a, b。  
2. 把小于n的连在a-next下边，大于等于连在b->next下边。  
3. 最后把a->next为头结点的链表和b->next为头结点的链表连接起来，即可。    
  