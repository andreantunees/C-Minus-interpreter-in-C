#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

void run_ast(AST *ast);
void rec_run_ast(AST *ast);
void remove_linebreak(char* str);
void remove_char(char *str, char garbage);
int VerificaNaoVariavel(AST* ast);
#endif
