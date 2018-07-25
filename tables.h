#include "ast.h"
#ifndef TABLES_H
#define TABLES_H

// Literals Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
struct lit_table;
typedef struct lit_table LitTable;

// Creates an empty literal table.
LitTable* create_lit_table();

// Returns the index where the given variable is stored or -1 otherwise.
int lookup_lit(LitTable* lt, char* s);
// Adds the given string to the table without repetitions.
// String 's' is copied internally.
// Returns the index of the string in the table.
int add_literal(LitTable* lt, char* s);

// Returns a pointer to the string stored at index 'i'.
char* get_literal(LitTable* lt, int i);

// Prints the given table to stdout.
void print_lit_table(LitTable* lt);

// Clears the allocated structure.
void free_lit_table(LitTable* lt);


// Symbols Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
// This table only stores the variable name and the declaration line.
struct sym_table;
typedef struct sym_table SymTable;

// Creates an empty symbol table.
SymTable* create_sym_table();

// Adds a fresh var to the table.
// No check is made by this function, so make sure to call 'lookup_var' first.
// Returns the index where the variable was inserted.
int add_var(SymTable* st, char* s, int line, int escopo, int tamanho);

void add_offset(SymTable* st, int offset, int i);
// Returns the index where the given variable is stored or -1 otherwise.
int lookup_var(SymTable* st, char* s,int scope);

// Returns the variable name stored at the given index.
// No check is made by this function, so make sure that the index is valid first.
char* get_name(SymTable* st, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid first.
int get_line(SymTable* st, int i);

int get_escopo(SymTable* st, int i);

int get_tamanho(SymTable* st, int i);

int get_offset(SymTable* st, int i);

// Prints the given table to stdout.
void print_sym_table(SymTable* st);

// Clears the allocated structure.
void free_sym_table(SymTable* st);

// Function Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
// This table only stores the variable name and the declaration line.
struct fun_table;
typedef struct fun_table FunTable;

// Creates an empty symbol table.
FunTable* create_fun_table();

// Adds a fresh var to the table.
// No check is made by this function, so make sure to call 'lookup_fun' first.
// Returns the index where the variable was inserted.
int add_fun(FunTable* fun, char* s, int line, int aridade);

// Returns the index where the given variable is stored or -1 otherwise.
int lookup_fun(FunTable* fun, char* s);

// Returns the variable name stored at the given index.
// No check is made by this function, so make sure that the index is valid first.
char* get_name_fun(FunTable* fun, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid first.
int get_line_fun(FunTable* fun, int i);

int get_aridade_fun(FunTable* fun, int i);

// Prints the given table to stdout.
void print_fun_table(FunTable* fun);

// Clears the allocated structure.
void free_fun_table(FunTable* fun);


#endif // TABLES_H

