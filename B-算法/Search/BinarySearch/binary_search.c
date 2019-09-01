//#include "quick_sort.h"
#include <stdio.h>

void swap(int *array, int a, int b)
{
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void quick_sort(int *array, int start, int end)
{
    if (start >= end) {
        return ;
    }

    int base = array[start];
    int base_index = start;
    int i = start;
    int j = end;
    while (i < j) {
        if (array[i] > base) {
            swap(array, i, j);
            j--;
        } else {
            i++;
        }
    }

    base_index = i;
    if (base < array[i]) {
        base_index = i - 1;
    }
    swap(array, start, base_index);
    quick_sort(array, start, base_index - 1);
    quick_sort(array, base_index + 1, end);
}

int binary_search(int *array, int start, int end, int target)
{
    int mid;

    while (start <= end) {
        mid = start + (end - start)/2;
        if (target > array[mid]) {
            start = mid + 1;
        } else if (target < array[mid]) {
            end = mid - 1;
        } else {
            return mid;
        }
    }

    return -1;
}

int main(void)
{
    int a[] = {3, 5, 11, -1, 11, 100, 9, 11, 8, 7, 200, 2};

    quick_sort(a, 0, 11);
    for (int i = 0; i < 12; i++) {
        printf("%d, ", a[i]);
    }

    printf("\n");
    printf("bs = %d\n", binary_search(a, 0, 11, 8));
}