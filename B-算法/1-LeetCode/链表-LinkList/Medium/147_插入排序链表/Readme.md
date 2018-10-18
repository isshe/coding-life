题目地址：https://leetcode.com/problems/insertion-sort-list/  
题目描述：用插入法排序链表。  
思路: 一个一个节点取出来插入到排序好的链表中，   
1. 注意取出的节点要pNode->next = NULL（指向的下一节点的指针要去掉，避免影响）  