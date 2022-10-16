class Solution {
public:
    int mySqrt(int x) {
        unsigned long long mid = 0;
        unsigned long long left = 0;
        unsigned long long right = x;
        unsigned long long mul = 0;
        
        while(left < right)
        {
            mid = left + ((right-left) >> 1);
            mul = mid * mid;
            if (mul == x)
            {
                return mid;
            }
            else if (mul > x)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }
        mul = left * left;
        if(mul > x)
        {
            return left-1;
        }
        
        return left;
        
    }
};