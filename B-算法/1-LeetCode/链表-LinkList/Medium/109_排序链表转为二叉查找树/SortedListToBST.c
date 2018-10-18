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
 //这个方法改变了链表并且每次都需要获取左右链表的长度，如果用开始位置和结束位置（不过也麻烦）
 //更好的方法是BST利用中序遍历结果已排序的特性，使用中序递归实现。
struct TreeNode *recursive(struct ListNode *head);
struct ListNode *separate(struct ListNode *head);
 
struct TreeNode* sortedListToBST(struct ListNode* head) {
    if (head == NULL)
        return NULL;
        
    struct ListNode *leftList = head;
    struct ListNode *rightList = separate(head);        //这里耗时间
    struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    struct ListNode *tempNode = NULL;
    
    
    newNode->val = rightList->val;   //右边第一个作为根节点
    newNode->left = NULL;
    newNode->right = NULL;
    if (leftList != rightList)
    {
        newNode->left = sortedListToBST(leftList);
    }
//    tempNode = rightList->next;
//    rightList->next = NULL;
//    rightList = tempNode;
    newNode->right = sortedListToBST(rightList->next);        //注意！rightList->next;
    
    return newNode;
}

struct ListNode *separate(struct ListNode *head)
{   //可以不判断head == NULL
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
        if (mid == 1)                   //注意这里, 把左边链表最后置为NULL.
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