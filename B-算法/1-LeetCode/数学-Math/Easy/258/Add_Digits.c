/*==============================================================================\
* Copyright(C)2015 Chudai.
*
* File name    : Add_Digits.c
* Version      : v1.0.0
* Author       : 初代
* Date         : 2015/09/19
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*==============================================================================*/


int addDigits(int num) {
    int add = 0;
    
        while(num > 9)
        {
            add = num%10;
            num = num/10 + add;
        }

    
    return (num);
    
}


//只是两位数的???
//超过两位呢?
