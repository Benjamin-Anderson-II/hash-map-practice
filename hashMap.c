#include <stdlib.h>
#include "hashMap.h"
#include "structs.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define MAX_LOAD_FACTOR 0.75
void _resizeMap(struct hashMap *ht);

char *strdup(const char *str)
{
	char *dup = malloc(strlen(str) + 1);
	assert(dup);
	if(dup)
		strcpy(dup, str);
	return dup;
}

int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between 1 and 2 is on this line*/
	return r;
}

void initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	assert(ht->table);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

void freeMap (struct hashMap * ht)
{
	hashLink *cur, *last;
	int i;
	for(i = 0; i < ht->tableSize; i++){
		cur = ht->table[i];
		while(cur != NULL){
			last = cur;
			cur = cur->next;
			free(last->key);
			free(last);
		}
	}
	free(ht->table);
}

void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
	char * k_copy = strdup(k);
	int hash = (HASHING_FUNCTION%2)?stringHash1(k):stringHash2(k);
	int hashIdx = (int) (labs(hash) % ht->tableSize);
	int keyDepth = containsKey(ht, k);
	hashLink* newLink;
	float loadFactor;


	/*When the key is already in the map*/
	if(keyDepth){
		int i;
		hashLink *cur = ht->table[hashIdx];
		for(i = 1; i < keyDepth; i++){
			cur = cur->next;
		}
		cur->value = v;
		return;
	}

	newLink = (hashLink*)malloc(sizeof(hashLink));
	assert(newLink);
	newLink->key = k_copy;
	newLink->value = v;
	newLink->next = ht->table[hashIdx];
	ht->table[hashIdx] = newLink;
	ht->count++;
	loadFactor = tableLoad(ht);
	if(loadFactor > MAX_LOAD_FACTOR)
		_resizeMap(ht);
}

void _resizeMap(struct hashMap *ht){
	int oldSize = ht -> tableSize;
	hashLink** oldTable = ht->table;
	hashLink* cur, * last;
	int i;

	initMap(ht, 2*oldSize);
	for(i = 0; i < oldSize; i++){
		cur = oldTable[i];
		while(cur != NULL){
			insertMap(ht, cur->key, cur->value);
			last = cur;
			cur = cur->next;
			free(last->key);
			free(last);
		}
	}
	free(oldTable);
}

ValueType* atMap (struct hashMap * ht, KeyType k)
{
	int hash = (HASHING_FUNCTION%2)?stringHash1(k):stringHash2(k);
	int hashIdx = (int) (labs(hash) % ht->tableSize);
	hashLink *cur = ht->table[hashIdx];

	while(cur != NULL){
		if(strcmp(cur->key, k)==0) return &cur->value;
		cur = cur->next;
	}

	return NULL;
}

int containsKey (struct hashMap * ht, KeyType k)
{
	int hash = (HASHING_FUNCTION%2)?stringHash1(k):stringHash2(k);
	int hashIdx = (int) (labs(hash) % ht->tableSize);
	hashLink *cur = ht->table[hashIdx];
	int depth = 1;
	
	while(cur != NULL){
		if(strcmp(cur->key, k)==0) return depth;
		cur = cur->next;
		depth++;
	}

	return 0;
}

void removeKey (struct hashMap * ht, KeyType k)
{
	int hash = (HASHING_FUNCTION%2)?stringHash1(k):stringHash2(k);
	int hashIdx = (int) (labs(hash) % ht->tableSize);
	int keyDepth = containsKey(ht, k);

	if(!keyDepth) return;

	if(keyDepth){
		int i;
		hashLink *cur = ht->table[hashIdx];
		hashLink *prev = NULL;
		for(i = 1; i < keyDepth; i++){
			prev = cur;
			cur = cur->next;
		}
		if(prev){
			prev->next = cur->next;
			free(cur);
		} else {
			ht->table[hashIdx] = cur->next;
			free(cur);
		}
	}
}

int sizeMap (struct hashMap *ht)
{
	return ht->count;
}

int capacityMap(struct hashMap *ht)
{
	return ht->tableSize;
}

int emptyBuckets(struct hashMap *ht)
{
	int i;
	int count;
	for(i = 0; i < ht->tableSize; i++){
		if(ht->table[i]==NULL) count++;
	}
	return count;
}

float tableLoad(struct hashMap *ht)
{
	return ht->count / ht->tableSize;
}
