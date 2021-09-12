题目地址：https://leetcode.com/problems/rotate-list/  
题目描述：给一个链表和一个数k， 翻转链表右边k个节点。
例如：1-2-3-4-5-NULL， k=2， 返回：4-5-1-2-3-NULL 
思路：
1. 求链表长度len, k = len % k;
2. 像求链表倒数第k个节点那样，找到倒数第k+1个节点， 有k+1->next = k;
3. 把k+1->next = NULL, 找后半段链表的最后一个节点end, 然后end->next = head即可。