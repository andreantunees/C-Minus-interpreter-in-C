#ifndef AST_H
#define AST_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHILDREN_LIMIT 20

typedef enum {
	IDENTIFICADOR,
	INPUT_NODE,
	OUTPUT_NODE,

	STMT_LIST_NODE,
	ELSE_NODE,
	WHILE_NODE,
	IF_NODE,

	USED_NODE_N,
	USED_NODE_V,
	VAR_DCL_N,
	VAR_DCL_V,
	VAR_LIST_NODE,

	FUNC_HEADER_NODE,
	FUNC_DECL_NODE,
	FCALL_NODE,
	ARG_LIST_NODE,
	FUN_NODE,
	FUNC_DECL_LIST_NODE,
	FUNC_BODY_NODE,

	PARAM_NODE,

	NUM_NODE,
	STRING_NODE,

	WRITE_NODE,
	RETURN_NODE,

	LT_NODE,   
	LE_NODE,    
	PLUS_NODE,
	MINUS_NODE, 
	GE_NODE, 
	NEQ_NODE,   
	GT_NODE,   
	TIMES_NODE,
	ASSIGN_NODE,
	EQ_NODE,
	OVER_NODE
} NodeKind;

typedef struct node AST;

struct node {
    NodeKind kind;
    int      data;
    char*    text;
    int      count;
    AST*     child[CHILDREN_LIMIT];
};

NodeKind get_kind(AST* node);

AST* new_node(NodeKind kind, int count, ...);
AST* new_leaf(NodeKind kind, int data);
AST* make_node(NodeKind kind, int data, char* text);
AST* get_child(AST *parent, int idx);

int get_count(AST *node);
int get_data(AST *node);

char* node2str(NodeKind kind);

void add_child(AST *parent, AST *child);
void free_tree(AST *tree);

#endif