/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* grayCode(int n, int* returnSize) {
    int *res = NULL;
    int len = pow(2, n);
    int i = 0;
    int temp = 0;
    
    res = (int*)malloc(sizeof(int) * len);
    if (res == NULL)
    {
        return res;
    }
    
    for (i = 0; i < len; i++)
    {
        res[i] = i ^ (i >> 1);
    }
    
    *returnSize = len;
    return res;
}