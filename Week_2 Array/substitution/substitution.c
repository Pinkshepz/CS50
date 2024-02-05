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
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // check whether key has 26 characters
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check whether key has alphabetic characters
    int unique[26] = {0};
    int check_sum = 0;

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }

        unique[((int)tolower(argv[1][i])) - 97] = 1;
    }

    // count for unique alphabets
    for (int i = 0; i < 26; i++)
    {
        check_sum += unique[i];
    }

    // check if there is 26 unique alphabets
    if (check_sum != 26)
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    // assign key value
    string key = argv[1];

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
            int c = text[i] - 96 - 1;
            printf("%c", tolower(key[c]));
        }

        // case 3: large alphabet character
        if (isupper(text[i]) != 0)
        {
            int c = text[i] - 64 - 1;
            printf("%c", toupper(key[c]));
        }
    }
    printf("\n");
    return 0;
}