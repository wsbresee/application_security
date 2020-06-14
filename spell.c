#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

/* this function borrowed from stackoverflow*/
void remove_punct_and_make_lower_case(char *p)
{
    char *src = p, *dst = p;

    while (*src)
    {
       if (ispunct((unsigned char)*src))
       {
          /* Skip this character */
          src++;
       }
       else if (isupper((unsigned char)*src))
       {
          /* Make it lowercase */
          *dst++ = tolower((unsigned char)*src);
          src++;
       }
       else if (src == dst)
       {
          /* Increment both pointers without copying */
          src++;
          dst++;
       }
       else
       {
          /* Copy character */
          *dst++ = *src++;
       }
    }
    *dst = 0;
}

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
    int num_misspelled = 0;
    char line[MAX_MISSPELLED];
    while (fgets(line, MAX_MISSPELLED, fp)) {
        char *word = strtok(line, " ");
        while (word != NULL) {
            remove_punct_and_make_lower_case(word);
            if (!check_word(word, hashtable)) {
                printf(word);
                misspelled[num_misspelled] = word;
                num_misspelled++;
            }
            word = strtok(NULL, " ");
        }
    }
    return num_misspelled;
}

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

bool check_word(const char* word, hashmap_t hashtable[]) {
    int bucket = hash_function(word);
    hashmap_t cursor = hashtable[bucket];

    const int length = strlen(word);
    char* lower_case = malloc(sizeof(char) * (length + 1));
    lower_case[length] = 0;
    for(int i = 0; i < length; i++) {
        lower_case[i] = tolower(word[i]);
    }

    while (cursor != NULL) {
        if (!strcmp(word, cursor->word)) {
            // free(lower_case);
            return 1;
        }
        cursor = cursor->next;
    }

    bucket = hash_function(lower_case);
    cursor = hashtable[bucket];

    while (cursor != NULL) {
        if (!strcmp(lower_case, cursor->word)) {
            // free(lower_case);
            return 1;
        }
        cursor = cursor->next;
    }

    // free(lower_case);
    return 0;
}
