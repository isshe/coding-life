#include <stdio.h>
#include <stdlib.h>

#include "1_simgle_linklist.h"
// 此写法有头结点（不存数据）


// 这个可以当做插入来使用，只要知道前一个节点
// 如果每次pre_node都等于头结点，那就是前插法
void list_add(struct list_head *pre_node, struct list_head *new_node)
{
    new_node->next = pre_node->next;
    pre_node->next = new_node;
}

void list_del(struct list_head *head_node, struct list_head *del_node)
{
    struct list_head *cur_node = head_node;
    while(cur_node != NULL) {
        if (cur_node->next == del_node) {
            cur_node->next = del_node->next;
        } else {
            cur_node = cur_node->next;
        }
    }
}

void print_something(struct list_head *head)
{
    struct list_head *pre_node = head;
    while(pre_node->next)
    {
        struct something *s = container_of(pre_node->next, struct something, list);
        printf("%d\n", s->val);
        pre_node = pre_node->next;
    }
}

int main(void)
{
    struct list_head head;
    head.next = NULL;
    for (int i = 0; i < 10; i++)
    {
        struct something *s = (struct something *)malloc(sizeof(struct something));
        if (!s) {
            printf("malloc failed!");
            return -1;
        }
        s->val = i;
        list_add(&head, &s->list);
    }

    print_something(&head);
    printf("\n");

    struct list_head *del_node = head.next;
    for (int i = 0; i < 5; i++) 
    {
        list_del(&head, del_node);
        struct list_head *tmp = del_node->next;
        struct something *s = container_of(del_node, struct something, list);
        if (s) {
            free(s);
        }
        del_node = tmp;
    }
    print_something(&head);
    printf("\n");
    return 0;
}