/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : MergeTwoSortedList.cpp
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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l1 == NULL)
        {
            return l2;
        }
        if (l2 == NULL)
        {
            return l1;
        }
        
        ListNode *head = NULL;
        ListNode *pNode1 = NULL;
        ListNode *pNode2 = NULL;
        ListNode *listEnd = NULL;
        if (l1->val < l2->val)
        {
            head = l1;
            pNode1 = l1->next;
            pNode2 = l2;
//            head->next = l2;
        }
        else
        {
            head = l2;
            pNode1 = l1;
            pNode2 = l2->next;
//            head->next = l1;
        }
        listEnd = head;
        
        while(pNode1 != NULL || pNode2 != NULL)
        {
            if (pNode1 == NULL)
            {
                listEnd->next = pNode2;
                break;
            }
            else if (pNode2 == NULL)
            {
                listEnd->next = pNode1;
                break;
            }
            else if (pNode1->val < pNode2->val)
            {
                listEnd->next = pNode1;
                listEnd = listEnd->next;
                pNode1 = pNode1->next;
            }
            else
            {
                listEnd->next = pNode2;
                listEnd = listEnd->next;
                pNode2 = pNode2->next;
                
            }
        }
        
        return head;
    }
};
