#include<stdio.h>

void func2(int i)
{
    // return 0;
}

int main()
{
    int i;
    for (i = 0; i < 100000000; i++) {
        func2(2);
    }
}
