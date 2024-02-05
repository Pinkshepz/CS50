#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    // get text
    string text = get_string("Text: ");

    // create variable to store text parameters
    int letters = 0;
    float words = 1.0;
    int sentences = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]) != 0)
        {
            letters += 1;
        }

        if (isspace(text[i]) != 0)
        {
            words += 1;
        }

        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences += 1;
        }
    }

    // calculate Coleman-Liau index
    float L = 100 * (letters / words);
    float S = 100 * (sentences / words);

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // print out grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }

    if (index > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }

    printf("Grade %i\n", index);
}