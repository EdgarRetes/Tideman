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
}
pair;

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

bool check_cycle(int w, int l);

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
    int n = 0;
    for (int i = 0; i < candidate_count; i++) //candidate_count = number odf candidates
    {
        if (strcmp(candidates[i], name) == 0) //searches the name in candidate list 1 by 1
        {
            ranks[rank] = i; //adds the value of i to ranks into position of ranks
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
            //preference gets the value of ranks position i for row
            //preference gets the value of ranks position j for collumn
            //adds the value to prefrence array
            //positions unchanged remain with default value 0
            //when ranks run again the value will be added to the ast value counting each voter
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO


    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j ++)
        {
            //looks for the posotion in the array and compares the opposing values to determine who wins
            if (preferences[i][j] > preferences [j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO

    int over[candidate_count];
    int change;
    int holderw;
    int holderl;

    for (int i = 0; i < candidate_count; i++)
    {
        over[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        //gets number of votes winner had over the loser and takes away the value of votes the loser had with the winner
    }

    for (int j = 0; j < pair_count; j++)
    {
        for (int n = 0; n < pair_count - 1; n++)
        {
            if (over[n] < over[n + 1]) //buble sort
            {
                change = over[n]; //holds the first value of position
                over[n] = over[n + 1];//changes value with next posotion
                over[n + 1] = change;//gives the next position the first value

                //same procces to hange values in winner an loser
                //over is an auxiliary array

                holderw = pairs[n].winner;
                pairs[n].winner = pairs[n + 1].winner;
                pairs[n + 1].winner = holderw;

                holderl = pairs[n].loser;
                pairs[n].loser = pairs[n + 1].loser;
                pairs[n + 1].loser = holderl;
            }
        }

    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO

    int row = 0;
    int column = 0;
    int cycle = 0;

    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if (check_cycle(winner, loser) == false)
        {
            locked[winner][loser] = true;
        }

    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO

    int i;

    for (i = 0; i < candidate_count; i++)
    {
        int tracker = 0;

        for (int j = 0; j < candidate_count; j++)
        {

            if (locked[j][i] == false) //the winner candidate has all his column at false. Search through all columns
            {
                tracker++; //tracker gets the amount of falses in a column
            }
        }
        if (tracker == candidate_count) //when there are as many falses as candidates it breaks
        {
            break;
        }

    }
    printf("%s\n", candidates[i]);
    return;
}

bool check_cycle(int first, int last)
{
    //first = winner
    //last = loser
    if (last == first) //i or last = first so it skips step
    {
        return true;
    }

    //continues first time

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[last][i]) //if the position has a 1 it may be a cycle so it continues
        {
            if (check_cycle(first, i)) //value of first = first and value of i = last - if last = first it is a cycle
            {
                return true;
            }
        }
    }
    return false;

}