#include <stdio.h>
#include <math.h>

int main(void)
{
    printf("Number: ");
    float n;
    scanf("%f", &n);
    printf("%f\n", n / 1);
    printf("%lf\n", round(n / 1));
    return 0;
}