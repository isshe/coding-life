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
    int getListLen(ListNode *head)
    {
        int count = 0;
        while(head != NULL)
        {
            count++;
            head = head->next;
        }
        return count;
    }

    ListNode* rotateRight(ListNode* head, int k) {
        if (head == NULL)               //不用这句就会runtime error!貌似也没有越界啊。
            return head;                
        ListNode *pKnode = head;
        ListNode *pNode = head;
        int i = 0;
        int len = getListLen(head);
        
        
        k = k % len;
        if (k == 0)
        {
            return head;
        }

        for (i = 0; i < k+1 && pNode != NULL; i++)
        {
            pNode = pNode->next;
        }
        
        if (i == k+1)
        {   
            
            while(pNode != NULL)
            {
                pNode = pNode->next;
                pKnode = pKnode->next;

            }

            pNode = pKnode->next;
            pKnode->next = NULL;
            pKnode = pNode;
            while(pNode != NULL)
            {
                if (pNode->next == NULL)
                {
                    pNode->next = head;
                    break;
                }
                pNode = pNode->next;
            }
            
        }
        
        return pKnode;
    }


};