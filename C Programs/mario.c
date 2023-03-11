#include <cs50.h>
#include <stdio.h>

int main(void)
{

    //Prompt the user to input a height for the pyramid.
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Build the first pyramid
    int space = (height - 1);
    int hash = (height - space);
    for (int i = 0; i < height; i++)
    {
        for (int s = 0; s < space; s++)
        {
            printf(" ");
        }
        for (int h = 0; h < hash; h++)
        {
            printf("#");
        }

        //Create the middle gap
        printf("  ");

        //Build the second pyramid
        for (int h = 0; h < hash; h++)
        {
            printf("#");
        }
        printf("\n");
        space --;
        hash ++;
    }
}