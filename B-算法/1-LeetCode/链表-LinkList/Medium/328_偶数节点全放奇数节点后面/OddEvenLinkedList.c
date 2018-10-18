/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* oddEvenList(struct ListNode* head) {
    if(head == NULL)
        return NULL;
        
    struct ListNode *oddNode = head;
    struct ListNode *evenHead = head->next;
    struct ListNode *evenNode = evenHead;
    
    while(oddNode != NULL && evenNode != NULL)
    {
        if (evenNode->next != NULL)
        {
            oddNode->next = evenNode->next;
            oddNode = oddNode->next;
        }
        else 
            break;
        
        if (oddNode != NULL)
        {
            evenNode->next = oddNode->next;
        }
        evenNode = evenNode->next;
    }
    oddNode->next = evenHead;
    
    return head;
}