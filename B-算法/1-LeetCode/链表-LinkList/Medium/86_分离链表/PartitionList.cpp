/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode smallHead = ListNode(0);
        ListNode bigHead = ListNode(0);
        ListNode *smallRear = &smallHead;
        ListNode *bigRear = &bigHead;
        ListNode *pNode = NULL;
        ListNode *ptemp = NULL;
        
        for (pNode = head; pNode != NULL; )
        {
            if (pNode->val < x)
            {
                smallRear->next = pNode;
                smallRear = smallRear->next;
            }
            else
            {
                bigRear->next = pNode;
                bigRear = bigRear->next;
            }
            ptemp = pNode;
            pNode = pNode->next;
            ptemp->next = NULL;
        }
        
        smallRear->next = bigHead.next;
        head = smallHead.next;
        return head;
    }
};
};