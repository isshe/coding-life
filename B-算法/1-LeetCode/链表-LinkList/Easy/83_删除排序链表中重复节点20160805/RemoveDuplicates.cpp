/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RemoveDuplicates.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/05
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
    ListNode* deleteDuplicates(ListNode* head) {
        if (head == NULL)
            return NULL;
            
        ListNode *pNode = head;
        ListNode *pNext = head->next;
        
        while(pNext != NULL)
        {
            if (pNode->val == pNext->val)
            {
                pNode->next = pNext->next;
                delete pNext;
                pNext = pNode->next;
            }
            else
            {
                pNode = pNext;
                pNext = pNext->next;
            }
        }
        return head;
    }
};
