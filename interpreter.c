#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "tables.h"

extern LitTable* lt;
extern SymTable* sy;
extern FunTable* fun;

int frame = 0;
int offset = 0;
// Pilha -----------------------------------------------------------------

#define STACK_SIZE 200

int stack[STACK_SIZE];
int sp; // stack pointer

void push(int x) {
    stack[++sp] = x;
}

int pop() {
    return stack[sp--];
}

void init_stack() {
	int i;
    for ( i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
    }
    sp = -1;
}

void print_stack() {
    printf("*** STACK: ");
    int i;
    for ( i = 0; i <= sp; i++) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

// ----------------------------------------------------------------------------

// Memoria --------------------------------------------------------------------

#define MEM_SIZE 200

int mem[MEM_SIZE];

void store(int addr, int val) {
    mem[addr] = val;
}

int load(int addr) {
    return mem[addr];
}

void init_mem() {
	int addr;
    for ( addr = 0; addr < MEM_SIZE; addr++) {
        mem[addr] = 0;
    }
}

// ----------------------------------------------------------------------------

// Funcões --------------------------------------------------------------------


void run_var_dcl_vector(AST* ast){
	push(load(get_data(ast)));
}

void run_var_dcl_no_vector(AST* ast){
	int i;
    for (i = 0; i < get_count(ast); i++)
    {
        rec_run_ast(get_child(ast, i));
    }
}

void run_func_call(AST* ast){
	return;
}

void run_stmt_list(AST* ast){
	int i;
	for(i = 0; i < get_count(ast); i++){
		rec_run_ast(get_child(ast,i));
	}
}

void run_var_list(AST* ast){

    int i;
    return; //try

    for (i = 0; i < get_count(ast); i++)
    {
        rec_run_ast(get_child(ast, i));
    }
}

void run_func_body(AST* ast){
	 int i;

    for (i = 0; i < get_count(ast); i++)
    {
        rec_run_ast(get_child(ast, i));
    }
}

void run_param_list(AST* ast){
	 int i;
	 for (i = 0; i < get_count(ast); i++)
    {
    	  //ind address = frame + offset++;
    	  //store(address,pop());
        rec_run_ast(get_child(ast, i));
    }
}

void run_write(AST* node) {
	char *imprimir = (char *) malloc(20 * sizeof(char));
	strcpy(imprimir, get_literal(lt, get_child(node,0)->data));
	
	//remover o \n
	remove_linebreak(imprimir);
	//remover "	
	remove_char(imprimir, '\"');
	
	printf("%s", imprimir);
	free(imprimir);
}

void run_while(AST* ast){
	 rec_run_ast(get_child(ast, 0));
    int test = pop();
    while(test){
    	rec_run_ast(get_child(ast,1));
    	rec_run_ast(get_child(ast,0));

    test = pop();
    }
}

void run_return(AST* ast){
	rec_run_ast(get_child(ast,0));
}

void run_output(AST* ast){
	rec_run_ast(get_child(ast,0));
	int vRetorno = pop();
	printf("%d", vRetorno);
}

void run_num(AST* ast){
	push(atoi(ast->text));
}

void run_input(AST* ast){
	int  valor;
	scanf("%d",&valor);
	printf("input: %d \n",valor);
	push(valor);
}

void run_if(AST* ast){
	 rec_run_ast(get_child(ast, 0));
    int removido = pop();
    if (removido == 0 && get_count(ast) == 3) {
        rec_run_ast(get_child(ast, 2));
    } else if (removido == 1) {
    	rec_run_ast(get_child(ast, 1));
    }
}

void run_func_list(AST* ast){
	int i;
	for(i = 0; i < get_count(ast); i++){
		rec_run_ast(get_child(ast,i));
	}
}

void run_func_decl(AST* ast){ 
	 int i;
    for (i = 0; i < get_count(ast); i++)
    {
        rec_run_ast(get_child(ast, i));
    }
}

void run_func_header(AST* ast){ //ver relacao pra quando for void o inicio da funcao (falta)
    return; //try
    //rec_run_ast(get_child(ast, get_count(as) -1));
}

void run_assign(AST* ast){
	 rec_run_ast(get_child(ast, 1));
    AST *child = get_child(ast, 0);
    int var_idx = get_data(child);
    store(var_idx, pop());
}

void run_arg_list(AST* ast){
	AST* node;
	int i;
	for(i = 0; i < get_count(ast); i++){
		node = get_child(ast,i);
		rec_run_ast(node);
		push(get_data(node));
	}
}

#define op() \
    rec_run_ast(get_child(ast, 0)); \
    rec_run_ast(get_child(ast, 1)); \
    int r = pop(); \
    int l = pop()

void run_lt(AST* ast){
	op();
    push(l < r);
}

void run_le(AST* ast){
	op();
    push(l <= r);
}

void run_gt(AST* ast){
	op();
    push(l > r);
}

void run_ge(AST* ast){
	op();
    push(l >= r);
}

void run_eq(AST* ast){
	op();
    push(l == r);
}

void run_neq(AST* ast){
	op();
    push(l + r);
}

void run_plus(AST* ast){
	op();
    push(l + r);
}

void run_minus(AST* ast){
	op();
    push(l - r);
}

void run_times(AST* ast){
	op();
    push(l * r);
}

void run_over(AST* ast){
	op();
    push((int) l / r);
}

void remove_char(char *str, char garbage) {
	char *src, *dst;

	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage) dst++;
	}

	*dst = '\0';
}

void remove_linebreak(char* str){
	char* i;
	for (i = str; *i; i++) {
		if (*i == '\\' && *(i + 1) == 'n') {
			*i = '\n';
			*(i + 1) = '\6';
		}
	}
	remove_char(str, '\6');
}

int VerificaNaoVariavel(AST* ast){
   char *p = (char *) ast->kind;	
   int valor = strcmp(p,"VAR_USED_N");
   int valoraux = strcmp(p,"USED_NODE_V");
	if( valor == 0 && valoraux == 0){
		free(p);
		return 1;
	} 
	free(p);
	return 0;
}

void rec_run_ast(AST *ast){
	
	switch(get_kind(ast)){
		  case LE_NODE:
		  	   run_le(ast);
		  	   break;
		  case GE_NODE:
		  		run_ge(ast);
		  		break;
		  case NEQ_NODE:
		  		run_neq(ast);
		  		break;
		  case GT_NODE:
		  		run_gt(ast);
		  		break;
		  case USED_NODE_N:
		  		run_var_dcl_vector(ast);
		  		break;		  
		  case USED_NODE_V:
		  		run_var_dcl_vector(ast);
		  		break;		  
		  case VAR_DCL_N:
		  		run_var_dcl_no_vector(ast);
		  		break;
		  case VAR_DCL_V:
		  		run_var_dcl_vector(ast);
		  		break;
		  case ASSIGN_NODE:
		      run_assign(ast);
		      break;
		  case ARG_LIST_NODE:
		  		run_arg_list(ast);
		  		break;
		  case FCALL_NODE:
		  		run_func_call(ast);
		  		break;
		  case INPUT_NODE:
		  		run_input(ast);
		  		break;
		  case OUTPUT_NODE:
		  		run_output(ast);
		  		break;
		  case STMT_LIST_NODE:
		  		run_stmt_list(ast);
		  		break;
		  case VAR_LIST_NODE:
		  		run_var_list(ast);
		  		break;
		  case FUNC_BODY_NODE:
		  		run_func_body(ast);
		  		break;
		  case PARAM_NODE:
		  		run_param_list(ast);
		  		break;		  
		  case WRITE_NODE:
            run_write(ast);
            break;
        case WHILE_NODE:
	        	run_while(ast);
	        	break;
        case FUNC_DECL_LIST_NODE:
        		run_func_list(ast);
        		break;
        case FUNC_DECL_NODE:
	         run_func_decl(ast);
				break;
        case RETURN_NODE:
        		run_return(ast);
        		break;
        case FUNC_HEADER_NODE:
            run_func_header(ast);
            break;
        case PLUS_NODE:
            run_plus(ast);
            break;
        case MINUS_NODE:
            run_minus(ast);
            break;
        case TIMES_NODE:
            run_times(ast);
            break;
        case OVER_NODE:
            run_over(ast);
            break;
        case LT_NODE:
            run_lt(ast);
            break;
        case EQ_NODE:
            run_eq(ast);
            break;
        case NUM_NODE:
            run_num(ast);
            break;
        case IF_NODE:
            run_if(ast);
            break;
            
		default:
            fprintf(stderr, "Invalid kind: %s!\n", node2str(get_kind(ast)));
            exit(1);
	}
}

void run_ast(AST *ast) {
    init_stack();
    init_mem();
    rec_run_ast(ast);
}
// ----------------------------------------------------------------------------