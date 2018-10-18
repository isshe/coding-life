/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode *Merge(struct ListNode *List1, struct ListNode *List2);
struct ListNode *recursive(struct ListNode *head, int n);

struct ListNode* sortList(struct ListNode* head) {
    if (head == NULL || head->next == NULL)
        return head;
        
    int len = 0;
    int count = 0;
    struct ListNode *pNode = head;
    while(pNode != NULL)
    {
        pNode= pNode->next;
        count++;
    }
    head = recursive(head, count);
    
    return head;
}

struct ListNode *Merge(struct ListNode *List1, struct ListNode *List2)
{
    if (List1 == NULL)
    {
        return List2;
    }
    if (List2 == NULL)
    {
        return List1;
    }
    
    struct ListNode *pHead = NULL;
    struct ListNode *pNode = NULL;
    
    if (List1->val <= List2->val)
    {
        pHead = List1;
        List1 = List1->next;
    }
    else
    {
        pHead = List2;
        List2 = List2->next;
    }
    pNode = pHead;
    
    while(List1 != NULL && List2 != NULL)
    {
        if (List1->val <= List2->val)
        {
            pNode->next = List1;
            List1 = List1->next;
        }
        else
        {
            pNode->next = List2;
            List2 = List2->next;
        }
        pNode = pNode->next;
    }
    if (List1 == NULL)
    {
        pNode->next = List2;
    }
    if (List2 == NULL)
    {
        pNode->next = List1;
    }
    
    return pHead;
}

struct ListNode *recursive(struct ListNode *head, int n)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }
    
    int mid = (n-1) / 2;
    int i = 0;
    struct ListNode *pLeft = head;
    struct ListNode *pRight = NULL;
    struct ListNode *pNode = head;
    
    //分开两部分
    for (i = 0; i < mid; i++)
    {
        pNode = pNode->next;
    }
    pRight = pNode->next;
    pNode->next = NULL;
    
    pLeft = recursive(pLeft, mid+1);
    pRight = recursive(pRight, n-mid-1);
    
    head = Merge(pLeft, pRight);
    
    return head;
}