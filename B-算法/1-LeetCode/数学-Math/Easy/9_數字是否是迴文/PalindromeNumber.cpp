/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PalindromeNumber.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/10
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0)
            return false;
        if (x == 0)
            return true;
            
        int allmi = 1;
        int tempx = x;
        int frontPart = 0;
        int numa = 0;
        int numb = 0;
        
        while(tempx >= 10)
        {
            allmi *= 10;
            tempx /= 10;
        }
        tempx = x;
        frontPart = allmi;
        while(frontPart >= 10)
        {
            //取頭部數字
            numa = tempx / frontPart;
            numa = numa % 10;
 //           frontPart /= 10;
            
            //取尾部數字
            numb = tempx % 10;
            
            if (numa != numb)
            {
                return false;
            }
            tempx /= 10;
            frontPart /= 100;       //爲了減少計算，上面的/10，放到這裏一起除.

        }
        return true;
    }
};
