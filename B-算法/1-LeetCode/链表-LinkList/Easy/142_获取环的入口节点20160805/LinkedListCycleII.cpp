/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : LinkedListCycleII.cpp
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
    ListNode *detectCycle(ListNode *head) {
        if (head == NULL)
            return NULL;
        //1. 判断链表中是否有环。
        //2. 如果有， 返回环中的一个节点。
        //3. 根据环中节点来计算环的长度n。
        //4. 定义两个节点指针初始化指向头节点，
        //5. 其中一个先走n步，然后一起走，只到相遇。
        ListNode *MeetingNode = hasCycle(head);
        if (MeetingNode == NULL)
        {
            return NULL;
        }
        
        int CycleLen = getCycleLength(MeetingNode);
        
        return getTargetNode(head, CycleLen);
    }
    
    ListNode * hasCycle(ListNode *head) {
            
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
                return NULL;
            }
            
            if (goOneStep == goTwoStep)
            {
                return goOneStep;
            }
        }
        
        return NULL;
    }
    
    int getCycleLength(ListNode *MeetingNode)
    {
        if (MeetingNode == NULL)
            return 0;
        
        int count = 1;
        ListNode *pNode = MeetingNode->next;
        while(pNode != MeetingNode)
        {
            count++;
            pNode = pNode->next;
        }
        
        return count;
    }
    
    ListNode *getTargetNode(ListNode *head, int CycleLen)
    {
        int i = 0;
        ListNode *pNode = head;
        ListNode *pBefore = head;
        for (i = 0; i < CycleLen; i++)
        {
            pNode = pNode->next;
        }
        
        while(pNode != pBefore)
        {
            pNode = pNode->next;
            pBefore = pBefore->next;
        }
        
        return pNode;
    }
};
