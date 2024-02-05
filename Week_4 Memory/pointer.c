#include <stdio.h>
#include <string.h>

int main(void)
{
    // declare variables
    char *x = "hello, world";

    // address
    // char *px = &x;

    // *px = 100;

    // test output
    printf("%p\n", x);
    printf("%c\n", *x);
    printf("%s\n", x);

    for (int i = 0; i < strlen(x) + 1; i++)
    {
        printf("%p: ", &x[i]);
        printf("%i: ", x[i]);
        printf("%c\n", x[i]);
    }
}