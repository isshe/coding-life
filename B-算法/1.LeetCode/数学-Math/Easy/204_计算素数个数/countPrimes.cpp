/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : countPrimes.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/14
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
int countPrimes(int n) {
    if (n <= 2)
        return 0;
        
    int *array = (int *) malloc(sizeof(int) * (n));
    int i = 0;
    int j = 0;
    for (i = 2; i < n; i++)
    {
        array[i] = 1;
    }
    
    for (i = 2; i * i < n;)
    {
        for (j = i+i; j < n; j += i)
        {
            array[j] = 0;
        }
        i++;
        while(array[i] != 1)
        {
            i++;
        }
    }
    
    int count = 0;
    for (i = 2; i < n; i++)
    {
        if (array[i] == 1)
            count++;
    }
    
    return count;
}
