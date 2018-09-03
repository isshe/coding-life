
#include <stdio.h>
#include <stdlib.h>

#include "linklist.h"


void init_list_head(struct list_head *head)
{
    //head->prev = head;
    head->next = NULL;

}

void list_add(struct list_head *head, struct list_head *node)
{
    node->next = head->next;
    head->next = node;
}

int main(void) 
{
    struct list_head head;
    struct element *node = NULL;
    struct element *temp_node = NULL;

    init_list_head(&head);

    for (int i = 0; i < 10; i++) {
        node = (struct element *)malloc(sizeof(struct element));
        if (!node) {
            printf("分配内存失败");
            return -1;
        }

        node->val = i;
        list_add(&head, &(node->list));
    }

    struct list_head *pos = head.next;
    while(pos) {
        temp_node = container_of(pos, struct element, list);
        if (temp_node){
            printf("val = %d\n", temp_node->val);
        }
        pos = pos->next;
    }

    return 0;
}