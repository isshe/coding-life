题目地址：https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/   
题目描述：删除链表中值重复的节点。  
例如：1->2->3->3->5, 返回：1->2->5;  
例如：1->1->2->3, 返回：2->3   
思路：如果某节点值只有一个，则放在目标链表下面后面，否则略过该值的所有节点。  