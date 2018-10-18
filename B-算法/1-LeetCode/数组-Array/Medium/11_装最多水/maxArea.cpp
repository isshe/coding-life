/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : maxArea.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/23
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
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size()-1;
        int maxA = 0;
        int area = 0;
        int minheight = 0;
        while(left < right)
        {
            minheight = height[left] < height[right] ? height[left] : height[right];
            area = minheight * (right - left);
            if (area > maxA)
            {
                maxA = area;
            }
            if (height[left] > height[right])
            {
                right--;
            }
            else
            {
                left++;
            }
        }
        
        return maxA;
    }
};
