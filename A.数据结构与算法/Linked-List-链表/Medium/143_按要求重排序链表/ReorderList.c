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
    void reorderList(ListNode* head) {
        if (head == NULL)
            return ;
            
        int listLen = getListLength(head);
        int mid = listLen / 2;
        ListNode *pNode = head;
        
        for (int i = 0; i < mid; i++)       //
        {
            pNode = pNode->next;
        }
        ListNode *pRear = reverseList(pNode);
        ListNode *pFront = head;
        ListNode *tempNode = NULL;
        while(pFront != pRear && pRear != NULL && pRear->next != NULL)
        {
            tempNode = pRear->next;
            pRear->next = pFront->next;
            pFront->next = pRear;
            pFront = pRear->next;
            pRear = tempNode;
        }
    }
    
    int getListLength(ListNode *head)
    {
        int count = 0;
        while(head != NULL)
        {
            count++;
            head= head->next;
        }
        
        return count;
    }
    
    ListNode * reverseList(ListNode *pHead)
    {
        if (pHead == NULL)
            return NULL;
            
        ListNode *pBefore = NULL;;
        ListNode *pMid = NULL;
        ListNode *pBehind = NULL;
        
        pBefore =pHead;
        pMid = pBefore->next;
        pBefore->next = NULL;       //±ÜÃâ»·
        
        while(pMid != NULL)
        {
            pBehind = pMid->next;
            pMid->next = pBefore;
            pBefore = pMid;
            pMid = pBehind;
        }
        
        return pBefore;
    }
};