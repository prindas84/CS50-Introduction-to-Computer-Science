// Implements a dictionary's functionality
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 4050;
unsigned int dictionarySize = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Hash the word passed into the function.
    unsigned int hashValue = hash(word);

    //Check the link list at the tables hash value to see if the word is included in the dictionary.
    //If the word is included, return true, else return false.
    for (node *cursor = table[hashValue]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Declare variables required to implement a hash function.
    unsigned int hashNumber = 0, wordLength = strlen(word), wordValue = 0;

    //Iterate over each character in the word and add its ASCII value to the wordValue counter.
    //This will give each word a number based ont he value of all its characters.
    for (int i = 0; i < wordLength; i++)
    {
        wordValue += toupper(word[i]);
    }

    //If the value of the word is greater than the number of buckets available, modulate the number to ensure it is within the correct range.
    if (wordValue > (N - 1))
    {
        hashNumber = wordValue % N;
    }

    //If the value of the word is withing range, return the value.
    else
    {
        hashNumber = wordValue;
    }
    return hashNumber;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Open the dictionary file.
    FILE *dictionaryFile = fopen(dictionary, "r");
    if (dictionaryFile == NULL)
    {
        return false;
    }

    //Read each word in the dictionary file.
    char wordRead[LENGTH + 1];
    
    while (fscanf(dictionaryFile, "%s", wordRead) != EOF)
    {
        //Declare a new node and malloc space for it.
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        
        //Copy the string stored in wordRead into the word field of the new node. Set the pointer to null.
        strcpy(n->word, wordRead);
        n->next = NULL;

        //Hash the word.
        unsigned int hashValue = hash(n->word);

        //Add the node to the front of the list for the corrosponding hash value.
        if (table[hashValue] == NULL)
        {
            table[hashValue] = n;
        }
        else
        {
            n->next = table[hashValue];
            table[hashValue] = n;
        }
        
        //Iterate 1 to the dictionary size count.
        dictionarySize++;
    }
    
    //Close the dictionary file.
    fclose(dictionaryFile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionarySize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Iterate over each element in the hash table.
    for (int j = 0; j < N; j++)
    {
        //Iterate over each node in the linked list which is located at a specific hash value of the table.
        //Set the tmp pointer to the second value in the list, then free the first value in the list, at location "table[j]".
        //Reset the first location to be the second location, and repeat the processs.
        while (table[j] != NULL)
        {
            node *tmp = table[j]->next;
            free(table[j]);
            table[j] = tmp;
        }
    }
    return true;
}
