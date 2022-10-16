/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
 //思路：只有一个的就放在目标链表下面，如果某数有多个，则全部跳过。
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode headNode = ListNode(0);
        ListNode *pNode = NULL;
        ListNode *pA = NULL;
        
        pA = &headNode;
        
        for (pNode = head; pNode != NULL; )//pNode = pNode->next)
        {
            if (pNode->next == NULL ||  pNode->val != pNode->next->val)
            {
                pA->next = pNode;
                pNode = pNode->next;
                pA = pA->next;
                pA->next = NULL;
            }
            else        //如果某数重复，跳过所有此数
            {
                while(pNode->next != NULL && pNode->val == pNode->next->val)
                {
                    pNode = pNode->next;
                }
                pNode = pNode->next;
            }
        }
        
        return headNode.next;
    }
};