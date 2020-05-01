#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "textProcessor.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "map.h"
#include "stack.h"

#define sassert(expr) \
if (!(expr)) { \
    printf("unexpected symbol \'%c\' on line %d(%s)\n", *S, CurLine + 1, __func__); \
    Error = 1; \
}

map Functions, FunctionsNums, Vars;
stack VarStack;

t_node *getProg(const char *file);

int main(int argc, char *argv[]) {
  if (argc < 2) {
		printf(" # Missing file\n");
		return 0;
	}

  char *fileInName  = argv[1];
  char *fileOutName = argv[2];

	mapCtor(&Functions);
	mapCtor(&FunctionsNums);
	mapCtor(&Vars);
	stackCtor(&VarStack);

	tree t = {};
	treeCtor(&t);
	t.root = getProg(fileInName);
//	treeDump(&t);

	treeSaveToFile(&t, fileOutName);

	treeDtor(&t);
	mapDtor(&Functions);
	mapDtor(&FunctionsNums);
	mapDtor(&Vars);
	stackCtor(&VarStack);
}

char **Lines = NULL;
char *S = NULL;
int CurLine = 0, LinesCount = 0;
int Error = 0;

void incrementPointer() {
	S++;

	while (isspace(*S) || (*S == '\0' && CurLine < LinesCount - 1)) {
		S = skipSpaces(S);

		if (*S == '\0' && CurLine != LinesCount - 1) {
			CurLine++;
			S = Lines[CurLine];
		}
	}
}

t_node *getG();
t_node *getOPBlock();
t_node *getOP();
t_node *getIf();
t_node *getWhile();
t_node *getAssignment();
t_node *getExpression();
t_node *getComparison();
t_node *getE();
t_node *getT();
t_node *getP();
t_node *getN();
t_node *getId();
t_node *getFunctionCall();
t_node *getArgList();
size_t getWordLength();
int isNextWord(const char *word);
t_node *getFunctionDeclaration();
t_node *getArgListDeclaration();
t_node *getReturn();
int getArgCount(t_node *node);

t_node *getProg(const char *file) {
	assert(file);

	Lines = readTextFromFile(file);
	LinesCount = countLines(Lines);

	S = Lines[0] - 1; // done to skip blank lines and space characters
	incrementPointer();

	t_node *root = getG();

	free(Lines[0]);
	free(Lines);

	if (Error) {
		freeNodes(root);
		return NULL;
	}

	return root;
}

t_node *getG() {
	t_node *val = NULL;
	while (*S == 'f') {
		t_node *funcNode = getFunctionDeclaration();
		val = createNode(OP, SEPARATOR, val, funcNode);
	}
	val = createNode(OP, SEPARATOR, val, createNode(OP, MAIN, NULL, getOPBlock()));
	sassert(*S == '\0');
	return val;
}

t_node *getFunctionDeclaration() {
	if (isNextWord("function")) {
		S += strlen("function") - 1;
		incrementPointer();

		size_t len = getWordLength();
		char savedChar = S[len];
		S[len] = '\0';

		char *funcName = calloc(len + 1, sizeof(char));
		strcpy(funcName, S);

		S[len] = savedChar;
		S += len - 1;
		incrementPointer();

		mapResetErrno(&Functions);
		mapGet(&Functions, funcName);

		if (mapErrno(&Functions) != MAP_NO_SUCH_ELEMENT) {
			printf("redeclaration of function %s on line %d\n", funcName, CurLine);
			sassert(*S == '(');
			incrementPointer();

			sassert(*S == ')');
			incrementPointer();

			freeNodes(getOPBlock());
			free(funcName);
			return NULL;
		}

		sassert(*S == '(');
		incrementPointer();

		t_node *argList = NULL;
		if (*S != ')') {
			argList = getArgListDeclaration();
		}

		sassert(*S == ')');
		incrementPointer();

		mapAdd(&Functions, funcName, getArgCount(argList));
		int num = mapSize(&FunctionsNums);
		mapAdd(&FunctionsNums, funcName, num);

		t_node *res = createNode(FUNCTION_DECLARATION, num, argList,
				createNode(OP, SEPARATOR, getOPBlock(), createNode(OP, RETURN, NULL, NULL)));

		mapClear(&Vars);

		return res;
	}

	return NULL;
}

t_node *getArgListDeclaration() {
	size_t len = getWordLength();
	char savedChar = S[len];
	S[len] = '\0';
	mapAdd(&Vars, S, mapSize(&Vars));
	char *varName = calloc(len + 1, sizeof(char));
	strcpy(varName, S);
	stackPush(&VarStack, varName);

	S[len] = savedChar;

	t_node *args = getId();

	while (*S == ',') {
		incrementPointer();

		len = getWordLength();
		savedChar = S[len];
		S[len] = '\0';
		mapAdd(&Vars, S, mapSize(&Vars));
		varName = calloc(len + 1, sizeof(char));
		strcpy(varName, S);
		stackPush(&VarStack, varName);
		S[len] = savedChar;

		args = createNode(OP, SEPARATOR, args, getId());
	}

	return args;
}

int getArgCount(t_node *node) {
	if (node == NULL)
		return 0;

	if (node->val->type == OP && node->val->val == SEPARATOR)
		return getArgCount(node->left) + getArgCount(node->right);

	return 1;
}

t_node *getOPBlock() {
	int varNum = 0;

	if (*S == '{') {
		incrementPointer();

		char *oldS = S;
		t_node *val = getOP();
		while (oldS != S && *S != '}') {
			oldS = S;

			t_node *opNode = getOP();
			if (opNode->val->type == OP && opNode->val->val == ASSIGMENT) {
				varNum++;
			}
			if (oldS != S)
				val = createNode(OP, SEPARATOR, val, opNode);
		}
		sassert(*S == '}');
		incrementPointer();
		return val;
	}

	while (varNum--) {
		char *varName = stackPop(&VarStack);
		mapRemove(&Vars, varName);
		free(varName);
	}
	return getOP();
}

t_node *getOP() {
	if (isNextWord("if"))
		return getIf();

	if (isNextWord("while"))
		return getWhile();

	if (isNextWord("return"))
		return getReturn();

	if (S[getWordLength()] == '(') { //TODO right check for function call
		t_node *func = getFunctionCall();
		sassert(*S == ';');
		incrementPointer();
		return func;
	}

	return getAssignment();
}

t_node *getReturn() {
	sassert(isNextWord("return"));
	S += 5;
	incrementPointer();

	if (*S == ';') {
		incrementPointer();
		return createNode(OP, RETURN, NULL, NULL);
	}

	t_node *val = createNode(OP, RETURN, NULL, getE());
	sassert(*S == ';');
	incrementPointer();
	return val;
}

t_node *getIf() {
	sassert(isNextWord("if"));
	S++;
	incrementPointer();

	sassert(*S == '(');
	incrementPointer();

	t_node *condition = getExpression();

	sassert(*S == ')');
	incrementPointer();

	t_node *operation = getOPBlock();

	if (isNextWord("else")) {
		S += 3;
		incrementPointer();
		operation = createNode(OP, IF_SEPARATOR, operation, getOPBlock());
	}

	return createNode(OP, IF, condition, operation);
}

t_node *getWhile() {
	sassert(isNextWord("while"));
	S += 4;
	incrementPointer();

	sassert(*S == '(');
	incrementPointer();

	t_node *condition = getExpression();

	sassert(*S == ')');
	incrementPointer();

	t_node *operation = getOPBlock();

	return createNode(OP, WHILE, condition, operation);
}

t_node *getAssignment() {
	char *oldS = S;

	size_t len = getWordLength();
	char savedChar = S[len];
	S[len] = '\0';
	if (mapAdd(&Vars, S, mapSize(&Vars))) {
		char *varName = calloc(len + 1, sizeof(char));
		strcpy(varName, S);
		stackPush(&VarStack, varName);
	}
	S[len] = savedChar;

	t_node *id = getId();
	if (oldS == S)
		return NULL;

	sassert(*S == '=');
	incrementPointer();

	t_node *expr = getE();

	sassert(*S == ';');
	incrementPointer();

	return createNode(OP, ASSIGMENT, id, expr);
}

t_node *getExpression() {
	t_node *left = getE();

	t_node *comp = getComparison();
	if (comp == NULL)
		return left;

	t_node *right = getE();

	comp->left = left;
	comp->right = right;

	return comp;
}

t_node *getComparison() {
	switch (*S) {
	case '>':
		if (*(S + 1) == '=') {
			S++;
			incrementPointer();
			return createNode(OP, GREATER_EQUAL, NULL, NULL);
		}

		incrementPointer();
		return createNode(OP, GREATER, NULL, NULL);
	case '<':
		if (*(S + 1) == '=') {
			S++;
			incrementPointer();
			return createNode(OP, LESS_EQUAL, NULL, NULL);
		}

		incrementPointer();
		return createNode(OP, LESS, NULL, NULL);
	case '=':
		if (*(S + 1) == '=') {
			S++;
			incrementPointer();
			return createNode(OP, EQUAL, NULL, NULL);
		}
		return NULL;
	case '!':
		if (*(S + 1) == '=') {
			S++;
			incrementPointer();
			return createNode(OP, NOT_EQUAL, NULL, NULL);
		}
		return NULL;

	default:
		return NULL;
	}
}

t_node *getE() {
	t_node *val = getT();

	while (*S == '+' || *S == '-') {
		int op = (*S == '+') ? ADD : SUB;
		incrementPointer();
		t_node *opNode = createNode(OP, op, val, getT());

		val = opNode;
	}

	return val;
}

t_node *getT() {
	t_node *val = getP();

	while (*S == '*' || *S == '/') {
		int op = (*S == '*') ? MUL : DIV;
		incrementPointer();
		t_node *opNode = createNode(OP, op, val, getP());

		val = opNode;
	}

	return val;
}

t_node *getP() {
	if (*S == '(') {
		incrementPointer();
		t_node *val = getE();
		sassert(*S == ')');
		incrementPointer();
		return val;
	}

	if (isdigit(*S) || *S == '+' || *S == '-')
		return getN();

	if (S[getWordLength()] == '(') { //TODO right check for function call
		return getFunctionCall();
	}

	return getId();
}

t_node *getN() {
	sassert(isdigit(*S) || ((*S == '+' || *S == '-') && isdigit(*(S + 1))));

	int constVal = 0, nextPos = 0;
	sscanf(S, "%d%n", &constVal, &nextPos);
	if (nextPos == 0)
		return NULL;

	S += nextPos - 1;
	incrementPointer();
	return createNode(CONST, constVal, NULL, NULL);
}

t_node *getId() {
	size_t len = getWordLength();

	sassert(len != 0);

	char savedChar = S[len];
	S[len] = '\0';

	mapResetErrno(&Vars);
	mapGet(&Vars, S);

	if (mapErrno(&Vars) == MAP_NO_SUCH_ELEMENT) {
		printf("unknown variable on line %d: %s\n", CurLine, S);
		Error = 1;

		S[len] = savedChar;
		S += len - 1;
		incrementPointer();

		return 0;
	}

	t_node *val = createNode(VAR, mapGet(&Vars, S), NULL, NULL);

	S[len] = savedChar;
	S += len - 1;
	incrementPointer();


	return val;
}

t_node *getFunctionCall() {
	if (isNextWord("in")) {
		S++;
		incrementPointer();

		sassert(*S == '(');
		incrementPointer();

		sassert(*S == ')');
		incrementPointer();

		return createNode(OP, IN, NULL, NULL);
	} else if (isNextWord("out")) {
		S += 2;
		incrementPointer();

		sassert(*S == '(');
		incrementPointer();

		t_node *arg = getE();

		sassert(*S == ')');
		incrementPointer();

		return createNode(OP, OUT, NULL, arg);
	} else {

		size_t len = getWordLength();
		if (len == 0)
			return NULL;

		char savedChar = S[len];
		S[len] = '\0';

		char *funcName = calloc(len + 1, sizeof(char));
		strcpy(funcName, S);
		S[len] = savedChar;

		mapResetErrno(&Functions);
		int argNum = mapGet(&Functions, funcName);

		if (mapErrno(&Functions) != MAP_NO_SUCH_ELEMENT) {
			t_node *val = createNode(FUNCTION, mapGet(&FunctionsNums, funcName), NULL, NULL);
			S += len - 1;
			incrementPointer();

			sassert(*S == '(');
			incrementPointer();

			if (*S != ')') {
				t_node *args = getArgList();
				if (getArgCount(args) != argNum) {
					printf("wrong args count on line %d: expected %d, got %d\n", CurLine, argNum, getArgCount(args));
					Error = 1;

					sassert(*S == ')');
					incrementPointer();

					freeNodes(args);
					freeNodes(val);
					return NULL;
				}
				val->right = args;
			} else if (argNum != 0) {
				printf("wrong args count on line %d: expected %d, got %d\n", CurLine, argNum, 0);
				Error = 1;

				sassert(*S == ')');
				incrementPointer();

				freeNodes(val);
				return NULL;
			}

			sassert(*S == ')');
			incrementPointer();
			return val;
		}
		printf("unknown function %s on line %d\n", funcName, CurLine);
		Error = 1;
		S += len - 1;
		incrementPointer();
		incrementPointer(); // '('

		freeNodes(getArgList());

		incrementPointer(); // ')'

		free(funcName);
		return NULL;
	}
}

t_node *getArgList() {
	t_node *args = getE();

	while (*S == ',') {
		incrementPointer();
		args = createNode(OP, SEPARATOR, args, getE());
	}

	return args;
}

size_t getWordLength() {
	if (!isalpha(*S) && *S != '_')
		return 0;

	size_t len = 0;
	while (isalnum(S[len]) || S[len] == '_')
		len++;

	return len;
}

int isNextWord(const char *word) {
	size_t len = getWordLength();
	char savedChar = S[len];
	S[len] = '\0';

	int res = strcmp(S, word);

	S[len] = savedChar;

	return res == 0;
}
