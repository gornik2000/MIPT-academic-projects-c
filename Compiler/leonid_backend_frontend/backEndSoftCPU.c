#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "textProcessor.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "map.h"

int *Functions = NULL;

int treeToAsm(t_node *root, char *file);
void getFunctions(t_node *node);
int getFunctionCount(t_node *node);

int main(int argc, char *argv[]) {
  if (argc < 2) {
		printf(" # Missing file\n");
		return 0;
	}

  char *fileInName  = argv[1];
  char *fileOutName = argv[2];

	tree t = {};
	treeCtor(&t);
	treeLoadFromFile(&t, fileInName);
//	treeDump(&t);
	Functions = calloc(getFunctionCount(t.root), sizeof(int));
	getFunctions(t.root);

	treeToAsm(t.root, fileOutName);

	treeDtor(&t);
}

int getVarCount(t_node *node);
int getArgCount(t_node *node);

int getFunctionCount(t_node *node) {
	if (node == NULL)
		return 0;

	if (node->val->type == FUNCTION_DECLARATION)
		return 1;

	return getFunctionCount(node->left) + getFunctionCount(node->right);
}

void getFunctions(t_node *node) {
	if (node == NULL)
		return;

	if (node->val->type == FUNCTION_DECLARATION) {
		Functions[node->val->val] = getVarCount(node->right);
	}

	getFunctions(node->left);
	getFunctions(node->right);
}

int getVarCount(t_node *node) {
	if (node == NULL)
		return 0;

	if (node->val->type == VAR)
		return node->val->val + 1;

	int right = getVarCount(node->right);
	int left = getVarCount(node->left);

	return (right > left) ? right : left;
}

int getArgCount(t_node *node) {
	if (node == NULL)
		return 0;

	if (node->val->type == FUNCTION_DECLARATION)
		return node->left->val->val;

	if (node->val->type == OP && node->val->val == SEPARATOR)
		return getArgCount(node->left) + getArgCount(node->right);

	return 1;
}

int ifCounter = 0, loopCounter = 0, argsCounter = 0;

int nodeToAsm(t_node *node, FILE *out) {
	if (node == NULL)
		return 0;

	switch (node->val->type) {
	case CONST:
		fprintf(out, "push %d\n", node->val->val);
		return 1;
	case VAR:
		fprintf(out, "pushr r%d\n", node->val->val);
		return 1;
	case FUNCTION:
		for (int i = Functions[node->val->val] - 1; i >= 0; i--) {
			fprintf(out, "pushr r%d\n", i);
		}
		if (node->right != NULL) {
			argsCounter = getArgCount(node->right);
			nodeToAsm(node->right, out);
			for (int i = 0; i < argsCounter; i++) {
				fprintf(out, "popr r%d\n", i > 14 ? 15 : i);
			}
			for (int i = argsCounter; i < Functions[node->val->val]; i++) {
				fprintf(out, "push 0\n"
							 "popr r%d\n", i);
			}
		}
		fprintf(out, "call func%d\n", node->val->val);
		fprintf(out, "popr r15\n");
		for (int i = 0; i < Functions[node->val->val]; i++) {
			fprintf(out, "popr r%d\n", i);
		}
		fprintf(out, "pushr r15\n");
		return 1;
	case FUNCTION_DECLARATION:
		fprintf(out, "func%d:\n", node->val->val);
		nodeToAsm(node->right, out);
		return 1;
	case OP:
		switch ((char) node->val->val) {
		case MAIN:
			fprintf(out, "main:\n");
			nodeToAsm(node->right, out);
			fprintf(out, "end\n");
			return 1;
		case SEPARATOR:
			nodeToAsm(node->left, out);
			fprintf(out, "\n");
			nodeToAsm(node->right, out);
			return 1;

		case LESS_EQUAL:
		case LESS:
		case GREATER_EQUAL:
		case GREATER:
		case EQUAL:
		case NOT_EQUAL:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			return 1;

		case ADD:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "add\n");
			return 1;
		case SUB:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "sub\n");
			return 1;
		case MUL:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "mul\n");
			return 1;
		case DIV:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "div\n");
			return 1;

		case IF:
			if (node->right->val->val != IF_SEPARATOR) {
				nodeToAsm(node->left, out);
				int counter = ifCounter;
				if (node->left->val->type != OP) {
					fprintf(out, "push 0\n"
								 "je if%d_end\n", counter);
				} else {
					switch (node->left->val->val) {
					case LESS_EQUAL:
						fprintf(out, "ja if%d_end\n", counter);
						break;
					case LESS:
						fprintf(out, "jae if%d_end\n", counter);
						break;
					case GREATER_EQUAL:
						fprintf(out, "jb if%d_end\n", counter);
						break;
					case GREATER:
						fprintf(out, "jbe if%d_end\n", counter);
						break;
					case EQUAL:
						fprintf(out, "jne if%d_end\n", counter);
						break;
					case NOT_EQUAL:
						fprintf(out, "je if%d_end\n", counter);
						break;
					default:
						fprintf(out, "push 0\n"
									 "je if%d_end\n", counter);
					}
				}
				ifCounter++;
				nodeToAsm(node->right, out);
				fprintf(out, "if%d_end:\n", counter);
				return 1;
			} else {
				nodeToAsm(node->left, out);
				int counter = ifCounter;
				if (node->left->val->type != OP) {
					fprintf(out, "push 0\n"
								 "je else%d\n", counter);
				} else {
					switch (node->left->val->val) {
					case LESS_EQUAL:
						fprintf(out, "ja else%d\n", counter);
						break;
					case LESS:
						fprintf(out, "jae else%d\n", counter);
						break;
					case GREATER_EQUAL:
						fprintf(out, "jb else%d\n", counter);
						break;
					case GREATER:
						fprintf(out, "jbe else%d\n", counter);
						break;
					case EQUAL:
						fprintf(out, "jne else%d\n", counter);
						break;
					case NOT_EQUAL:
						fprintf(out, "je else%d\n", counter);
						break;
					default:
						fprintf(out, "push 0\n"
									 "je else%d\n", counter);
					}
				}
				ifCounter++;
				nodeToAsm(node->right->left, out);
				fprintf(out, "jmp if%d_end\n"
							 "else%d:\n", counter, counter);
				nodeToAsm(node->right->right, out);
				fprintf(out, "if%d_end:\n", counter);
				return 1;
			}
		case WHILE:
			fprintf(out, "loop%d:\n", loopCounter);
			int counter = loopCounter;
			nodeToAsm(node->left, out);
			if (node->left->val->type != OP) {
				fprintf(out, "push 0\n"
							 "je loop%d_end\n", counter);
			} else {
				switch (node->left->val->val) {
				case LESS_EQUAL:
					fprintf(out, "ja loop%d_end\n", counter);
					break;
				case LESS:
					fprintf(out, "jae loop%d_end\n", counter);
					break;
				case GREATER_EQUAL:
					fprintf(out, "jb loop%d_end\n", counter);
					break;
				case GREATER:
					fprintf(out, "jbe loop%d_end\n", counter);
					break;
				case EQUAL:
					fprintf(out, "jne loop%d_end\n", counter);
					break;
				case NOT_EQUAL:
					fprintf(out, "je loop%d_end\n", counter);
					break;
				default:
					fprintf(out, "push 0\n"
								 "je loop%d_end\n", counter);
				}
			}
			loopCounter++;
			nodeToAsm(node->right, out);
			fprintf(out, "jmp loop%d\n", counter);
			fprintf(out, "loop%d_end:\n", counter);
			return 1;

		case ASSIGMENT:
			nodeToAsm(node->right, out);
			fprintf(out, "popr r%d\n", node->left->val->val);
			return 1;
//
//		case 'm':
//			fprintf(out, "meow\n");
//			return 1;
//		case 'd':
//			fprintf(out, "dump\n");
//			return 1;

//		case 's':
//			nodeToAsm(node->right, out);
//			fprintf(out, "sqrt\n");
//			return 1;

		case OUT:
			nodeToAsm(node->right, out);
			fprintf(out, "out\n");
			return 1;
		case IN:
			fprintf(out, "in\n");
			return 1;

		case RETURN:
			if (node->right == NULL)
				fprintf(out, "push 0\n");
			else
				nodeToAsm(node->right, out);
			fprintf(out, "ret\n");
			return 1;

		default:
			return 0;
		}
	default:
		return 0;
	}
}

int treeToAsm(t_node *root, char *file) {
	if (root == NULL)
		return 0;

	FILE *out = fopen(file, "wb");

	fprintf(out, "jmp main\n");
	nodeToAsm(root, out);

	fclose(out);

	return 1;
}
