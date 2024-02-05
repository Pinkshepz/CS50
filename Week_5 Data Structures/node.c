#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int number;
    struct node *next;
} node;

int main(void)
{
    // List of size
    node *list = NULL;

    // Add a number to list
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(n);
        return 1;
    }
    n->number = 1;
    n->next = NULL;

    // Update list to point to new node
    list = n; // store address

    // Add second node
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(n);
        return 1;
    }
    n->number = 2;
    n->next = NULL;
    list->next = n;

    // Add third node
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(list->next);
        free(list);
        return 1;
    }
    n->number = 3;
    n->next = NULL;
    list->next->next = n;

    // Print numbers
    for (node *temp = list; temp != NULL; temp = temp->next)
    {
        printf("%i\n", temp->number);
    }

    // Free list
    while (list != NULL)
    {
        node *temp = list->next;
        free(list);
        list = temp;
    }

    return 0;
}