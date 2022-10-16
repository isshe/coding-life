/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

// 空间换时间的头插。
// 用一个节点作为头部，把整个链表翻转。
struct ListNode* reverseList(struct ListNode* head){
    struct ListNode temp_head;
    struct ListNode *ptemp_head = &temp_head;
    struct ListNode *temp = NULL;

    temp_head.next = NULL;
    while (head) {
        temp = head->next;
        head->next = ptemp_head->next;
        ptemp_head->next = head;
        head = temp;
    }

    return ptemp_head->next;

}
