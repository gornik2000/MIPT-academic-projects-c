#ifndef TREE_H

#define TREE_H

typedef struct tree tree;
typedef struct t_node t_node;
typedef struct token *tree_data_t;

enum treeError {
	TREE_NO_ERROR,
	TREE_ALLOCATION_ERROR,
	TREE_LINK_ERROR,
	TREE_SIZE_ERROR
};

enum tokenType {
	OP = 0,
	VAR = 1,
	CONST = 2,
	FUNCTION = 3,
	FUNCTION_DECLARATION = 4,
	MAX_TOKEN_TYPE
};

enum operationType {
	MAIN = 0,
	RETURN = 1,
	ASSIGMENT = 2,
	IF = 3,
	WHILE = 4,
	ADD = 5,
	SUB = 6,
	MUL = 7,
	DIV = 8,
	EQUAL = 9,
	NOT_EQUAL = 10,
	GREATER = 11,
	GREATER_EQUAL = 12,
	LESS = 13,
	LESS_EQUAL = 14,
	IN = 15,
	OUT = 16,
	SEPARATOR = 17,
	IF_SEPARATOR = 18
};

struct tree {
	t_node *root;
	int size;
	int errNo;
};

struct t_node {
	tree_data_t val;
	t_node *parent;
	t_node *left;
	t_node *right;
};

struct token {
	char type;
	int val;
};

int treeCtor(tree *t);
void treeDtor(tree *t);
void treeClear(tree *t);
void freeNodes(t_node *node);

int treeCountNodes(tree *t, t_node *node, int *counter);

int treeOk(tree *t);

void treeDump(tree *t);

int treeAddLeft(tree *t, t_node *node, t_node *newLeft);
int treeAddRight(tree *t, t_node *node, t_node *newRight);
int treeRemove(tree *t, t_node *node);

t_node *treeRoot(tree *t);
int treeSize(tree *t);
int treeErrno(tree *t);
void treeResetErrno(tree *t);

void processTree(tree *t, void (*operation)(t_node *node));
void process(t_node *node, void (*operation)(t_node *node));

void tNodeCtor(t_node *node, tree_data_t val);
void tNodeDtor(t_node *node);
t_node *createNode(char type, int val, t_node *left, t_node *right);
t_node *createNodeStr(char type, char *val, t_node *left, t_node *right);

tree_data_t tNodeVal(t_node *node);
t_node *tNodeLeft(t_node *node);
t_node *tNodeRight(t_node *node);

int tNodeCountChildren(t_node *node);

char *loadNode(t_node *node, char *line);
int treeLoadFromFile(tree *t, const char *file);
int treeSaveToFile(tree *t, const char *file);

int treeFind(tree *t, char *path, tree_data_t val);

#include "tree.c"

#endif
