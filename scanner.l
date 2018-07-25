/* Definição para L */
L		[a-zA-Z_]

%option outfile="scanner.c"
%option noyywrap
%option nounput
%option noinput
%option yylineno
%{
#include "ast.h"
#include "parser.h"
#include "tables.h"

int line_num = 1;

%}

ignore      [ \t\n]+
/* Os comentários de várias linhas são tratados na seção de regras.
 * Primeiro precisamos definir uma condição inicial. 
 */
%x comment

%%

"/*"         			{BEGIN(comment);}
<comment>[^*\n]*        
<comment>"*"+[^*/\n]*   
<comment>\n             {++line_num;}
<comment>"*"+"/"        {BEGIN(INITIAL);}

{ignore}                {} /* Eat up whitespace. */
"//".*					{}

"}"						{return RBRACE ;}
"{"						{return LBRACE ;}
"]"						{return RBRACK ;}
"["						{return LBRACK ;}
")"						{return RPAREN ;}
"("						{return LPAREN ;}
,						{return COMMA  ;}
;						{return SEMI   ;}
"="						{return ASSIGN ;}
"!="					{return NEQ    ;}
"=="					{return EQ     ;}
">="					{return GE     ;}
">"						{return GT     ;}
"<="					{return LE     ;}
"<"						{return LT     ;}
"*"						{return TIMES  ;}
"-"						{return MINUS  ;}
"+"						{return PLUS   ;}
"/"						{return OVER   ;}
write					{return WRITE  ;}
while					{return WHILE  ;}
void					{return VOID   ;}
return					{return RETURN ;}
output					{return OUTPUT ;}
int 					{return INT    ;}
input					{return INPUT  ;}
if	    				{return IF     ;}
else					{return ELSE   ;}
[A-Za-z][A-Za-z0-9]*    { yylval = make_node(IDENTIFICADOR, -1, yytext); return ID; }
[0-9]+                  { yylval = make_node(NUM_NODE, -1, yytext); return NUM; }
L?\"(\\.|[^\\"])*\"     { yylval = make_node(STRING_NODE, -1, yytext); return STRING; }
'\n'                	{yylineno++;}	   
.					    {printf("SCANNING ERROR (%d): Unknown symbol %s \n",yylineno,yytext); exit(1);}

