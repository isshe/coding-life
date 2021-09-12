/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
 //��������ı���������ÿ�ζ���Ҫ��ȡ��������ĳ��ȣ�����ÿ�ʼλ�úͽ���λ�ã�����Ҳ�鷳��
 //���õķ�����BST������������������������ԣ�ʹ������ݹ�ʵ�֡�
struct TreeNode *recursive(struct ListNode *head);
struct ListNode *separate(struct ListNode *head);
 
struct TreeNode* sortedListToBST(struct ListNode* head) {
    if (head == NULL)
        return NULL;
        
    struct ListNode *leftList = head;
    struct ListNode *rightList = separate(head);        //�����ʱ��
    struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    struct ListNode *tempNode = NULL;
    
    
    newNode->val = rightList->val;   //�ұߵ�һ����Ϊ���ڵ�
    newNode->left = NULL;
    newNode->right = NULL;
    if (leftList != rightList)
    {
        newNode->left = sortedListToBST(leftList);
    }
//    tempNode = rightList->next;
//    rightList->next = NULL;
//    rightList = tempNode;
    newNode->right = sortedListToBST(rightList->next);        //ע�⣡rightList->next;
    
    return newNode;
}

struct ListNode *separate(struct ListNode *head)
{   //���Բ��ж�head == NULL
    int count = 0;
    struct ListNode *pNode = head;
    struct ListNode *tempNode = NULL;
    int mid = 0;
    
    while(pNode != NULL)
    {
        pNode = pNode->next;
        count++;
    }
    
    mid = (count-1) / 2;
    for (; mid > 0; mid--)
    {
        if (mid == 1)                   //ע������, ��������������ΪNULL.
        {
            tempNode = head;            
        }
        head = head->next;
    }
    
    if (tempNode != NULL)
    {
        tempNode->next = NULL;
    }
    return head;
}