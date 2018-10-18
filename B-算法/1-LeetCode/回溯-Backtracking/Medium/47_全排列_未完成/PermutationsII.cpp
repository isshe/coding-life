class Solution {
private:
    void helper(vector<vector<int> > &results,
                vector<int> &permutation,
                vector<int> &nums,
                bool used[]) {
        if (nums.size() == permutation.size()) {
            results.push_back(permutation);
            return;
        }

        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) {
                continue;
            }
			//例如[1,1,2],
			//当 1, 1, 2(下标为1,0,2)的时候，下面这个条件会生效从而跳过不必要的情况
			//1，1，2(下标为0,1,2)已经有了，不需要上面1,1,2(下标为1,0,2)的情况了
            if (i > 0 && used[i - 1] == false && nums[i] == nums[i-1]) {
                continue;
            }

            used[i] = true;
            permutation.push_back(nums[i]);
            helper(results, permutation, nums, used);
            permutation.pop_back();
            used[i] = false;
        }
    }

public:
    vector<vector<int> > permuteUnique(vector<int> &nums) {
        vector<vector<int> > results;
        vector<int> permutation;
        bool used[nums.size()];

        for (int i = 0; i < nums.size(); i++) {
            used[i] = false;
        }

        sort(nums.begin(), nums.end());

        helper(results, permutation, nums, used);
        return results;
    }
};