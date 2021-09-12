/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
 
struct ListNode *insNode(struct ListNode *pHead, struct ListNode *insertNode);

struct ListNode* insertionSortList(struct ListNode* head) {
    if (head == NULL || head->next == NULL)
        return head;
        
    struct ListNode *pNode = head;
    struct ListNode *newHead = NULL;
    struct ListNode *pTemp = NULL;
    while(pNode != NULL)
    {
        pTemp = pNode->next;
        pNode->next = NULL;
        newHead = insNode(newHead, pNode);
        pNode =pTemp;
    }
    
    return newHead;
}

struct ListNode *insNode(struct ListNode *pHead, struct ListNode *insertNode)
{
    if (pHead == NULL)
    {
        return insertNode;
    }
    
//    struct ListNode *newHead = pHead;
    struct ListNode *pNode = pHead;
    struct ListNode *pTemp = NULL;
    
//    insertNode->next = NULL;                //�Է�����
    if (insertNode->val < pHead->val)       //����ڵ�Ϊ��С��ʱ��, ����Ҳ����Ϊ <= . 
    {
        insertNode->next = pHead;
        return insertNode;
    }
    
    while(pNode->next != NULL && pNode->next->val < insertNode->val)
    {
        pNode = pNode->next;
    }
    
    pTemp = pNode->next;
    pNode->next = insertNode;
    insertNode->next = pTemp;
    
    return pHead;
}