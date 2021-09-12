/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : ReverseBetween.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/21
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
 *     struct ListNode *next;
 * };
 */
void reversePart(struct ListNode *startNode);

struct ListNode* reverseBetween(struct ListNode* head, int m, int n) {
    if (head == NULL || head->next == NULL || m >= n)
        return head;
        
    int i = 0;
    struct ListNode *pNode = head;
    struct ListNode *mNode = NULL;
    struct ListNode *nNode = NULL;
    for (i = 1; pNode != NULL; i++, pNode = pNode->next)
    {
        if ((m == 1 && i == 1) || (i == m-1))
        {
            mNode = pNode;
        }
        if (i == n)
        {
            nNode = pNode;
        }
    }
    
    //保存第n个节点的next,然后把n->next = NULL, 为了后面翻转。
    pNode = nNode->next;
    nNode->next = NULL;
    
    if (m == 1)
    {
        reversePart(mNode);
        mNode->next = pNode;
    }
    else
    {
        reversePart(mNode->next);
        mNode->next->next = pNode;
        mNode->next = nNode;
    }
    
    if (m == 1)
    {
        head = nNode;
    }
    return head;
}

void reversePart(struct ListNode *startNode)
{
    if (startNode == NULL)
        return;
    struct ListNode *Anode = startNode;
    struct ListNode *Bnode = startNode->next;
    struct ListNode *Cnode = NULL;
    
    while(Bnode != NULL)
    {
        Cnode = Bnode->next;
        Bnode->next = Anode;
        Anode = Bnode;
        Bnode = Cnode;
    }
    
    //return Anode;         
}
