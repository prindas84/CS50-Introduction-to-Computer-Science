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
bool find_cycle(int winner, int loser, bool answer);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //We must assign each candidate name an integer number to reference the candidate.
    int candidate_number[candidate_count];
    for (int c = 0; c < candidate_count; c++)
    {
        candidate_number[c] = c;
    }

    //We must assign each voters rankings to the preferences array.
    int array_count = candidate_count, pos1 = 0, pos2 = 1, i = 0, j = 0;
    while (array_count > 1)
    {
        //Search through each candidate number to see if it matches the voters first preference.
        //If the candidate matches, then assign the integer "i" as the candidate number that matches the preference, to reference the preference array position "i".
        for (int x = 0; x < candidate_count; x++)
        {
            if (ranks[pos1] == candidate_number[x])
            {
                i = candidate_number[x];
            }
        }

        //Perform this task for every element in the array, except for the first position.
        for (int z = 0; z < (array_count - 1); z++)
        {
            //Search the next position in the array, to see if it matches a candidate number.
            //If this position matches a candidate number, set "j" as that candidate number, to reference the preference array position "j".
            //If this position matches a candidate number, find position [i][j] in the preferences table, and add 1 to that elements count.
            //Once completed, move to the next position in the array and start the process again, comparing it to the first preference.
            for (int y = 0; y < candidate_count; y++)
            {
                if (ranks[pos2] == candidate_number[y])
                {
                    j = candidate_number[y];
                    preferences[i][j]++;
                }
            }
            pos2++;
        }

        //To repeat the process down the array, make the next search field 1 smaller so the process is repeated the correct number of times.
        //Move the first search field one across, to start at the next preference in the array.
        //Move the second search field in line with the corrosponding first search field.
        array_count--;
        pos1++;
        pos2 = (pos1 + 1);
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //Set the variables and the count to 0.
    int pos1 = 0, pos2 = 0;
    pair_count = 0;

    //As long as there are candidates that are left to compare.
    while (pos1 < candidate_count)
    {
        //Compare each individual pair. If there is a winner, update the pairs struct. If there is no winner, move to the next available pair.
        for (int i = 0; i < candidate_count; i++)
        {
            if (preferences[pos1][pos2] <= preferences[pos2][pos1])
            {
                pos2++;
            }
            else
            {
                pairs[pair_count].winner = pos1;
                pairs[pair_count].loser = pos2;
                pair_count++;
                pos2++;
            }
        }
        pos1++;
        pos2 = 0;
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //Calculate the strength of each pair in the pairs array.
    int pair_strength[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        pair_strength[i] = (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
    }

    //Bubble Sort - Order the pairs array by strength. Decreasing strongest to weakest.
    int swap_counter = 1, array_size = pair_count;
    while (swap_counter != 0)
    {
        //Set the positions to 0 and declare a blank pairs array and a blank strength array, to use while switching the positions.
        int pos1 = 0, pos2 = 1, blank_strength;
        pair blank_pair;
        swap_counter = 0;

        //Continue this process for as long as there are positions that are not in order.
        if (pos1 < (array_size - 1))
        {
            for (int i = 0; i < (array_size - 1); i++)
            {
                //If the first position is smaller than the second position, switch them to make the larger number come first.
                if (pair_strength[pos1] < pair_strength[pos2])
                {
                    blank_pair = pairs[pos1];
                    pairs[pos1] = pairs[pos2];
                    pairs[pos2] = blank_pair;
                    blank_strength = pair_strength[pos1];
                    pair_strength[pos1] = pair_strength[pos2];
                    pair_strength[pos2] = blank_strength;
                    pos1++;
                    pos2++;
                    swap_counter++;
                }
                //If the first position is larger than the second position, move to the next position and look again.
                else
                {
                    pos1++;
                    pos2++;
                }
            }
        }
        array_size--;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //Do this for every pair in the pairs array that has a winner.
    int pos1, pos2;
    for (int i = 0; i < pair_count; i++)
    {
        //Set the positions of the pairs winner and loser.
        pos1 = pairs[i].winner;
        pos2 = pairs[i].loser;

        //Declare a function to search the locked array to check for any loops.
        bool cycle = find_cycle(pairs[i].winner, pairs[i].loser, false);

        //If a loop or cycle was found, do not lock the edge. If no loop or cycle was found, lock the edge.
        if (cycle == true)
        {
            locked[pos1][pos2] = false;
        }
        else
        {
            locked[pos1][pos2] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //Check each candidates edges, to see if they can be defined as a winner.
    for (int i = 0; i < candidate_count; i++)
    {
        int true_count = 0, false_count = 0, true_pos2 = 0, false_pos1 = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            //If they have been preferenced over any other candidates, then increase their true count.
            if (locked[i][true_pos2] == 1)
            {
                true_count++;
                true_pos2++;
            }
            else
            {
                true_pos2++;
            }
            //If any of the other candidates have been preferenced over them, increase their false count.
            if (locked[false_pos1][i] != 0)
            {
                false_count++;
                false_pos1++;
            }
            else
            {
                false_pos1++;
            }
        }
        //If they have beaten at least 1 other candidate, their true count will be greated than 0.
        //If they have been beaten by any other candidate, their false count will be greater than 0.
        //If they have a true count greater than 0 and a false count that is 0, they can be declared the winner.
        if (true_count > 0 && false_count == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

bool find_cycle(int winner, int loser, bool answer)
{
    //Repeat this function for the total number of candidates.
    for (int i = 0; i < candidate_count; i++)
    {
        //Move through the locked table looking at the loser array. If and elements in the loser array are true, then proceed.
        //If the element does not point back to the winner of the original pair, then their is no cycle, but recursivly run the function again to check the next path in the cycle.
        //If the element does point back to the winner of the original pair, this means their is a cycle present. Set the answer to 1.
        //Return the answer to the previous cycle, so it is not deleted.
        if (locked[loser][i] == 1)
        {
            if (i != winner)
            {
                answer = find_cycle(winner, i, answer);
            }
            else
            {
                answer = true;
                return answer;
            }
        }
    }
    return answer;
}

