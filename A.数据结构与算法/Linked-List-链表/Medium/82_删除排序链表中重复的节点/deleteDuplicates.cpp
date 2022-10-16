/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
 //˼·��ֻ��һ���ľͷ���Ŀ���������棬���ĳ���ж������ȫ��������
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
            else        //���ĳ���ظ����������д���
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