/****************************************************************************
 * dictionary.h
 *
 * Application Security, Assignment 1
 *
 * Adapted from code written by Ben Halperin
 ***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdio.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

#define HASH_SIZE 2000

#define MAX_MISSPELLED 1000

typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

typedef node* hashmap_t;

/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
/**
 * Inputs:
 *  fp:         A file pointer to the document to check for spelling errors.
 *  hashtable:  The hash table used to determine spelling
 *  misspelled: An empty char* array to be populated with misspelled words.
 *              This array will never be greater than 1000 words long.
 *            
 * Returns:
 *  int:        The number of words in the misspelled arary.
 *
 * Modifies:
 *  misspelled: This array will be filled with misspelled words.
 *
 * Example:
 *  int num_misspelled = check_words(text_file, hashtable, misspelled);
 **/
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
    int num_misspelled = 0;
    char line[MAX_MISSPELLED];
    while (fgets(line, MAX_MISSPELLED, fp)) {
        char *word = strtok(line, " ");
        while (word != NULL) {
            remove_punct_and_make_lower_case(word);
            if (!check_word(word, hashtable)) {
                misspelled[num_misspelled] = word;
                num_misspelled++;
            }
            word = strtok(NULL, " ");
        }

        for word in line {
            remove punctuation from beginning and end of word;
            if (!check_word(word)){
                misspelled.append(word);
                increment num_misspelled;
            }
        }
    }
    return num_misspelled;
}

void remove_punct_and_make_lower_case(char *p) {
    char *src = p, *dst = p;
    while (*src) {
        if (ispunct((unsigned char)*src)) {
            src++;
        } else if (isupper((unsigned char)*src)) {
            *dst++ = tolower((unsigned char)*src);
            src++;
        } else if (src == dst) {
            src++;
            dst++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = 0;
}

/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *            
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *  
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[]) {
    int bucket;
    hashmap_t cursor;
    bucket = hash_function(word);
    cursor = hashmap[bucket];

    while (cursor != NULL) {

        if (!strcmp(word, cursor->word)) {
            return 1;
        }
        cursor = cursor->next;
    }

    const int length = strlen(word);
    char* lower_case = (char*)malloc(length + 1);
    lower_case[length] = 0;
    for(int i = 0; i < length; i++) {
        lower_case[i] = tolower(word[i]);
    }
    
    bucket = hash_function(lower_case);
    cursor = hashtable[bucket];

    while (cursor != NULL) {
        if (!strcmp(lower_case, cursor->word)) {
            free(lower_case);
            return 1;
        }
        cursor = cursor->next;
    }

    free(lower_case);
    return 0;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
/**
 * Inputs:
 *  dictionary_file:    Path to the words file.
 *  hashtable:          The hash table to be populated.
 *            
 * Returns:
 *  bool:       Whether or not the hashmap successfully populated.
 *
 * Modifies:
 *  hashtable: This hashmap should be filled with words from the file provided.
 *
 * Example:
 *  bool success = load_dictionary("wordlist.txt", hashtable);
 **/
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    for(int i = 0; i < HASH_SIZE; i++) {
        hashtable[i] = NULL;
    }

    FILE* dict_file = fopen(dictionary_file, "r");
    hashmap_t new_node;

    if (dict_file == NULL) {
        return 0;
    }

    char word[LENGTH];
    while (fgets(word, LENGTH, dict_file) != NULL) {
        word[strcspn(word, " \"\t\n\v\f\r")] = 0;
        new_node = malloc(sizeof(struct node));
        new_node-> = NULL;
        new_node->next = NULL;
        strcpy(new_node->word, word);
        int bucket = hash_function(word);
        if (hashtable[bucket] == NULL) {
            hashtable[bucket] = new_node;
        } else {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
    }
    fclose(dict_file);
    return 1;
}

/**
 * Already implemented in dictionary.c
 **/
int hash_function(const char* word);

#endif // DICTIONARY_H
