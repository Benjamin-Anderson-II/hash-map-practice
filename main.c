#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashMap.h"

/*
 the getWord function takes a FILE pointer and returns you a string which was
 the next word in the in the file. words are defined (by this function) to be
 characters or numbers separated by periods, spaces, or newlines.
 
 when there are no more words in the input file this function will return NULL.
 
 this function will malloc some memory for the char* it returns. it is your job
 to free this memory when you no longer need it.
 */
char* getWord(FILE *file); /* prototype */

void adjustConcordance(struct hashMap *ht, char *word);

void printFileConcordance(struct hashMap *ht);

/****************************************/

int main (int argc, const char * argv[]) {
    FILE *file;
    char *word = ".";
    hashMap *concordance = (hashMap *)malloc(sizeof(hashMap));

    if(argc != 2) {
    	printf("You must input a file.\n");
    	return -1;
    }

    initMap(concordance, 32);
    file = fopen(argv[1], "r");
    word = getWord(file);
    while(word!=NULL){
    	adjustConcordance(concordance, word);
    	free(word);
    	word = getWord(file);
    }
    free(word);
    printFileConcordance(concordance);
    fclose(file);
    freeMap(concordance);
    free(concordance);
    return 0;
}

void adjustConcordance(struct hashMap *ht, char *word){
	int *wordConcordance = atMap(ht, word);
	insertMap(ht, word, (wordConcordance) ? *wordConcordance+1 : 1);
}

void printFileConcordance(struct hashMap *ht){
	int i;
	hashLink *cur;
	for(i = 0; i < ht->tableSize; i++){
		cur = ht->table[i];
		while(cur!=NULL){
			printf("%s: %d\n", cur->key, cur->value);
			cur = cur->next;
		}
	}
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = (char*)malloc(sizeof(char) * maxLength);
	assert(word != NULL);
    
	while( (character = fgetc(file)) != EOF)
	{
		if((length+1) > maxLength)
		{
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || /*is a number*/
		   (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
		   (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
		   character == 39) /*or is an apostrophy*/
		{
			word[length] = character;
			length++;
		}
		else if(length > 0)
			break;
	}
    
	if(length == 0)
	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}

