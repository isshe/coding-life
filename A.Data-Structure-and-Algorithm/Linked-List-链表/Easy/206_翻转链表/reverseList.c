/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


struct ListNode* reverseList(struct ListNode* head){
    struct ListNode *a = NULL;
    struct ListNode *b = NULL;
    struct ListNode *c = NULL;

    a = head;

    if (head) {
        b = head->next;
        head->next = NULL;
    }

    while (b) {
        c = b->next;
        b->next = a;
        a = b;
        b = c;
    }

    return a;
}
