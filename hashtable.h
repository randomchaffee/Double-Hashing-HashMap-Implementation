#ifndef _CLOSED_HASHTABLE_H_
#define _CLOSED_HASHTABLE_H_

typedef char* STRING;
typedef char** STRING_ARRAY_PTR;

typedef struct hash_tag {

  int size;

  int tableSize;

  STRING_ARRAY_PTR list;
  
} HASH_TABLE;

typedef HASH_TABLE* HASH_TABLE_PTR;


HASH_TABLE_PTR createHashTable(int tableSize);


int isEmpty(HASH_TABLE_PTR H);
int isFull(HASH_TABLE_PTR H);

void put(HASH_TABLE_PTR H, STRING key, STRING data);

STRING find(HASH_TABLE_PTR H, STRING key, STRING data);

STRING erase(HASH_TABLE_PTR H, STRING key, STRING data);

void destroy(HASH_TABLE_PTR H);

void printTable(HASH_TABLE_PTR H);

#endif
