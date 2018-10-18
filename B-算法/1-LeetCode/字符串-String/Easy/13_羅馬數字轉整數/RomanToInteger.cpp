/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : RomanToInteger.cpp
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
    int romanToInt(string s) {
        if (s.size() == 0)
            return 0;
        //I(1)，V(5)，X(10)，L(50)，C(100)，D(500)，M(1000)
        
        
        int j = 0;
        int beforeNum = 0;
        int num = 0;
        int sum = 0;
        char ch = 0;
        for (; j < s.size(); j++)
        {
            ch = s[j];
            switch(ch)
            {
                case 'I': 
                    num = 1;
                    break;
                case 'V':
                    num = 5;
                    break;
                case 'X':
                    num = 10;
                    break;
                case 'L':
                    num = 50;
                    break;
                case 'C':
                    num = 100;
                    break;
                case 'D':
                    num = 500;
                    break;
                case 'M':
                    num = 1000;
                    break;
                default:
                    break;
            }
            
            sum += num;
            if (num > beforeNum)
            {
                sum -= beforeNum*2;             //注意這些
            }
            beforeNum = num;
        }
        
        return sum;
    }
};
