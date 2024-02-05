#include <stdio.h>
#include <cs50.h>

int recursion(int n);

int main(void)
{
    int number = get_int("Integer: ");
    recursion(number);
    return 0;
}

int recursion(int n)
{
    printf("%i ", n);
    if (n == 1)
        return n;

    if (n % 2 == 0)
        return recursion(n / 2);
    else
        return recursion((3 * n) + 1);
}