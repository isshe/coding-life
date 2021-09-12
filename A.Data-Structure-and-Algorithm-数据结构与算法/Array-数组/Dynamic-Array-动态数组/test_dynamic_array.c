#include <stdio.h>

#include "dynamic_array.h"

int main(void)
{
    dynamic_array_t *darray = dynamic_array_create(2, sizeof(int));
    int *new_elt = dynamic_array_push(darray); 
    *new_elt = 1;

    new_elt = dynamic_array_push(darray);
    *new_elt = 2;

    new_elt = dynamic_array_push(darray);
    *new_elt = 3;

    new_elt = dynamic_array_push_n(darray, 2);
    *new_elt = 4;
    new_elt++;
    *new_elt = 5;

    int *pop = dynamic_array_pop(darray);
    printf("pop = %d\n", *pop);
    dynamic_array_pop_n(darray, 3);

    int *array_start = (int *)dynamic_array_elts(darray);
    unsigned int cur_size = dynamic_array_nelts(darray);
    for (int i = 0; i < cur_size; i++)
    {
        printf("%d\n", array_start[i]);      // 这里隐含了一个类型转换（void *）转 (int *)
    }

    printf("array size = %d\n", dynamic_array_size(darray));

    dynamic_array_destroy(darray);
    return 0;
}