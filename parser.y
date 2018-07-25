
/* opcoes do bison */
// arquivo para geracao do parse.
%output "parser.c"
// produz um 'parser.h'
%defines "parser.h"
// Fornece mensagens de erro adequadas quando um erro de sintaxe for encontrado.
%define parse.error verbose
// Ativar LAC (lookahead correction) para melhorar o tratamento de erros de sintaxe.
%define parse.lac full
// Ativar a opção de rastreio para que a depuração seja possível.
%define parse.trace

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tables.h"
#include "interpreter.h"


LitTable* lt;
SymTable* sy;
FunTable* fun;

extern char* yytext;
extern int yylineno;

int scope = 0;
int qtdparam = 0;
int callparam = 0;
int indice = 0;

int yylex(void);
void yyerror(char const *s);

int check_var(char *id, int scope);
int new_var(char *id, int i, int scope);

int new_string(char *string);

int check_fun(char *id);
int new_fun(char *id);

AST *tree;
%}

%define api.value.type {AST *}

%token LBRACE
%token COMMA
%token RBRACE
%token VOID
%token INT
%token LPAREN
%token RPAREN
%token RBRACK
%token LBRACK
%token ELSE
%token ID
%token IF
%token NUM
%token SEMI
%token ASSIGN
%token WHILE
%token RETURN
%token INPUT
%token OUTPUT
%token WRITE
%token STRING
%token PLUS
%token MINUS
%token LT
%token EQ
%token TIMES
%token OVER
%token LE
%token GT
%token GE
%token NEQ

%left LT GT LE GE NEQ EQ
%left PLUS MINUS
%left TIMES OVER

%start program

%%

program:
func-decl-list														        { tree = $1; }
;

func-decl-list:
func-decl-list { scope++; } func-decl             { add_child($1, $3); }
| func-decl																        { $$ = new_node(FUNC_DECL_LIST_NODE, 1, $1); }
;

func-decl:
func-header func-body												      { $$ = new_node(FUNC_DECL_NODE, 2, $1, $2);}
;

func-header:
ret-type ID LPAREN params RPAREN						      { indice = new_fun($2->text); AST* nodefun= make_node(FUN_NODE, indice, $2->text); $$ = new_node(FUNC_HEADER_NODE, 2, nodefun, $4); free($2);}
;

func-body:
LBRACE opt-var-decl opt-stmt-list RBRACE			    { $$ = new_node(FUNC_BODY_NODE, 2, $2, $3); }
;

opt-var-decl:
%empty																            { $$ = new_node(VAR_LIST_NODE, 0); }
| var-decl-list															      { $$ = $1; }
;

opt-stmt-list:
  stmt-list                                                                                  
;

ret-type:
INT				{$$ = $1;}													             
| VOID						{$$ = $1;}											         
;

params:
VOID																	            { $$ = new_node(PARAM_NODE, 0); } 
| param-list															        { $$ = $1; }
;

param-list:
param-list COMMA param                            { qtdparam++; add_child($1, $3); }
| param                                           { qtdparam++; $$ = new_node(PARAM_NODE, 1, $1); }

;

param:
INT ID																            { indice = new_var($2->text,0,scope); $$ = new_leaf(VAR_DCL_N, indice); free($2);}
| INT ID LBRACK RBRACK												    { indice = new_var($2->text,-1,scope); $$ = new_leaf(VAR_DCL_V, indice); free($2);}
;

var-decl-list:
var-decl-list var-decl                            { add_child($1, $2); }
| var-decl																        { $$ = new_node(VAR_LIST_NODE, 1, $1); }
;

var-decl:
INT ID SEMI															          { indice = new_var($2->text,0,scope); $$ = new_leaf(VAR_DCL_N, indice);  free($2);}
| INT ID LBRACK NUM RBRACK SEMI									  { indice = new_var($2->text,atoi($4->text),scope) ; $$ = new_leaf(VAR_DCL_V, indice); free($2);free($4);}
;

block:
LBRACE opt-stmt-list RBRACE										    { $$ = $2; }
;

stmt-list:
%empty																            { $$ = new_node(STMT_LIST_NODE, 0); }
| stmt-list stmt														      { add_child($1, $2); }
;

stmt:
assign-stmt															          { $$ = $1; }
| if-stmt																          { $$ = $1; }
| while-stmt															        { $$ = $1; }
| return-stmt															        { $$ = $1; }
| func-call SEMI														      { $$ = $1; }
;

assign-stmt:
lval ASSIGN arith-expr SEMI										    { $$ = new_node(ASSIGN_NODE, 2, $1, $3); }
;

lval:
ID																	              { indice = check_var($1->text,scope); $$ = new_leaf(USED_NODE_N, indice); free($1);}
| ID LBRACK NUM RBRACK													  { indice = check_var($1->text,scope); free($1); }
| ID LBRACK ID RBRACK													    { indice = check_var($1->text,scope); AST* nodelval= new_leaf(USED_NODE_V, indice); indice = check_var($3->text,scope);add_child(nodelval, $$ = new_leaf(USED_NODE_N, indice)); $$ = nodelval; free($1);free($3);}
;

if-stmt:
IF LPAREN bool-expr RPAREN block							    { $$ = new_node(IF_NODE, 2, $3, $5); }
| IF LPAREN bool-expr RPAREN block ELSE block		  { $$ = new_node(IF_NODE, 3, $3, $5, $7); }
;

while-stmt:
  WHILE LPAREN bool-expr RPAREN block						  { $$ = new_node(WHILE_NODE, 2, $3, $5); }
;

return-stmt:
  RETURN SEMI															        { $$ = new_node(RETURN_NODE, 0); }
| RETURN arith-expr SEMI												  { $$ = new_node(RETURN_NODE, 1, $2); }
;

func-call:
  output-call															        { $$ = $1; }
| write-call															        { $$ = $1; }
| user-func-call														      { $$ = $1; }
;

input-call:
  INPUT LPAREN RPAREN													    { $$ = new_node(INPUT_NODE, 0); }
;

output-call:
  OUTPUT LPAREN arith-expr RPAREN								  { $$ = new_node(OUTPUT_NODE, 1, $3); }
;

write-call:
  WRITE LPAREN STRING RPAREN										  { indice = new_string($3->text); $$ = new_node(WRITE_NODE, 1, make_node(STRING_NODE, indice, $3->text)); free($3);}
;

user-func-call:
  ID LPAREN opt-arg-list RPAREN								    { indice = check_fun($1->text); AST* nodefuncall = new_leaf(FCALL_NODE, indice); add_child(nodefuncall, $3); $$ = nodefuncall; free($1);}
;

opt-arg-list:
  %empty                                         
| arg-list																        { $$ = $1; }
;

arg-list:
  arith-expr															        { callparam++; $$ = new_node(ARG_LIST_NODE, 1, $1); }
| arg-list COMMA arith-expr                       { callparam++; add_child($1, $3); }
;


bool-expr: 
  arith-expr LT arith-expr												{ $$ = new_node(LT_NODE, 2, $1, $3); }
| arith-expr LE arith-expr												{ $$ = new_node(LE_NODE, 2, $1, $3); }
| arith-expr GT arith-expr												{ $$ = new_node(GT_NODE, 2, $1, $3); }
| arith-expr GE arith-expr												{ $$ = new_node(GE_NODE, 2, $1, $3); }
| arith-expr EQ arith-expr												{ $$ = new_node(EQ_NODE, 2, $1, $3); }
| arith-expr NEQ arith-expr												{ $$ = new_node(NEQ_NODE, 2, $1, $3); }
;

arith-expr: 
  NUM                                             { $$ = make_node(NUM_NODE, -1, $1->text); free($1);}
| input-call															        { $$ = $1; }
| lval																	          { $$ = $1; }
| user-func-call														      { $$ = $1; }
| LPAREN arith-expr RPAREN												{ $$ = $2; }
| arith-expr PLUS arith-expr											{ $$ = new_node(PLUS_NODE , 2, $1, $3);}
| arith-expr MINUS arith-expr											{ $$ = new_node(MINUS_NODE, 2, $1, $3);}
| arith-expr TIMES arith-expr											{ $$ = new_node(TIMES_NODE, 2, $1, $3);}
| arith-expr OVER arith-expr											{ $$ = new_node(OVER_NODE , 2, $1, $3);}
;

%%

// funcao de verificacao se a funcao foi declarada ou esta com a quantidade de argumentos 
// sendo chamad corretamente
int check_fun(char *id){
  int idx = lookup_fun(fun, id);
  int aridade = 0;
    if (idx == -1) {
        printf("SEMANTIC ERROR (%d): function '%s' was not declared.\n", yylineno, id);
        exit(1);
    }else if ((aridade = get_aridade_fun(fun, idx)) != callparam )
    {
      printf("SEMANTIC ERROR (%d): function '%s' was called with %d arguments but declared with %d parameters.\n", yylineno, id, callparam, aridade);
      exit(1);
    }
  callparam = 0;
  return idx;
}

// funcao que adiciona funções na tabela de funcao
int new_fun(char *id){
  int idx = lookup_fun(fun, id);
    if (idx != -1) {
        printf("SEMANTIC ERROR (%d): function '%s' already declared at line %d.\n", yylineno, id, get_line_fun(fun, idx));
        exit(1);
    }
    idx = add_fun(fun, id, yylineno, qtdparam);
  qtdparam = 0;
  return idx;
}


// funcao de adicionar strings na tabela de literais
int new_string(char *string){
  int idx = lookup_lit(lt, string);
  if (idx == -1) {
    idx = add_literal(lt, string);
  }
  return idx;
}

//funcao de verificacao da existencia da variavel
int check_var(char *id, int scope) {
  int idx = lookup_var(sy, id, scope);
  if (idx == -1) {
      printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n",
              yylineno, id);
      exit(1);
  }
  return idx;
}

// funcao para adicionar nova variavel de symbolos na tabela de symbolos
int new_var(char *id, int i, int scope){
    int idx = lookup_var(sy, id, scope);
    if (idx != -1) {
        printf("SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n", yylineno, id, get_line(sy, idx));
        exit(1);
    }
    idx = add_var(sy, id, yylineno, scope, i);
    return idx;
}

//funcao para erro do parse
void yyerror (char const *s) {
  printf("PARSE ERROR (%d): %s\n", yylineno, s);

}


int main() {
    yydebug = 0;

    //cria as tabels
    lt = create_lit_table();
    sy = create_sym_table();
    fun = create_fun_table();

    if (yyparse() == 0) {
      //printf("PARSE SUCCESSFUL!");
      stdin = fopen(ctermid(NULL), "r");
      run_ast(tree);
      //imprime as tabelas
      printf("\n\n");
      //print_lit_table(lt); printf("\n\n");
      //print_sym_table(sy); printf("\n\n");
      //print_fun_table(fun);printf("\n\n");
    }

      free_tree(tree);
      free_lit_table(lt);
      free_sym_table(sy);
      free_fun_table(fun);
      
    return 0;
}