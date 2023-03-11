#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet.
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players.
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words.
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner.
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    //Declare a variable for the total score of the word.
    int total = 0;

    //Add the value of each character to the total.
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (word[i] == 'A' || word[i] == 'a')
        {
            total = total + POINTS[0];
        }
        if (word[i] == 'B' || word[i] == 'b')
        {
            total = total + POINTS[1];
        }
        if (word[i] == 'C' || word[i] == 'c')
        {
            total = total + POINTS[2];
        }
        if (word[i] == 'D' || word[i] == 'd')
        {
            total = total + POINTS[3];
        }
        if (word[i] == 'E' || word[i] == 'e')
        {
            total = total + POINTS[4];
        }
        if (word[i] == 'F' || word[i] == 'f')
        {
            total = total + POINTS[5];
        }
        if (word[i] == 'G' || word[i] == 'g')
        {
            total = total + POINTS[6];
        }
        if (word[i] == 'H' || word[i] == 'h')
        {
            total = total + POINTS[7];
        }
        if (word[i] == 'I' || word[i] == 'i')
        {
            total = total + POINTS[8];
        }
        if (word[i] == 'J' || word[i] == 'j')
        {
            total = total + POINTS[9];
        }
        if (word[i] == 'K' || word[i] == 'k')
        {
            total = total + POINTS[10];
        }
        if (word[i] == 'L' || word[i] == 'l')
        {
            total = total + POINTS[11];
        }
        if (word[i] == 'M' || word[i] == 'm')
        {
            total = total + POINTS[12];
        }
        if (word[i] == 'N' || word[i] == 'n')
        {
            total = total + POINTS[13];
        }
        if (word[i] == 'O' || word[i] == 'o')
        {
            total = total + POINTS[14];
        }
        if (word[i] == 'P' || word[i] == 'p')
        {
            total = total + POINTS[15];
        }
        if (word[i] == 'Q' || word[i] == 'q')
        {
            total = total + POINTS[16];
        }
        if (word[i] == 'R' || word[i] == 'r')
        {
            total = total + POINTS[17];
        }
        if (word[i] == 'S' || word[i] == 's')
        {
            total = total + POINTS[18];
        }
        if (word[i] == 'T' || word[i] == 't')
        {
            total = total + POINTS[19];
        }
        if (word[i] == 'U' || word[i] == 'u')
        {
            total = total + POINTS[20];
        }
        if (word[i] == 'V' || word[i] == 'v')
        {
            total = total + POINTS[21];
        }
        if (word[i] == 'W' || word[i] == 'w')
        {
            total = total + POINTS[22];
        }
        if (word[i] == 'X' || word[i] == 'x')
        {
            total = total + POINTS[23];
        }
        if (word[i] == 'Y' || word[i] == 'y')
        {
            total = total + POINTS[24];
        }
        if (word[i] == 'Z' || word[i] == 'z')
        {
            total = total + POINTS[25];
        }
    }

    //Return the total value to the program.
    return total;
}
