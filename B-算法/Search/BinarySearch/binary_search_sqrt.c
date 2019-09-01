
#include <stdio.h>
#include <math.h>

double bs_sqrt(double x)
{
    double start = 0;
    double end = x;
    double mid = 0;
    double temp = 0;

    while (fabs(end - start) > 1e-6) {
        mid = start + (end - start) / 2;
        temp = mid * mid;
        if (fabs(temp - x) < 1e-6) {
            return mid;
        } else if (temp - x > 0) {
            end = mid;
        } else {
            start = mid;
        }
    }

    return mid;
}

int main(void)
{
    printf("%f\n", bs_sqrt(2));
    printf("%f\n", bs_sqrt(4));
}