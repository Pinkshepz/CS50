#include <cs50.h>
#include <stdio.h>

int main(void)
{
  int n;
  do
  {
    n = get_int("Height: ");
  } while (n < 1 || n > 8);

  for (int i = 0; i < n; i++)
  {
    // create front space
    for (int j = n - i - 1; j > 0; j--)
    {
      printf(" ");
    }

    // create # block
    for (int k = 0; k < i + 1; k++)
    {
      printf("#");
    }

    // go to next line
    printf("\n");
  }
}