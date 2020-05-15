// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include "dictionary.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000000;

// Hash table
node *table[N] = {NULL};

// initalize running count of words in dicitionary
int word_count = 0;

void deleteList(struct node *head);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int key = hash(word);

    node *ptrtmp = table[key];

    while (true)
    {
        if (ptrtmp == NULL)
        {
            return false;
        }

        else if (strcasecmp(ptrtmp->word, word) == 0)
        {
            return true;
        }

        else
        {
            ptrtmp = ptrtmp -> next;
        }
    }
}


// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file to read
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH+1];

    // reads one word at a time until end of file
    while (fscanf(file, "%s", word) == 1)
    {
        int length = strlen(word) + 1;

        // allocate memory and return the pointer to a node
        node* ptr = malloc(sizeof(node));

        if (ptr == NULL)
        {
            return false;
        }

        // copy word into node and set pointer to NULL
        strcpy(ptr -> word , word);
        ptr -> next = NULL;

        // call hash function and return integer key index
        unsigned int key = hash(word);

        // checks for empty list
        if (table[key] == NULL)
        {
            table[key] = ptr;
            word_count++;
        }

        else
        {
            // initalize a temporary pointer to the first item
            node* ptrtmp = table[key];

            while (true)
            {
                // insert at tail and break forever loop
                if (ptrtmp -> next == NULL)
                {
                    ptrtmp -> next = ptr;
                    word_count++;
                    break;
                }
                // update pointer to move along the linked list
                ptrtmp = ptrtmp -> next;
            }
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (word_count == 0)
    {
        return 0;
    }

    else
    {
        return word_count;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        deleteList(head);
    }
    return true;
}

// recursive function to free each linked list
void deleteList(struct node *head)
{
    if (head == NULL)
    {
        return;
    }
    deleteList(head -> next);
    free(head);
}
