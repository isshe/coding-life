/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : DeleteLinkedListNode.cpp
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
    void deleteNode(ListNode* node) {
        if (node == NULL)
            return;
        
        //把要删除的节点的下一节点的值赋值给当前节点，然后把下一节点删除
        ListNode *pNode = node->next;
        if (pNode != NULL)
        {
            node->val = pNode->val;
            node->next = pNode->next;
            delete pNode;
        }
        else
        {
            node = NULL;
        }
    }
};
