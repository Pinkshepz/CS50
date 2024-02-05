// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Word counter
unsigned int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word
    int hash_value = hash(word);

    // Access linked list
    node *index = table[hash_value];

    // Loop for finding word
    for (node *cursor = index; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    if (strcmp(&word[1], "'") == 0 || strcmp(&word[2], "'") == 0)
    {
        return (toupper(word[0]) - 'A') % N;
    }
    if (strlen(word) == 1)
    {
        return (toupper(word[0]) - 'A') % N;
    }
    if (strlen(word) == 2)
    {
        return ((26 * toupper(word[1]) - 'A') + (toupper(word[0]) - 'A')) % N;
    }
    else
    {
        return ((576 * toupper(word[2]) - 'A') + (26 * toupper(word[1]) - 'A') + (toupper(word[0]) - 'A')) % N;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary and check if dictionary can be opened
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Read each string from file
    char buffer[LENGTH + 1];
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        // Create new node and check if node is valid
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        // Copy word to node
        strcpy(new_node->word, buffer);
        new_node->next = NULL;

        // Hash word
        int hash_value = hash(buffer);

        // Insert new node into the hash table's linked list
        if (table[hash_value] == NULL)
        {
            table[hash_value] = new_node;
        }
        else
        {
            new_node->next = table[hash_value];
            table[hash_value] = new_node;
        }

        // Increment word count
        word_count++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return word count
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Unload linked list
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}