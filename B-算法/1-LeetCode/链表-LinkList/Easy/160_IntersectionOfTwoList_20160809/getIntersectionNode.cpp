/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : getIntersectionNode.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/09
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == NULL || headB == NULL)
        {
            return NULL;
        }
        
        int lenA = getListLength(headA);
        int lenB = getListLength(headB);
        int more = lenA > lenB ? (lenA - lenB) : (lenB - lenA);
        ListNode *pNodeA = headA;
        ListNode *pNodeB = headB;
        if (lenA > lenB)
        {
            pNodeA = goNStep(headA, more);
        }
        else
        {
            pNodeB = goNStep(headB, more);
        }
        
        while(pNodeA != NULL || pNodeB != NULL)
        {
            if (pNodeA == pNodeB)
            {
                return pNodeA;
            }
            pNodeA = pNodeA->next;
            pNodeB = pNodeB->next;
        }
        
        return NULL;
    }
    
    int getListLength(ListNode *head)
    {
        int len = 0;
        while(head)
        {
            len++;
            head = head->next;
        }
        
        return len;
    }
    
    ListNode *goNStep(ListNode *head, int step)
    {
        for (int i = 0; i < step; i++)
        {
            head = head->next;
        }
        
        return head;
    }
};
