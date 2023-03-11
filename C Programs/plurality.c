#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        //Search the candidate list for the name entered.
        if (strcmp(candidates[i].name, name) == 0)
        {
            //If found, add the vote to the candidates vote count.
            candidates[i].votes ++;
            return true;
        }
    }
    //If not found, return false.
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //Bubble Sort (ORDERED SMALLEST TO LARGEST) - Set the swap counter to 1. Set the array size to an editable array that can be changed with each pass.
    int swap_counter = 1, array_size = candidate_count;
    
    //If there have been more than 0 swaps, make another pass of the array.
    while (swap_counter != 0)
    {
        //Set the position of the first and second number to be compared (comp 1 compares to comp 2)
        //Set a blank candidate in order to facilitate the swapping of array positions.
        //Set the swap count back to 0, to begin counting how many swaps have been made in this pass.
        int comp1 = 0, comp2 = 1;
        candidate blank;
        swap_counter = 0;
        
        //If the first compareable position is not equal to the final position in the current array size (search field), then take another step.
        if (comp1 < (array_size - 1))
        {
            for (int i = 0; i < (array_size - 1); i++)
            {
                //If the first compareable number is larger than the second compareable number, swap the numbers. 
                //Position 1 -> Blank. Position 2-> Position 1. Blank -> Position 1.
                //Update the swap counter to signify a change has been made, then move to the next position.
                if (candidates[comp1].votes > candidates[comp2].votes)
                {
                    blank = candidates[comp1];
                    candidates[comp1] = candidates[comp2];
                    candidates[comp2] = blank;
                    comp1++;
                    comp2++;
                    swap_counter++;
                }
                //If the first compareable number is smaller than the second compareable number, move to the next step.
                else
                {
                    comp1++;
                    comp2++;
                }
            }
        }
        //Adjust the array size for the next pass, as to not include the numbers that are already in order at the end of the array.
        array_size--;
    }
    
    //Print the winning candidate.
    printf("%s\n", candidates[(candidate_count - 1)].name);
            
    //Check the rest of the candidates that might have drawn equal with the winner. Print any candidates the have drawn equal with the winner.        
    for (int x = 1; x < candidate_count; x++)
    {
        if (candidates[((candidate_count) - 1) - x].votes == candidates[(candidate_count) - 1].votes)
        {
            printf("%s\n", candidates[((candidate_count) - 1) - x].name);
        }
    }

    return;
}