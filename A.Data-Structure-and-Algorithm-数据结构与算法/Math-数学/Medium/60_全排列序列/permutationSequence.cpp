//˼·������lehmer�루��֪��ɶ���ٶ�����Ҳ�٣�
//������˵����n = 3, k = 6;
//��k��, �����0��ʼ���k�����±�Ϊk-1, �����о���5�� �У�
// 5 / (n-1)! = 2 ���� 1
// 1 / (n-2)! = 1 ���� 0
// 0 / (n-3)! = 0 ���� 0.
// ��2��1��0�ֱ���3�������±ꡣ210������ν��lehmer�룬����������ҵ����С�
// ��һ�������1~n������
//ȡ�±�2��[2] = 3, ��3��������ȥ����ʣ������Ԫ�ء�
//ȡ�±�1, [1] = 2����2��������ȥ����ʣ��һ��Ԫ�ء�
//ȡ�±�0��[0] = 1, ��1��������ȥ�����գ�������
//������Ӳ����ã�̫�����ˣ��������� n = 4, k = 7.��������

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