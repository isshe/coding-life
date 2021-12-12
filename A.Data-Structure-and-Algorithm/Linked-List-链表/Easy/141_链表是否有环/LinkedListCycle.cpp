/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : LinkedListCycle.cpp
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
    bool hasCycle(ListNode *head) {
        if (head == NULL)
            return false;
            
        ListNode *goOneStep = head;
        ListNode *goTwoStep = head;
        
        while(goTwoStep != NULL)
        {
            goOneStep = goOneStep->next;
            goTwoStep = goTwoStep->next;
            if (goTwoStep != NULL)
            {
                goTwoStep = goTwoStep->next;
            }
            else
            {
                return false;
            }
            
            if (goOneStep == goTwoStep)
            {
                return true;
            }
        }
        
        return false;
    }
};
