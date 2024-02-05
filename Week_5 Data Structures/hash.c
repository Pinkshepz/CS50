#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int number;
    struct node *next;
} node;

void print_table(int i, node *n);

int main(void)
{
    node *table[3];

    node *new_node = malloc(sizeof(node));

    new_node->number = 1;
    new_node->next = table[0];

    table[0]->next = new_node;

    // print
    for (int i = 0; i < 3; i++)
    {
        printf("==========%i\n", i);
        print_table(i, table[i]->next);
    }

    free(new_node);
    return 0;
}

void print_table(int i, node *n)
{
    if (n == NULL)
    {
        return;
    }

    printf("%i\n", n->number);
    print_table(i, n->next);
}