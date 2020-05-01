#ifndef MAP_H

#define MAP_H

#include "list.h"
#define HASH_SIZE 256

typedef struct map map;

enum mapError {
	MAP_NO_ERROR,
	MAP_ALLOCATION_ERROR,
	MAP_LiST_ERROR,
	MAP_NO_SUCH_ELEMENT
};

struct map {
	list table[HASH_SIZE];
	unsigned int size;
	int errNo;
};

int mapCtor(map *m);
void mapDtor(map *m);

int mapOk(map *m);

void mapDump(map *m);
void printDistribution(map *m);

int mapAdd(map *m, m_key_t key, val_t val);
val_t mapGet(map *m, m_key_t key);
int mapRemove(map *m, m_key_t key);
int mapClear(map *m);

unsigned int mapSize(map *m);
int mapErrno(map *m);
void mapResetErrno(map *m);

unsigned int hash(const char *key);

void mapIterate(map *m, void func(struct l_node *node));

#include "map.c"

#endif
