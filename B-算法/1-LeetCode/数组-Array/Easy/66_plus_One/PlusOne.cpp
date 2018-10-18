/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : PlusOne.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/04
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
    vector<int> plusOne(vector<int>& digits) {
        

            
        int len = digits.size();
        vector<int> res;
        int i = len-1;
        int carry = 1;
        int num_i = 0;
        
        if (digits.size() == 0)
            return res;    
        while(i >= 0)
        {
            num_i = digits[i]  + carry;
            if (num_i == 10)
            {
                digits[i] = 0;
                carry = 1;
            }
            else
            {
                digits[i] = num_i;
                carry = 0;
                break;
            }
        
            i--;
        }
        
        if (carry == 1 && i== -1)
        {
            res.push_back(1);
        }
        
        i = 0;
        while(i < len)
        {
            res.push_back(digits[i]);
            i++;
        }
        
        return res;
    }
};
