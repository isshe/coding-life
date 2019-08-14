#include "single-linklist.h"

#include <stdio.h>
#include <stdlib.h>

int compare(const void *data, const void *key)
{
    int a = *(int *)data;
    int b = *(int *)key;

    if (a == b) {
        return 0;
    } else if (a > 0) {
        return 1;
    } else {
        return -1;
    }
}

int print(const void *data)
{
    printf("%d, ", *(int *)data);
    return 0;
}

int main(void)
{
    struct slist_head head;
    struct slist_node *node;
    int i;
    int *p;

    slist_init(&head);

    for (i = 0; i < 10; i++) {
        node = (struct slist_node*)malloc(sizeof(struct slist_node));
        p = (int *)malloc(sizeof(int));
        if (!node || !p) {
            printf("malloc error!\n");
            exit(1);
        }
        *p = i;
        //printf("*p = %d", *p);
        node->data = p;
        slist_prepend(&head, node);
        //slist_append(&head, node);
    }

    //slist_traversal(&head, print);

    //for (i = 0; i < 10; i++) {
    //    slist_delete(&head, compare, &i);
    //}
    i = 9;
    slist_delete(&head, compare, &i);

    slist_traversal(&head, print);
}