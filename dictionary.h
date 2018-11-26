#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DICTIONARY_SIZE 1000
#define MAX_WORD_SIZE 100

char* get_word(char file_name[], int verbose);
int count_used_words(char file_name[]);
void reset_dictionary(char file_name[]);
char* get_dictionary_word(char file_name[], int number, int verbose);
