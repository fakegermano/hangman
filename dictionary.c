#include "dictionary.h"

char* get_word(char file_name[], int verbose){
    char *word = NULL;
    int used_words, number;
    time_t t;

    /* Intializes random number generator */
    srand((unsigned) time(&t));

    /* Gets number of already chosen words, if it equals DICTIONARY_SIZE,
       reset dictionary */
    used_words = count_used_words(file_name);
    if(verbose){
        printf("%d Already used words\n", used_words);
    }

    /* If all words have been used, reset the dictionary */
    if(used_words >= DICTIONARY_SIZE){
        if(verbose){
            printf("All words already used, resetting dictionary...\n");
        }
        reset_dictionary(file_name);
        if(verbose){
            printf("Dictionary reset!\n");
        }
    }

    /* While a valid word hasn't been chosen, choose another */
    while(!word) {
        number = rand() % DICTIONARY_SIZE;
        if(verbose){
            printf("Randomly generated number: %d\n", number+1);
            printf("Getting dictionary word...\n");
        }
        word = get_dictionary_word(file_name, number, verbose);
    }

    if(verbose){
        printf("Word chosen: %s\n", word);
    }

    return word;
}

int count_used_words(char file_name[]){
    char *aux;
    FILE *dict;
    int used_words = 0;

    /* Opens dictionary file */
    dict = fopen(file_name, "r");
    aux = malloc(sizeof(char)*MAX_WORD_SIZE);

    while (fscanf(dict, "%s", aux) != EOF){
        if(!strcmp(aux,".")){
            used_words++;
        }
    }

    /* Close dictionary file */
    fclose(dict);
    free(aux);

    return used_words;
}

void reset_dictionary(char file_name[]){
    char *aux;
    FILE *dict, *temp;

    /* Open dictionary file and temporary file that will become new dictionary */
    dict = fopen(file_name, "r");
    temp = fopen("temp.txt", "w");

    aux = malloc(sizeof(char)*MAX_WORD_SIZE);

    while (fscanf(dict, "%s", aux) != EOF){
        if(strcmp(aux,".")){
            fprintf(temp, "%s\n", aux);
        }
    }

    fclose(dict);
    fclose(temp);

    remove(file_name);
    rename("temp.txt", file_name);

    free(aux);

    return;
}

char* get_dictionary_word(char file_name[], int number, int verbose){
    char aux, *word;
    FILE *dict, *temp;
    int line = 0, word_size = 0;

    word = malloc(MAX_WORD_SIZE*sizeof(char));
    /* Open dictionary file and temporary file that will become new dictionary */
    dict = fopen(file_name, "r");
    temp = fopen("temp.txt", "w");

    while (fscanf(dict, "%c", &aux) != EOF){
        if(line == number){
            while(aux != '\n') {
                if(word_size == MAX_WORD_SIZE){
                    if(verbose){
                        printf("Word size exceeded\n");
                    }
                    return 0;
                } else if (aux == '.') {
                    if(verbose){
                        printf("Word already used\n");
                    }
                    return 0;
                }
                word[word_size] = aux;
                word_size++;
                fscanf(dict, "%c", &aux);
            }
        }
        if(aux == '\n'){
            line++;
        }
    }

    rewind(dict);
    line = 0;

    while (fscanf(dict, "%c", &aux) != EOF){
        if(line == number){
            while(aux != '\n') {
                fprintf(temp, "%c", aux);
                fscanf(dict, "%c", &aux);
            }
            fprintf(temp, " .");
        }
        fprintf(temp, "%c", aux);
        if(aux == '\n'){
            line++;
        }
    }

    fclose(dict);
    fclose(temp);

    remove(file_name);
    rename("temp.txt", file_name);

    return word;
}
