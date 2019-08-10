/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

// 找到最后一个节点，当做新的头结点
// 进行头插
struct ListNode* reverseList(struct ListNode* head){
    struct ListNode *new_head = head;

    if (!head) {
        return head;
    }

    while (new_head->next) {
        new_head = new_head->next;
    }

    struct ListNode *cur = head;
    struct ListNode *cur_next = NULL;
    while (cur != new_head) {
        cur_next = cur->next;
        cur->next = new_head->next;
        new_head->next = cur;
        cur = cur_next;

    }

    return new_head;
}
