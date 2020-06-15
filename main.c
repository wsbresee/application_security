#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

/*this function borrowed from online and modified*/
int main(int argc, char** argv) {
    hashmap_t hashtable[HASH_SIZE];
    if (argc < 3) {
        fprintf(stderr, "Error: Insufficient arguments!\n");
        fprintf(stderr, "Usage: ./program to_check.txt wordlist.txt\n");
        exit(-1);
    }
    FILE* fp = fopen(argv[1], "r");
    char * dictionary = argv[2];
    if (!load_dictionary(dictionary, hashtable)) {
        fprintf(stderr, "Doubldn't load dictionary %s\n", argv[2]);
        return 1;
    }
    char* misspelled[MAX_MISSPELLED];
    for (int i = 0; i < MAX_MISSPELLED; i++) {
        misspelled[MAX_MISSPELLED] = (char*)malloc(LENGTH*sizeof(char));
    }
    int num_misspelled = check_words(fp, hashtable, misspelled);
    printf("final misspelled words: ");
    for (int i = 0; i < num_misspelled; i++) {
        printf("%s\n", misspelled[i]);
    }
}
