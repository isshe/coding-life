/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RemoveNthNode.cpp
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (head == NULL || head->next == NULL || n <= 0)       //0或1个节点，直接返回空链表
            return NULL;
            
        ListNode *pNode = head;
        ListNode *pRear = head;
        int i = 0;
        for (i = 0; i < n+1 && pRear != NULL; i++)
        {
            pRear = pRear->next;
        }
        
        if (i == n)                 //如果删除的是头节点，可能要释放。
        {
            return head->next;
        }
        else if (i != n + 1)         //处理n大于链表长度的情况
        {
            return NULL;
        }
        
        while(pRear != NULL)
        {
            pRear = pRear->next;
            pNode = pNode->next;
        }
        
        pNode->next = pNode->next->next;
        //要不要释放一下呢？delete。
    
        return head;
    }
};
