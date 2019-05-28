/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RectangleArea.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/15
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
int computeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
    
    int area1 = (C-A) * (D-B);
    int area2 = (G-E) * (H-F);
    int x_right = min(C, G);
    int x_left = max(A, E);
    int y_up = min(D, H);
    int y_down = max(B, F);
    int area1Andarea2 = (x_right - x_left) * (y_up - y_down);
    
    int res = 0;
    if (x_left >= x_right || y_down >= y_up)
    {
        res = area1 + area2;
    }
    else
    {
        res = area1 + area2 - area1Andarea2;
    }
    
    return res;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}
