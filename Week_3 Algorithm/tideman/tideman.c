#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int tail, int head);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(candidates[k], name) == 0)
        {
            ranks[rank] = k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int k = 0; k < candidate_count - 1; k++)
    {
        for (int l = k + 1; l < candidate_count; l++)
        {
            preferences[ranks[k]][ranks[l]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int k = 1; k < candidate_count; k++)
    {
        for (int l = 0; l < k; l++)
        {
            if (preferences[k][l] > preferences[l][k])
            {
                pairs[pair_count].winner = k;
                pairs[pair_count].loser = l;
                pair_count += 1;
            }
            else if (preferences[k][l] < preferences[l][k])
            {
                pairs[pair_count].winner = l;
                pairs[pair_count].loser = k;
                pair_count += 1;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO Selection sort
    for (int n = 0; n < pair_count; n++)
    {
        // find strongest diff
        int most = 0;   // strongest win gap
        int diff = 0;   // gap of winner and loser scores
        int target = n; // target to swap position

        for (int m = n; m < pair_count; m++)
        {
            diff = preferences[pairs[m].winner][pairs[m].loser] - preferences[pairs[m].loser][pairs[m].winner];

            if (diff > most)
            {
                most = diff;
                target = m;
            }
        }

        // swap the biggest one (index target) to index n
        int temp[] = {pairs[target].winner, pairs[target].loser};
        pairs[target].winner = pairs[n].winner;
        pairs[target].loser = pairs[n].loser;
        pairs[n].winner = temp[0];
        pairs[n].loser = temp[1];
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int k = 0; k < pair_count; k++)
    {
        // try creating graph
        locked[pairs[k].winner][pairs[k].loser] = true;

        // if cycle occurs, turns back to false
        if (!check_cycle(pairs[k].winner, pairs[k].loser))
        {
            locked[pairs[k].winner][pairs[k].loser] = false;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // we assume that the origin of the graph has all false values
    for (int p = 0; p < pair_count; p++)
    {
        int all_false = 0;
        for (int q = 0; q < pair_count; q++)
        {
            all_false += locked[q][p];
        }
        if (all_false == 0)
        {
            // the winner is in the index p
            printf("%s\n", candidates[p]);
            return;
        }
    }
    return;
}

// Check whether cycle occur on the graph or not
bool check_cycle(int tail, int head)
{
    if (tail == head) // find loop!
    {
        return false;
    }

    // try going forward from head to tail point in the graph
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[head][j])
        {
            if (!check_cycle(tail, j))
            {
                return false;
            }
        }
    }
    return true;
}