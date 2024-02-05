#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    // check that the program was run with just one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // check that every digit is a number
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) != 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // assign key value
    int key = atoi(argv[1]);

    // get plaintext string input
    string text = get_string("plaintext: ");

    // cipher the text
    printf("ciphertext: ");

    for (int i = 0; i < strlen(text); i++)
    {
        // case 1: non-alphabet character
        if (isalpha(text[i]) == false)
        {
            printf("%c", text[i]);
        }

        // case 2: small alphabet character
        if (islower(text[i]) != 0)
        {
            char c = 96 + (text[i] - 96 + key) % 26;
            printf("%c", c);
        }

        // case 3: large alphabet character
        if (isupper(text[i]) != 0)
        {
            char c = 64 + (text[i] - 64 + key) % 26;
            printf("%c", c);
        }
    }
    printf("\n");
    return 0;
}