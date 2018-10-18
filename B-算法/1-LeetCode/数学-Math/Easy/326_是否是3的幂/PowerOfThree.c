/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PowerOfThree.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/09
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
bool isPowerOfThree(int n) {
    if (n <= 0)
        return false;
    int x = (int)(log10(n)/log10(3));
    double y = log10(n)/log10(3);
    
    if (y-x <= 0.0000000001)
    {
        return true;
    }

    return false;
}
