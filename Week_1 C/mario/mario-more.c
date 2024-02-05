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
    // create left space
    for (int j = n - i - 1; j > 0; j--)
    {
      printf(" ");
    }

    // create left # block
    for (int k = 0; k < i + 1; k++)
    {
      printf("#");
    }

    // create center space
    printf("  ");

    // create right # block
    for (int l = 0; l < i + 1; l++)
    {
      printf("#");
    }

    // go to next line
    printf("\n");
  }
}