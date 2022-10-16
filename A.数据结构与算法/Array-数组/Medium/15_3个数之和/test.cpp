/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : test.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/21
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
 using namespace std;
 #include <vector>
 #include <iostream>
 #include <algorithm>
 
    vector<vector<int> > threeSum(vector<int>& nums) {
        vector<vector<int> > res;
        if (nums.size() <= 2)
            return res;
            
        vector<int> aidArr;
        int mid = 0;
        int i = 0;
        int j = nums.size() - 1;
         int savemid = 0;
         int flag = 0;
         int sum = 0;
        
        sort(nums.begin(), nums.end());
  /*      while(!((nums[mid] == 0) || (nums[mid] < 0 && nums[mid+1] > 0)))
        {
            mid++;
        }
*/
        mid = (i+j)/2;
        savemid = mid;
        while(i != mid && j != mid)
        {
 //           cout << "什么鬼" << endl;
 //           cout <<"[i] = " << nums[i] << ", i = " << i << endl;
//            cout << "[j] = " << nums[j] << " ,j = " << j << endl;
            sum = nums[i] + nums[j] + nums[mid];
            if (sum < 0)
            {
                while(sum < 0 && mid < j)
                {
                    mid++;
                    sum = nums[i] + nums[j] + nums[mid];
                }
            }
            else if (sum > 0)
            {
                while(sum > 0 && mid > i)
                {
                    mid--;
                    sum = nums[i] + nums[j] + nums[mid];
                }
            }
            
                if (mid == j || sum != 0 || mid == i)
                {
                    if (mid == j || sum > 0)
                    {
                        i++;
                    }
                    if (mid == i || sum < 0)
                    {
                        j--;
                    }
                    mid = savemid;
                    continue;
                }
                else
                {
                    aidArr.push_back(nums[i]);
                    aidArr.push_back(nums[mid]);
                    aidArr.push_back(nums[j]);
                    cout << "([i],[mid],[j])" << nums[i] << " " << nums[mid] << " " << nums[j] << endl;
                    if (res.size() == 0 || (find(res.begin(), res.end(), aidArr) == res.end()))
                        res.push_back(aidArr);
                    aidArr.clear();
                    mid = savemid;
                    if (flag == 0)
                    {
                        j--;
                        if (j == mid)
                        {
                            flag++;
                            j = nums.size()-1;
                        }
                    }
                    else
                    {
                        i++;
                        if (i == mid)
                        {
                            flag++;
                        }
                    }
                    continue;
                }
        }
        
//        sort(res.begin(), res.end());
        return res;
    }
    
    
int main(void)
{
    vector<int> array;
/*    array.push_back(0);
    array.push_back(0);
    array.push_back(0);    
    array.push_back(0);
    array.push_back(0);
    array.push_back(0);  

*/
    array.push_back(-4);
    array.push_back(-2);
    array.push_back(-2);
    array.push_back(-2);
    array.push_back(0);
    array.push_back(1);   
    array.push_back(2);
    array.push_back(2);
    array.push_back(2);
    array.push_back(3);
    array.push_back(3);
    array.push_back(4);   
    array.push_back(4);
    array.push_back(6);
    array.push_back(6);   
     
    threeSum(array);
    

    for (int i = 0; i < array.size(); i++)
    {
        cout << array[i] << " " ;
    }
    return 0;
}
