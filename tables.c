
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tables.h"

// Literals Table
// ----------------------------------------------------------------------------

#define LITERAL_MAX_SIZE 128
#define LITERALS_TABLE_MAX_SIZE 100

struct lit_table {
    char t[LITERALS_TABLE_MAX_SIZE][LITERAL_MAX_SIZE];
    int size;
};

LitTable* create_lit_table() {
    LitTable *lt = malloc(sizeof * lt);
    lt->size = 0;
    return lt;
}

int lookup_lit(LitTable* lt, char* s) {
    int i;
    for (i = 0; i < lt->size; i++) {
        if (strcpy(lt->t[lt->size], s) == 0) {
            return i;
        }
    }
    return -1;
}

int add_literal(LitTable* lt, char* s) {
    int i;
    for (i = 0; i < lt->size; i++) {
        if (strcmp(lt->t[i], s) == 0) {
            return i;
        }
    }
    strcpy(lt->t[lt->size], s);
    int idx_added = lt->size;
    lt->size++;
    return idx_added;
}

char* get_literal(LitTable* lt, int i) {
    return lt->t[i];
}

void print_lit_table(LitTable* lt) {
    printf("Literals table:\n");
    int i;
    for (i = 0; i < lt->size; i++) {
        printf("Entry %d -- %s\n", i, get_literal(lt, i));
    }
}

void free_lit_table(LitTable* lt) {
    free(lt);
}

// Symbols Table
// ----------------------------------------------------------------------------

#define SYMBOL_MAX_SIZE 128
#define SYMBOL_TABLE_MAX_SIZE 100

typedef struct {
  char name[SYMBOL_MAX_SIZE];
  int line;
  int escopo;
  int tamanho;
  int offset;
} Entry;

struct sym_table {
    Entry t[SYMBOL_TABLE_MAX_SIZE];
    int size;
};

SymTable* create_sym_table() {
    SymTable *st = malloc(sizeof * st);
    st->size = 0;
    return st;
}

int lookup_var(SymTable* st, char* s,int scope) {
    int i;
    for (i = 0; i < st->size; i++) {
        if (strcmp(st->t[i].name, s) == 0 && st->t[i].escopo == scope) {
            return i;
        }
    }
    return -1;
}

int add_var(SymTable* st, char* s, int line, int escopo, int tamanho) {
    strcpy(st->t[st->size].name, s);
    st->t[st->size].line = line;
    st->t[st->size].escopo = escopo;
    st->t[st->size].tamanho = tamanho;
    int idx_added = st->size;
    st->size++;
    return idx_added;
}

void add_offset(SymTable* st, int offset, int i){
	 st->t[i].offset = offset; 
}

char* get_name(SymTable* st, int i) {
    return st->t[i].name;
}

int get_line(SymTable* st, int i) {
    return st->t[i].line;
}

int get_escopo(SymTable* st, int i) {
    return st->t[i].escopo;
}

int get_tamanho(SymTable* st, int i) {
    return st->t[i].tamanho;
}

int get_offset(SymTable* st, int i){
	 return st->t[i].offset;
}

void print_sym_table(SymTable* st) {
    printf("Variables table:\n");
    int i;
    for (i = 0; i < st->size; i++) {
         printf("Entry %d -- name: %s, line: %d, scope: %d, size: %d\n", i, get_name(st, i), get_line(st, i), get_escopo(st, i), get_tamanho(st, i));
    }
}

void free_sym_table(SymTable* st) {
    free(st);
}

// Function Table
// ----------------------------------------------------------------------------

#define FUNCTION_MAX_SIZE 128
#define FUNCTION_TABLE_MAX_SIZE 100

typedef struct {
  char name[FUNCTION_MAX_SIZE];
  int line;
  int aridade;

} Func;

struct fun_table {
    Func t[FUNCTION_TABLE_MAX_SIZE];
    int size;
};

FunTable* create_fun_table() {
    FunTable *fun = malloc(sizeof * fun);
    fun->size = 0;
    return fun;
}

int lookup_fun(FunTable* fun, char* s) {
    int i;
    for (i = 0; i < fun->size; i++) {
        if (strcmp(fun->t[i].name, s) == 0) {
            return i;
        }
    }
    return -1;
}

int add_fun(FunTable* fun, char* s, int line, int aridade) {
    strcpy(fun->t[fun->size].name, s);
    fun->t[fun->size].line = line;
    fun->t[fun->size].aridade = aridade;
    int idx_added = fun->size;
    fun->size++;
    return idx_added;
}

char* get_name_fun(FunTable* fun, int i) {
    return fun->t[i].name;
}

int get_line_fun(FunTable* fun, int i) {
    return fun->t[i].line;
}

int get_aridade_fun(FunTable* fun, int i) {
    return fun->t[i].aridade;
}

void print_fun_table(FunTable* fun) {
    printf("Functions table:\n");
    int i;
    for (i = 0; i < fun->size; i++) {
         printf("Entry %d -- name: %s, line: %d, arity: %d\n", i, get_name_fun(fun, i), get_line_fun(fun, i), get_aridade_fun(fun, i));
    }
}

void free_fun_table(FunTable* fun) {
    free(fun);
}
