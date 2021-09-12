/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : swapPairs.cpp
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
    ListNode* reverseList(ListNode* head) {
        ListNode *before = NULL;
        ListNode *mid = NULL;
        ListNode *behind = NULL;
        
        before = head;
        if (before != NULL)
        {
            mid = before->next;
            before->next = NULL;        //不要这个指向了。
            
        }
        
        while(mid != NULL)
        {
            behind = mid->next;
            mid->next = before;
            before = mid;
            mid = behind;
        }
        
        return before;
    }
};
