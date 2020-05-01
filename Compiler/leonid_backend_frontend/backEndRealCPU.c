#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "textProcessor.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "map.h"
#include "stack.h"

map Functions;

int treeToAsm(t_node *root, char *file);

int main(int argc, char *argv[]) {
  if (argc < 2) {
		printf(" # Missing file\n");
		return 0;
	}

  char *fileInName  = argv[1];
  char *fileOutName = argv[2];

	mapCtor(&Functions);

	tree t = {};
	treeCtor(&t);
	treeLoadFromFile(&t, fileInName);
//	treeDump(&t);

	treeToAsm(t.root, fileOutName);

	treeDtor(&t);
	mapDtor(&Functions);
}

int getVarCount(t_node *node);
int getArgCount(t_node *node);

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

int IfCounter = 0, LoopCounter = 0, ArgsCounter = 0, Main = 0;

int computeOffset(int varNum) {
	return (varNum < ArgsCounter) ? ((ArgsCounter - varNum + 1) * 8) : ((varNum - ArgsCounter + 1) * -8);
}

int nodeToAsm(t_node *node, FILE *out) {
	if (node == NULL)
		return 0;

	switch (node->val->type) {
	case CONST:
		fprintf(out, "mov rax, %d\n"
					 "push rax\n", node->val->val);
		return 1;
	case VAR:
		fprintf(out, "push [rbp + (%d)]\n", computeOffset(node->val->val));
		return 1;
	case FUNCTION:
		fprintf(out, "sub rsp, 8\n");
		nodeToAsm(node->right, out);
		fprintf(out, "call func%d\n"
					 "add rsp, %d\n", node->val->val, getArgCount(node->right) * 8);
		return 1;
	case FUNCTION_DECLARATION:
		ArgsCounter = getArgCount(node);
		fprintf(out, "func%d:\n"
					 "push rbp\n"
					 "mov rbp, rsp\n"
					 "sub rsp, %d\n", node->val->val, (getVarCount(node->right) - ArgsCounter) * 8);
		nodeToAsm(node->right, out);
		ArgsCounter = 0;
		return 1;
	case OP:
		switch ((char) node->val->val) {
		case MAIN:
			fprintf(out, "_start:\n"
						 "push rbp\n"
						 "mov rbp, rsp\n"
						 "sub rsp, %d\n", getVarCount(node->right) * 8);
			Main = 1;
			nodeToAsm(node->right, out);
			fprintf(out, "leave\n"
						 "mov rax, 0x3c\n"
						 "xor rdi, rdi\n"
						 "syscall\n");
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
			fprintf(out, "pop rbx\n"
						 "pop rax\n"
						 "add rax, rbx\n"
						 "push rax\n");
			return 1;
		case SUB:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "pop rbx\n"
						 "pop rax\n"
						 "sub rax, rbx\n"
						 "push rax\n");
			return 1;
		case MUL:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "pop rbx\n"
						 "pop rax\n"
						 "xor rdx, rdx\n"
						 "imul rbx\n"
						 "push rax\n");
			return 1;
		case DIV:
			nodeToAsm(node->left, out);
			nodeToAsm(node->right, out);
			fprintf(out, "pop rbx\n"
						 "pop rax\n"
						 "xor rdx, rdx\n"
						 "idiv rbx\n"
						 "push rax\n");
			return 1;

		case IF:
			if (node->right->val->val != '|') {
				nodeToAsm(node->left, out);
				int counter = IfCounter;
				if (node->left->val->type != OP) {
					fprintf(out, "pop rax\n"
								 "test rax, rax\n"
								 "je if%d_end\n", counter);
				} else {
					fprintf(out, "pop rbx\n"
								 "pop rax\n"
								 "cmp rax, rbx\n");
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
						fprintf(out, "push rax\n"
									 "test rbx, rbx\n"
									 "je if%d_end\n", counter);
					}
				}
				IfCounter++;
				nodeToAsm(node->right, out);
				fprintf(out, "if%d_end:\n", counter);
			} else {
				nodeToAsm(node->left, out);
				int counter = IfCounter;
				if (node->left->val->type != OP) {
					fprintf(out, "pop rax\n"
								 "test rax, rax\n"
								 "je else%d\n", counter);
				} else {
					fprintf(out, "pop rbx\n"
								 "pop rax\n"
								 "cmp rax, rbx\n");
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
						fprintf(out, "push rax\n"
									 "test rbx, rbx\n"
									 "je else%d\n", counter);
					}
				}
				IfCounter++;
				nodeToAsm(node->right->left, out);
				fprintf(out, "jmp if%d_end\n"
							 "else%d:\n", counter, counter);
				nodeToAsm(node->right->right, out);
				fprintf(out, "if%d_end:\n", counter);
			}

			return 1;
		case WHILE:

			fprintf(out, "loop%d:\n", LoopCounter);
			int counter = LoopCounter;
			nodeToAsm(node->left, out);
			if (node->left->val->type != OP) {
				fprintf(out, "pop rax\n"
							 "test rax, rax\n"
							 "je loop%d_end\n", counter);
			} else {
				fprintf(out, "pop rbx\n"
							 "pop rax\n"
							 "cmp rax, rbx\n");
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
					fprintf(out, "push rax\n"
								 "test rbx, rbx\n"
								 "je loop%d_end\n", counter);
				}
			}
			LoopCounter++;
			nodeToAsm(node->right, out);
			fprintf(out, "jmp loop%d\n", counter);
			fprintf(out, "loop%d_end:\n", counter);

			return 1;

		case ASSIGMENT:
			nodeToAsm(node->right, out);
			fprintf(out, "pop [rbp + (%d)]\n", computeOffset(node->left->val->val));
			return 1;

		case OUT:
			nodeToAsm(node->right, out);
			fprintf(out, "call out\n");
			return 1;
		case IN:
			fprintf(out, "sub rsp, 8\n"
						 "call in\n");
			return 1;

		case RETURN:

			if (node->right == NULL)
				fprintf(out, "mov rax, 0\n"
							 "push rax\n");
			else
				nodeToAsm(node->right, out);
			if (!Main) {
				fprintf(out, "pop rax\n"
							 "mov [rbp + (%d)], rax\n"
							 "leave\n"
							 "ret\n", computeOffset(-1));
			} else {
				fprintf(out, "xor rdi, rdi\n"
							 "leave\n"
							 "mov rax, 0x3c\n"
							 "syscall\n");
			}
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

	fprintf(out, ".intel_syntax noprefix\n"
				 "\n"
				 "\t\t.globl _start\n"
				 "\n"
				 ".text\n");
	nodeToAsm(root, out);

	fclose(out);

	return 1;
}
