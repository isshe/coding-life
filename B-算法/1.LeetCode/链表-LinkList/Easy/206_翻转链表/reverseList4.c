/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

// 递归法
struct ListNode* reverseList(struct ListNode* head){
    if (!head) {
        return NULL;
    }

    if (!head->next) {
        return head;
    }

    struct ListNode *new_head = reverseList(head->next);
    head->next->next = head;
    head->next = NULL;  // for first Node
    return new_head;
}