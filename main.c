#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EMPTY_INDEX "empty"

void printTable(HASH_TABLE_PTR H){
	if(isEmpty(H)) {
		printf("*empty\n");
		return;
	}

	for(int index=0; index<H->tableSize; index++){
		printf("Cell#%d: ", index);
		if(H->list[index]){
			printf("%s\n", H->list[index]);
		}else{
			printf("*empty*\n");
		}
	}
}

int primaryHash(STRING key, int tableSize) // primary hash
{
	int sum = 0;
	for (int i = 0; key[i] != '\0'; i++)
	{

		sum += key[i]; // get ascii value of the chars in the string
	}
	int h1 = sum % tableSize;

	return h1;
}

int secondaryHash(STRING key, int tableSize)
{
	// 37 - computeKey(key) % 26
	int sum = 0;
	for (int i = 0; key[i] != '\0'; i++)
	{
		if (key[i] % 2 != 0) // this means value of char is odd
		{
			sum += key[i];
		}
	}

	int h2 = 37 - (sum % 26);
	return h2;
}

HASH_TABLE_PTR createHashTable(int tableSize)
{
	// allocate memory for new hash table
	HASH_TABLE_PTR newHashtable = malloc(sizeof(HASH_TABLE));
	// allocate memory for array of strings
	newHashtable->list = malloc(tableSize * sizeof(char *));
	for (int i = 0; i < tableSize; i++)
	{
		newHashtable->list[i] = NULL;
	}
	newHashtable->size = 0; // set initial size
	newHashtable->tableSize = tableSize; // set table size

	return newHashtable;
}

int isEmpty(HASH_TABLE_PTR H)
{
	if (H->size == 0)
	{
		return 1;
	}
	
	return 0;
}

int isFull(HASH_TABLE_PTR H)
{
	if (H->size == H->tableSize)
	{
		return 1;
	}

	return 0;
}

void put(HASH_TABLE_PTR H, STRING key, STRING data)
{
	if (isFull(H))
	{
		printf("table is full!\n");
		return;
	}
	
	int h1 = primaryHash(key, H->tableSize);
	int h2 = secondaryHash(key, H->tableSize);
	int index = h1;

	

	int i = 0;
	// formula: k =[h(k)+p(i)] % SIZE ----- p(i) = i *h2(k)
	while (H->list[index] != NULL && H->list[index] != EMPTY_INDEX) // code block for collision
	{
		i++;
		index = (h1 + (i * h2)) % H->tableSize;
	}

	H->list[index] = strdup(data);
	H->size++;
}

STRING find(HASH_TABLE_PTR H, STRING key, STRING data)
{
	int h1 = primaryHash(key, H->tableSize);
	int h2 = secondaryHash(key, H->tableSize);
	int index = h1;

	int i = 0;
	while (H->list[index] != NULL)
	{
		if (strcmp(H->list[index], data) == 0)
		{
			return H->list[index]; // return the string
		}
		i++;
		index = (h1 + (i * h2)) % H->tableSize;
	}

	return NULL; // not found
}

STRING erase(HASH_TABLE_PTR H, STRING key, STRING data)
{
	int h1 = primaryHash(key, H->tableSize);
	int h2 = secondaryHash(key, H->tableSize);
	int index = h1;

	int i = 0;
	while (H->list[index] != NULL)
	{
		// i used "empty" instead of -1 since we are dealing with string values
		if (H->list[index] != NULL)
		{
			// value to be erased is the current index
			if (strcmp(H->list[index], data) == 0)
			{
				STRING temp = H->list[index];
				H->list[index] = NULL;
				H->size--;
				return temp; // return the copied (erased) string
			}
		}
		i++;
		index = (h1 + (i *h2)) % H->tableSize;
	}

	return NULL; // not found
}

void destroy(HASH_TABLE_PTR H)
{
	if (H == NULL)
	{
		return;
	}

	for (int i = 0; i < H->tableSize; i++)
	{
		// free indexes
		if (H->list[i] != NULL && H->list[i] != EMPTY_INDEX)
		{
			free(H->list[i]);
		}
	}

	// free the array
	free(H->list);

	// free hash table
	free(H);
}

int main(){

	char command;
	STRING key;
	STRING data;
	STRING result;

	int tsize;
	HASH_TABLE_PTR H;
	
	// get table size then maxsize
	scanf("%d\n", &tsize); 
	H = createHashTable(tsize);

	// assume max key size is 20
	key = (STRING)malloc(sizeof(char)*20);
	key[0] = '\0';
	// assume max data size is 100
	data = (STRING)malloc(sizeof(char)*100);
	data[0] = '\0';

	while(1){
		scanf(" %c", &command);

		switch(command){
			case '+':
				scanf(" k:%s d:%s", key, data);
				printf("Inserting data %s with key %s\n", data, key);
				put(H, key, data);
				break;
			case '-':
				scanf(" %s", key);
				printf("Deleting data with key %s\n", key);
				result = erase(H, key, data); 
				// result is unused. print if u want
				break;
			case '?':
				scanf(" k:%s d:%s", key, data);
				printf("Searching data with key: %s. Location: %p\n", key, find(H, key, data));
				// (nil) means NULL pointer
				break;
			case 'p':
				printf("Hash Table: \n");
				printTable(H);
				printf("\n");
				break;
			case 'E':
				printf("Hash table %s empty.\n", isEmpty(H)?"is":"is not");
				break;
			case 'F':
				printf("Hash table %s full.\n", isFull(H)?"is":"is not");
				break;
			case 'C':
				printf("Deleting all contents.\n");
				destroy(H);
				H = createHashTable(tsize);
				break;
			case 'Q':
				free(key); free(data);
				destroy(H); // ensure deletion
				H = NULL;
				return 0;
			default:
				printf("Unknown command: %c\n", command);
		}
	}
	
	return 0;
}
