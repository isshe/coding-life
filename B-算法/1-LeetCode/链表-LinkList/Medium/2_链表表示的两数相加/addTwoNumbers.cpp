/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : addTwoNumbers.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/22
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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        if (l1 == NULL)
            return l2;
        if (l2 == NULL)
            return l1;
        
        ListNode head = ListNode(0);
        ListNode *pBefore = &head;
        ListNode *newNode = NULL;
        int carry = 0;
        int sum = 0;
        int l1Val = 0;
        int l2Val = 0;
        ListNode *l1pNode = l1; //reverseList(l1);
        ListNode *l2pNode = l2; //reverseList(l2);
        
        //注意carry也放这里判断了!
        while( l1pNode != NULL || l2pNode != NULL || carry != 0 )
        {
            if (l1pNode == NULL)
            {
                l1Val = 0;
            }
            else
            {
                l1Val = l1pNode->val;
            }
            
            if (l2pNode == NULL)
            {
                l2Val = 0;
            }
            else
            {
                l2Val = l2pNode->val;
            }
            
            sum = l1Val + l2Val + carry;
            carry = sum / 10;
            newNode = (ListNode *)malloc(sizeof(ListNode));
            if (newNode == NULL)
            {
                return NULL;
            }
            newNode->val = sum % 10;
            newNode->next = NULL;                   //如果不要这句会runtime error!
            pBefore->next = newNode;
            pBefore = pBefore->next;
//            newNode = NULL;                       //不要也可以
            
            if (l1pNode != NULL)
            {
                l1pNode = l1pNode->next;
            }
            if (l2pNode != NULL)
            {
                l2pNode = l2pNode->next;
            }
        }

        return head.next;
    }

};
