题目地址：https://leetcode.com/problems/rotate-list/  
题目描述：给一个链表和一个数 k，翻转链表右边 k 个节点。
例如：1-2-3-4-5-NULL，k=2，返回：4-5-1-2-3-NULL 
思路：
1. 求链表长度 len, k = len % k;
2. 像求链表倒数第 k 个节点那样，找到倒数第 k+1 个节点，有 k+1->next = k;
3. 把 k+1->next = NULL, 找后半段链表的最后一个节点 end, 然后 end->next = head 即可。