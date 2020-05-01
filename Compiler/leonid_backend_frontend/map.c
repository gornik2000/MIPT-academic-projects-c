#include "map.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define LIST_START_SIZE 5

/*
int main() {
	map m = {};
	mapCtor(&m);
	mapAdd(&m, "wda", 1);
	printf("%d %d\n", mapGet(&m, "wda"), mapGet(&m, "w"));
	mapDump(&m);
	mapDtor(&m);
}*/

/*!
 * @brief constructor of struct map
 * @param m
 */
int mapCtor(map *m) {
	assert(m);

	for (int i = 0; i < HASH_SIZE; i++) {
		listCtor(&(m->table[i]), LIST_START_SIZE);
	}
	m->size = 0;
	m->errNo = MAP_NO_ERROR;

	return m->errNo;
}

/*!
 * @brief destructor of strust map
 * @param m
 */
void mapDtor(map *m) {
	assert(m);

	for (int i = 0; i < HASH_SIZE; i++) {
		listDtor(&(m->table[i]));
	}
	m->size = 0;
	m->errNo = MAP_NO_ERROR;
}

/*!
 * @brief checks if m is operable
 * @param m
 * @return 1 if map is operable, 0 otherwise
 */
int mapOk(map *m) {
	if (m == NULL)
		return 0;

	for (int i = 0; i < HASH_SIZE; i++) {
		if (!listOk(&(m->table[i]))) {
			m->errNo = MAP_LiST_ERROR;
			return 0;
		}
	}

	if (m->errNo != MAP_NO_ERROR)
		return 0;

	return 1;
}

/*!
 * @brief debug info for map m
 * @param m
 */
void mapDump(map *m) {
	assert(m);

	printf("map [%p] {\n", m);
	printf("errno = %d\n", m->errNo);
	printf("size = %d\n", m->size);
	printf("nodes:\n");
	for (int i = 0; i < HASH_SIZE; i++) {
//		listDump(&(m->table[i]));
		printf("\t%d: %d\n", i, listSize(&(m->table[i])));
	}
	printf("}\n");
}

void printDistribution(map *m) {
	for (int i = 0; i < HASH_SIZE; i++) {
//		listDump(&(m->table[i]));
		printf("%d, %d\n", i, listSize(&(m->table[i])));
	}
}

/*!
 * @brief adds new node with key, val pair
 * @param m
 * @param key
 * @param val
 * @return 0 if error occured, 1 if addition completed
 */
int mapAdd(map *m, m_key_t key, val_t val) {
	assert(m);
	assert(key);

	int keyHash = hash(key) % HASH_SIZE;

	if (listFindVerySlow(&(m->table[keyHash]), key) == 0) {
		struct l_node *newEl = calloc(1, sizeof(struct l_node));
		if (newEl == NULL) {
			m->errNo = MAP_ALLOCATION_ERROR;
			return 0;
		}

		newEl->key = calloc(strlen(key) + 1, sizeof(char));
		if (newEl->key == NULL) {
			m->errNo = MAP_ALLOCATION_ERROR;
			return 0;
		}

		strcpy(newEl->key, key);
//		newEl->key = key;
		newEl->val = val;

		listInsert(&(m->table[keyHash]), listTail(&(m->table[keyHash])), newEl);
		m->size++;
		return 1;
	}

	return 0;
}

/*!
 * @brief gets val by key
 * @param m
 * @param key
 * @return val of node with key or 0 if no such node present
 * @note if there is no such node errno is set to MAP_NO_SUCH_ELEMENT
 */
val_t mapGet(map *m, m_key_t key) {
	assert(m);
	assert(key);

	int keyHash = hash(key) % HASH_SIZE;
	int idx = listFindVerySlow(&(m->table[keyHash]), key);

	if (idx == 0) {
		m->errNo = MAP_NO_SUCH_ELEMENT;
		return 0;
	}

	return listGet(&(m->table[keyHash]), idx)->val;
}

/*!
 * @brief removes node with key
 * @param m
 * @param key
 * @return 0 if no such node present, 1 if deletion successful
 */
int mapRemove(map *m, m_key_t key) {
	assert(m);
	assert(key);

	int keyHash = hash(key) % HASH_SIZE;
	int idx = listFindVerySlow(&(m->table[keyHash]), key);

	if (idx == 0) {
		return 0;
	}

	listRemove(&(m->table[keyHash]), idx);
	m->size--;
	return 1;
}

int mapClear(map *m) {
	assert(m);

	for (int i = 0; i < HASH_SIZE; i++) {
		listClear(&(m->table[i]));
	}

	m->size = 0;
	m->errNo = MAP_NO_ERROR;

	return m->errNo;
}

/*!
 *
 * @param m
 * @return size of m
 */
unsigned int mapSize(map *m) {
	assert(m);

	return m->size;
}

/*!
 *
 * @param m
 * @return errno of m
 */
int mapErrno(map *m) {
	assert(m);

	return m->errNo;
}

/*!
 * @brief resets errno of m to 0
 * @param m
 */
void mapResetErrno(map *m) {
	assert(m);

	m->errNo = 0;
}

unsigned int MyHash(const char *key) {
	assert(key);

	unsigned int res = 0;
	int i = 0;
	while (key[i] != 0) {
		res += key[i];
		i++;
	}

	return res;
}

unsigned int DJBHash(const char *key) {
	assert(key);

	unsigned int h = 5381;
	int i = 0;
	while (key[i] != '\0') {
		h = (h << 5) + h + key[i];
		i++;
	}

	return h;
}

unsigned int XORHash(const char *key) {
	assert(key);

	unsigned int res = 0;
	int i = 0;
	while (key[i] != 0) {
		res ^= key[i];
		i++;
	}

	return res;
}

unsigned int XOR2Hash(const char *key) {
	assert(key);

	unsigned int res = 0;
	int i = 0;
	while (key[i] != 0) {
		res ^= key[i];
		if (i % 2 == 0)
			res <<= 1;
		else
			res >>= 1;
		i++;
	}

	return res;
}

unsigned int XORPowHash(const char *key) {
	assert(key);

	unsigned int res = 0;
	int i = 0;
	while (key[i] != 0) {
		res ^= key[i] << i;
		i++;
	}

	return res;
}

unsigned int WeightedHash(const char *key) {
	assert(key);

	unsigned int res = 0;
	int i = 0;
	while (key[i] != 0) {
		res += key[i] * (i + 1);
		i++;
	}

	return res;
}

unsigned int PowHash(const char *key) {
	assert(key);

	unsigned int res = 0;
	int i = 0;
	while (key[i] != 0) {
		res += key[i] << i;
		i++;
	}

	return res;
}

unsigned int hash(const char *key) {
	assert(key);

	return PowHash(key);
}

void mapIterate(map *m, void func(struct l_node *node)) {
	assert(m);
	assert(func);

	for (int i = 0; i < HASH_SIZE; i++) {
		listIterate(&(m->table[i]), func);
	}
}
