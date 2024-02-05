#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get string from input
    string name = get_string("What's your name?: ");

    // print hello + name
    printf("hello, %s\n", name);
}