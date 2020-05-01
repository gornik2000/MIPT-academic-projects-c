#include "tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "textProcessor.h"

#define BUFF_SIZE 100

int treeCtor(tree *t) {
	assert(t);

	t->root = NULL;
	t->size = 0;
	t->errNo = 0;

	return t->errNo;
}

void freeNodes(t_node *node) {
	if (node == NULL)
		return;

	freeNodes(node->left);
	freeNodes(node->right);
//	if (node->val->type == FUNCTION_DECLARATION || node->val->type == FUNCTION || node->val->type == VAR)
//		free(node->val->val);
	free(node->val);
	tNodeDtor(node);
	free(node);
}

void treeDtor(tree *t) {
	assert(t);

	freeNodes(t->root);
	t->root = NULL;
	t->size = 0;
	t->errNo = 0;
}

void treeClear(tree *t) {
	assert(t);

	t->root = NULL;
	t->size = 0;
	t->errNo = 0;
}

int treeCountNodes(tree *t, t_node *node, int *counter) {
	if (node == NULL)
		return 0;

	(*counter)++;
	if (*counter > t->size)
		return -1;

	if (treeCountNodes(t, node->left, counter) == -1)
		return -1;

	if (treeCountNodes(t, node->right, counter) == -1)
		return -1;

	return *counter;
}

int checkLinks(t_node *node) {
	if (node == NULL)
		return 1;

	if (node->left != NULL) {
		if (node->left->parent != node || !checkLinks(node->left))
			return 0;
	}

	if (node->right != NULL) {
		if (node->right->parent != node || !checkLinks(node->right))
			return 0;
	}

	return 1;
}

int treeOk(tree *t) {
	if (t == NULL)
		return 0;

	if (t->errNo != TREE_NO_ERROR)
		return 0;

	int nodeCount = 0;
	treeCountNodes(t, t->root, &nodeCount);

	if (nodeCount != t->size) {
		t->errNo = TREE_SIZE_ERROR;
		return 0;
	}

	if (checkLinks(t->root) || (t->root != NULL && t->root->parent != NULL)) {
		t->errNo = TREE_LINK_ERROR;
		return 0;
	}

	return 1;
}

int Fopen(const char *file);
int Fclose();
int Fprintf(char *fmt, ...);

void printGraphNodeVal(t_node *node) {
	if (node == NULL)
		return;

	switch (node->val->type) {
	case OP:
		Fprintf("OP; \\\"%d\\\"", (char) node->val->val);
		break;
	case VAR:
		Fprintf("VAR; \\\"%d\\\"", node->val->val);
		break;
	case CONST:
		Fprintf("CONST; %d", node->val->val);
		break;
	case FUNCTION:
		Fprintf("FUNC; \\\"%d\\\"", node->val->val);
		break;
	case FUNCTION_DECLARATION:
		Fprintf("FUNC_DECL; \\\"%d\\\"", node->val->val);
		break;
	default:
		Fprintf("Unknown");
	}
}

void printGraphNode(t_node *node) {
	if (node == NULL)
		return;

	Fprintf("\tn%p [shape=record, label=\"{", node);
	printGraphNodeVal(node);
	Fprintf("|{<l%p> left|<r%p> right}}\"];\n", node, node);
//	Fprintf("\tn%p [shape=record, label=\"{%s|{<l%p> left|<r%p> right}}\"];\n", node, node->val, node, node);
	if (node->left)
		Fprintf("\tn%p:l%p -> n%p:n;\n", node, node, node->left);
	if (node->right)
		Fprintf("\tn%p:r%p -> n%p:n;\n", node, node, node->right);
	if (node->parent)
		Fprintf("\tn%p:n -> n%p;\n", node, node->parent);
}

void plotTreeGraph(tree *t) {
	assert(t);

	Fopen("tree.dv");
	Fprintf("digraph tree {\n");
	processTree(t, printGraphNode);
	Fprintf("}\n");
	Fclose();

	system("dotty tree.dv");

	remove("tree.dv");
}

void treeDump(tree *t) {
	assert(t);

	printf("tree [%p] {\n", t);
	printf("\terrno = %d\n", t->errNo);
	printf("\troot = [%p]\n", t->root);
	printf("\tsize = %d\n", t->size);
	printf("}\n");

	plotTreeGraph(t);
}

int treeAddLeft(tree *t, t_node *node, t_node *newLeft) {
	assert(t);
	assert(newLeft);

	if (node == NULL) { // inserting as root
		if (t->root != NULL) {
			t->errNo = TREE_LINK_ERROR;
			return t->errNo;
		}

		t->root = newLeft;
		t->size = tNodeCountChildren(t->root);
		t->root->parent = NULL;

		return t->errNo;
	}

	if (node->left != NULL) {
		t->errNo = TREE_LINK_ERROR;
		return t->errNo;
	}

	node->left = newLeft;
	node->left->parent = node;
	t->size += tNodeCountChildren(newLeft);

	return t->errNo;
}

int treeAddRight(tree *t, t_node *node, t_node *newRight) {
	assert(t);
	assert(newRight);

	if (node == NULL) { // inserting as root
		if (t->root != NULL) {
			t->errNo = TREE_LINK_ERROR;
			return t->errNo;
		}

		t->root = newRight;
		t->size = tNodeCountChildren(t->root);

		return t->errNo;
	}

	if (node->right != NULL) {
		t->errNo = TREE_LINK_ERROR;
		return t->errNo;
	}

	node->right = newRight;
	node->right->parent = node;
	t->size += tNodeCountChildren(newRight);

	return t->errNo;
}

int treeRemove(tree *t, t_node *node) {
	assert(t);
	assert(node);

	if (t->root == node) {
		t->root = NULL;
		t->size = 0;

		return t->errNo;
	}

	if (node->parent->left == node)
		node->parent->left = NULL;
	else
		node->parent->right = NULL;

	t->size -= tNodeCountChildren(node);
	node->parent = NULL;

	return t->errNo;
}

t_node *treeRoot(tree *t) {
	assert(t);

	return t->root;
}

int treeSize(tree *t) {
	assert(t);

	return t->size;
}

int treeErrno(tree *t) {
	assert(t);

	return t->errNo;
}

void treeResetErrno(tree *t) {
	assert(t);

	t->errNo = 0;
}

void processTree(tree *t, void (*operation)(t_node *node)) {
	assert(t);

	process(t->root, operation);
}

void process(t_node *node, void (*operation)(t_node *node)) {
	if (node == NULL)
		return;

	(*operation)(node);
	process(node->left, operation);
	process(node->right, operation);
}

void tNodeCtor(t_node *node, tree_data_t val) {
	assert(node);

	node->val = val;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
}

void tNodeDtor(t_node *node) {
	assert(node);

	node->val = 0;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
}

t_node *createNode(char type, int val, t_node *left, t_node *right) {
	assert(type >= 0 && type < MAX_TOKEN_TYPE);

	t_node *node = (t_node *) calloc(1, sizeof(t_node));
	struct token *tkn = (struct token *) calloc(1, sizeof(struct token));

	tkn->type = type;
	tkn->val = val;

	node->val = tkn;
	node->left = left;
	node->right = right;

	return node;
}

tree_data_t tNodeVal(t_node *node) {
	assert(node);

	return node->val;
}

t_node *tNodeLeft(t_node *node) {
	assert(node);

	return node->left;
}

t_node *tNodeRight(t_node *node) {
	assert(node);

	return node->right;
}

int tNodeCountChildren(t_node *node) {
	if (node == NULL)
		return 0;

	return tNodeCountChildren(node->left) + tNodeCountChildren(node->right) + 1;
}

FILE *out;

int Fopen(const char *file) {
	assert(file);

	out = fopen(file, "wb");

	if (out == NULL)
		return EOF;

	return 0;
}

int Fclose() {
	if (out == NULL)
		return EOF;

	fclose(out);
	return 0;
}

int Fprintf(char *fmt, ...) {
	assert(fmt);

	if (out == NULL)
		return 0;

	va_list args;
	va_start(args, fmt);
	int res = vfprintf(out, fmt, args);
	va_end(args);

	return res;
}

char *loadNodeValue(t_node *node, char *line) {
	assert(node);
	assert(line);

	line = skipSpaces(line);

	int nextPos = 0, type = 0;

	sscanf(line, "%d%n", &type, &nextPos);
	if (nextPos == 0)
		return NULL;

	node->val->type = (char) type;
	line += nextPos;
	line = skipSpaces(line);

	sscanf(line, "%d%n", &(node->val->val), &nextPos);
	if (nextPos == 0)
		return NULL;

	line += nextPos;
	line = skipSpaces(line);

	return line;
}

char *loadNode(t_node *node, char *line) {
	assert(node);
	assert(line);

	line = skipSpaces(line);

	line = loadNodeValue(node, line);
	if (line == NULL)
		return NULL;

	line = skipSpaces(line);

	if (node->val->type == FUNCTION_DECLARATION
			|| (node->val->type == OP && (node->val->val == IF || node->val->val == WHILE))) {
		node->left = createNode(0, 0, NULL, NULL);
		line = loadNode(node->left, ++line);
		if (line == NULL)
			return NULL;

		line = skipSpaces(line);
		node->right = createNode(0, 0, NULL, NULL);
		line = loadNode(node->right, ++line);
		if (line == NULL)
			return NULL;
		line = skipSpaces(line);

		while (*line == '(') {
			t_node *op = createNode(0, 0, NULL, NULL);
			line = loadNode(op, ++line);
			if (line == NULL)
				return NULL;

			node->right = createNode(OP, SEPARATOR, node->right, op);
			line = skipSpaces(line);
		}
	} else if ((node->val->type == OP && (node->val->val == OUT || node->val->val == MAIN))
			|| node->val->type == FUNCTION) {
		line = skipSpaces(line);
		node->right = createNode(0, 0, NULL, NULL);
		line = loadNode(node->right, ++line);
		if (line == NULL)
			return NULL;
		line = skipSpaces(line);
		while (*line == '(') {
			t_node *op = createNode(0, 0, NULL, NULL);
			line = loadNode(op, ++line);
			if (line == NULL)
				return NULL;

			node->right = createNode(OP, SEPARATOR, node->right, op);
			line = skipSpaces(line);
		}
	} else if (node->val->type == OP && node->val->val == RETURN) {
		line = skipSpaces(line);
		if (*line == ')')
			return line + 1;

		node->right = createNode(0, 0, NULL, NULL);
		line = loadNode(node->right, ++line);
		if (line == NULL)
			return NULL;
		line = skipSpaces(line);
		while (*line == '(') {
			t_node *op = createNode(0, 0, NULL, NULL);
			line = loadNode(op, ++line);
			if (line == NULL)
				return NULL;

			node->right = createNode(OP, SEPARATOR, node->right, op);
			line = skipSpaces(line);
		}
	} else {

		if (*line == '(') {
			node->left = createNode(0, 0, NULL, NULL);
			line = loadNode(node->left, ++line);
			if (line == NULL)
				return NULL;
			line = skipSpaces(line);
		}

		if (*line == '(') {
			node->right = createNode(0, 0, NULL, NULL);
			line = loadNode(node->right, ++line);
			if (line == NULL)
				return NULL;
			line = skipSpaces(line);
		}
	}

	if (*line != ')') {
		printf("error: %s", line);
		return NULL;
	}

	return line + 1;
}

int treeLoadFromFile(tree *t, const char *file) {
	assert(t);
	assert(file);

	if (t->root != NULL)
		return 0;

	size_t fileSize = sizeofFile(file);
	if (fileSize < 0)
		return 0;
	char *buff = (char *) calloc(fileSize + 1, sizeof(char));
	FILE *in = fopen(file, "rb");
	fread(buff, sizeof(char), fileSize, in);
	fclose(in);

	char *line = skipSpaces(buff);
	if (*line != '(') {
		free(buff);
		return 0;
	}

	t->root = createNode(0, 0, NULL, NULL);
	line = loadNode(t->root, ++line);
	if (line == NULL) {
		freeNodes(t->root);
		t->root = NULL;
		free(buff);
		return 0;
	}

	line = skipSpaces(line);
//	printf("%s", line);
	while (*line == '(') {
//		printf("|");
		t_node *node = createNode(0, 0, NULL, NULL);
		line = loadNode(node, ++line);
		if (line == NULL) {
			freeNodes(t->root);
			t->root = NULL;
			free(buff);
			return 0;
		}
		t->root = createNode(OP, SEPARATOR, t->root, node);
		line = skipSpaces(line);
	}

	free(buff);
	t->size = tNodeCountChildren(t->root);
	return 1;
}

void printNodeVal(t_node *node, int indent) {
	if (node == NULL) {
		return;
	}

	for (int i = 0; i < indent; ++i) {
		Fprintf("\t");
	}
	Fprintf("%d %d\n", node->val->type, node->val->val);
}

void printTextNode(t_node *node, int indent) {
	if (node == NULL) {
		return;
	}

	if (node->val->type == OP && node->val->val == SEPARATOR) {
		printTextNode(node->left, indent);
		printTextNode(node->right, indent);
	} else if (node->val->type == FUNCTION_DECLARATION) {
		for (int i = 0; i < indent; ++i) {
			Fprintf("\t");
		}
		Fprintf("(\n");

		printNodeVal(node, indent + 1);

		t_node *argNum = createNode(CONST, (tNodeCountChildren(node->left) + 2) / 2, NULL, NULL);
		printTextNode(argNum, indent + 1);
		free(argNum);
		printTextNode(node->right, indent + 1);
		for (int i = 0; i < indent; ++i) {
			Fprintf("\t");
		}
		Fprintf(")\n");

	} else {

		for (int i = 0; i < indent; ++i) {
			Fprintf("\t");
		}
		Fprintf("(\n");

		printNodeVal(node, indent + 1);
		printTextNode(node->left, indent + 1);

//		Fprintf("%d ", node->val);
//		Fprintf("%s ", node->val);

		printTextNode(node->right, indent + 1);

		for (int i = 0; i < indent; ++i) {
			Fprintf("\t");
		}
		Fprintf(")\n");
	}
}

int treeSaveToFile(tree *t, const char *file) {
	assert(t);

	if (Fopen(file) != 0)
		return 0;

	printTextNode(t->root, 0);

	Fclose();
	return 1;
}

int findNode(t_node *node, char *path, tree_data_t val) {
	assert(path);

	if (node == NULL)
		return 0;

	if (node->val == val)
//	if (strcasecmp(node->val, val) == 0)
		return 1;

	*path = 'l';
	if (findNode(node->left, path + 1, val) != 0)
		return 1;

	*path = 'r';
	if (findNode(node->right, path + 1, val) != 0)
		return 1;

	*path = '\0';
	return 0;
}

int treeFind(tree *t, char *path, tree_data_t val) {
	assert(t);
	assert(path);

	return findNode(t->root, path, val);
}
