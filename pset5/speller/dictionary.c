// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <strings.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
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
const unsigned int N = 100;

// Number of words in dictionary
int words_in_dictionary = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    bool bool_value = false;

    // hash word
    unsigned int hash_word_value = hash(word);

    // access linked list at that inex in the hash table
    node *cursor = table[hash_word_value];

    // while is not the end of the linked list (to check every word)
    while (cursor != NULL)
    {
        // check if the word is in that cursor, if it is we'll return true, otherwise false
    	if (strcasecmp(cursor->word, word) == 0)
        {
            bool_value = true;
            break;
        }
        // move to the next cursor
    	cursor = cursor->next;
    }
    return bool_value;
}

//from: https://stackoverflow.com/questions/40140390/hash-function-issue-adding-functionality
// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381, c;
    while((c = *word++))
      hash += (hash << 5) + (c | 040);

    unsigned int res = (int)hash%N;
    return res; //el cero representa la primera letra
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *file_pointer = fopen(dictionary, "r");

    // check if it exists
    if (file_pointer == NULL)
    {
        printf("File %s could not be open\n", dictionary);
        return false;
    }

    char word[LENGTH + 1]; // is a buffer

    // hashtable has to be empty, so we set it to null
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // read strings from file until it ends
    while(fscanf(file_pointer, "%s", word) != EOF)
    {
        // create a new node to store that word
        node *var_node = malloc(sizeof(node));

        // chequear que tenga la memoria libre
        if (var_node == NULL)
        {
            fclose(file_pointer);
            return false;
        }

        // copy word into node
        strcpy(var_node->word, word);

        // obtain hash value from word
        int index = hash(var_node->word);

        // insert node into hash table at that location
        var_node->next = table[index];
        table[index] = var_node;

        words_in_dictionary++;
    }
    fclose(file_pointer);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words_in_dictionary;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //iterate through hash table and set it free
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        // while is not empty
	    while (cursor != NULL)
	    {
	        //set memory free
		    node *temp = cursor;
	    	cursor = cursor->next;
	    	free(temp);
	    }
	    table[i] = NULL;
    }
	return true;
}