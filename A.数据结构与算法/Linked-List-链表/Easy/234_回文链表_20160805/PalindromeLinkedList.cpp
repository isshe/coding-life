/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PalindromeLinkedList.cpp
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
    bool isPalindrome(ListNode* head) {
        if (head == NULL)
            return true;
            
        //1. 获取链表长度
        //2. 把链表后半部分翻转过来。
        //3. 从两端开始遍历
        //时间复杂度O(n), 空间复杂度O(1)
        
        int len = getLength(head);
        if (len == 1)
        {
            return true;
        }
        
        int midlen = len/2;
        int i = 0;
        ListNode *pNode = head;
        ListNode *pRear = NULL;
        
        for (i = 0; i < midlen; i++)
        {
            pNode = pNode->next;
        }
        
        pRear = reversal(pNode);
        pNode = head;
        for (i = 0; i < midlen; i++)
        {
            if (pNode->val != pRear->val)
            {
                return false;
            }
            pNode = pNode->next;
            pRear = pRear->next;
        }
        
        return true;
    }
    
    int getLength(ListNode *head)
    {
        int count = 0;
        while(head != NULL)
        {
            count++;
            head = head->next;
        }
        
        return count;
    }
    
    ListNode * reversal(ListNode *head)
    {
        
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
