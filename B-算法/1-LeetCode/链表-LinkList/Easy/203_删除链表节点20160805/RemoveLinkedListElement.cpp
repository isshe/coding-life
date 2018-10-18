/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RemoveLinkedListElement.cpp
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
    ListNode* removeElements(ListNode* head, int val) {
        if (head == NULL)
            return head;
            
        ListNode *pNode = NULL;
        ListNode *currentNode = NULL;
        
        //删除头节点
        pNode = head;
        while(pNode != NULL && pNode->val == val)
        {
            head = pNode->next;
            delete pNode;
            pNode = head;
        }
        if (head != NULL)
        {
            currentNode = head->next;
        }
        while(currentNode != NULL)
        {
            if (currentNode->val == val)
            {
                pNode->next = currentNode->next;
                delete currentNode;
                currentNode = pNode->next;
            }
            else
            {
                pNode = pNode->next;
                currentNode = currentNode->next;
            }
        }
        
        return head;
    }
};
