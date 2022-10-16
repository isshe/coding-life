//思路：根据lehmer码（不知道啥，百度资料也少）
//用例子说明：n = 3, k = 6;
//第k个, 如果从0开始则第k个的下标为k-1, 例子中就是5， 有：
// 5 / (n-1)! = 2 余数 1
// 1 / (n-2)! = 1 余数 0
// 0 / (n-3)! = 0 余数 0.
// 商2，1，0分别是3个数的下标。210就是所谓的lehmer码，根据这个码找到排列。
// 用一个数组存1~n个数。
//取下标2，[2] = 3, 把3从数组中去除，剩下两个元素。
//取下标1, [1] = 2，把2从数组中去除，剩下一个元素。
//取下标0，[0] = 1, 把1从数组中去除，空，结束。
//这个例子不够好，太特殊了，可以试试 n = 4, k = 7.或其他。

class Solution {
public:
    string getPermutation(int n, int k) {
        string res(n, '0');
        vector<int> aidArray(n, 0);
        int i = 0;
        int index = 0; //k - 1;
        int permu = 1;
        
        for (i = 0; i < n; i++)
        {
            aidArray[i] = i+1;
            permu *= (i+1);
        }
        
//        permu /= n;         //(n-1)!
        k--;
        for (i = 0; i < n; i++)
        {
            permu /= (n-i);
            index = k / permu;
            k = k % permu;
            res[i] = aidArray[index] + '0';
            aidArray.erase(aidArray.begin() + index);
        }
        
        return res;
    }
};