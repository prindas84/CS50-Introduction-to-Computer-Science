#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Ash the user for their name and then say hello.
    string answer = get_string("What is your name?\n");
    printf("hello, %s\n", answer);
}