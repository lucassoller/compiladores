%{

// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>

#include "ast.h"
#include "semantic.h"
using namespace std;

int yylex();
int yyerror(string msg);
int getLineNumber();

%}

// Tokens
%token KW_CHAR         
%token KW_INT            
%token KW_IF             
%token KW_THEN           
%token KW_ELSE           
%token KW_WHILE         
%token KW_READ           
%token KW_PRINT          
%token KW_RETURN        

%token<symbol> TK_IDENTIFIER     
%token<symbol> LIT_INT           
%token<symbol> LIT_CHAR         
%token<symbol> LIT_STRING        

%token TOKEN_ERROR   

%type<ast> start
%type<ast> decl
%type<ast> dec
%type<ast> vardec
%type<ast> tipo
%type<ast> valor
%type<ast> valorvetor
%type<ast> valorvec
%type<ast> fundec
%type<ast> funlparam
%type<ast> paramdecl
%type<ast> paramrest
%type<ast> block
%type<ast> lcmd
%type<ast> cmd
%type<ast> lprint
%type<ast> printrest
%type<ast> printelement
%type<ast> expr
%type<ast> lparam
%type<ast> lparamrest

// Precedência e associatividade
%left '~' '|' '&'
%left '<' '>' '='
%left '[' ']'
%left '(' ')'
%left '+' '-'
%left '*' '/'

%nonassoc KW_THEN
%nonassoc KW_ELSE

%union {
  AST* ast;
  Symbol* symbol;
}

%%

// Início do programa
start: decl                                                   { //astPrint($1, 0); 
                                                                astGenerateFile($1, outputfile);
                                                                checkAndSetDeclarations($1); 
                                                                checkUndeclarations();
                                                                checkDeclarationUsage($1);
                                                                }

decl: dec decl                                                { $$ = astCreate(AST_DEC,0,$1,$2,0,0); }
  |                                                           { $$ = 0; }
  ;

dec: vardec                                                   { $$ = $1; }
  | fundec                                                    { $$ = $1; }
  ;

// Declarações
vardec: tipo TK_IDENTIFIER '=' valor ';'                      { $$ = astCreate(AST_VAR_DECL,$2,$1,$4,0,0); }
  | tipo TK_IDENTIFIER '[' valorvec ']' '=' valorvetor ';'    { $$ = astCreate(AST_VECTOR_DECL,$2,$1,$4,$7,0); }
  | tipo TK_IDENTIFIER '[' valorvec ']' ';'                   { $$ = astCreate(AST_VECTOR_DECL_EMPTY,$2,$1,$4,0,0); }
  ;

// Valores do vetor
valorvetor: LIT_INT valorvetor                                { $$ = astCreate(AST_SYMBOL,$1,$2,0,0,0); }
  | LIT_CHAR valorvetor                                       { $$ = astCreate(AST_SYMBOL,$1,$2,0,0,0); }
  |                                                           { $$ = 0; }
  ;


// Tipo de variável
tipo: KW_INT                                                  { $$ = astCreate(AST_KW_INT,0,0,0,0,0); }
  | KW_CHAR                                                   { $$ = astCreate(AST_KW_CHAR,0,0,0,0,0); }
  ;

// Valor de variável
valor: LIT_INT                                                { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
  | LIT_CHAR                                                  { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
  ;

// Valor de variável
valorvec: LIT_INT                                             { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
  ;

// Definição de função
fundec: tipo TK_IDENTIFIER '(' funlparam ')' block            { $$ = astCreate(AST_FUN_DECL,$2,$1,$4,$6,0); }
  ;

  
funlparam: paramdecl paramrest			                          { $$ = astCreate(AST_FUN_PARAML,0,$1,$2,0,0); }
	|			                                                      { $$ = 0; }
	;

paramdecl: tipo TK_IDENTIFIER			                            { $$ = astCreate(AST_SYMBOL,$2,$1,0,0,0); }
	;

paramrest: ',' paramdecl paramrest			                      { $$ = astCreate(AST_FUN_PARAMREST,0,$2,$3,0,0); }
	|			                                                      { $$ = 0; }
	;

// Bloco de código 
block: '{' lcmd '}'                                           { $$ = astCreate(AST_BLOCK,0,$2,0,0,0); }
  ;

lcmd: cmd ';' lcmd                                            { $$ = astCreate(AST_LCMD,0,$1,$3,0,0); }
  | cmd                                                       { $$ = $1; }
  ;



// Lista de comandos (recursão à direita)
cmd: TK_IDENTIFIER '=' expr                                   { $$ = astCreate(AST_VAR_ATRIB,$1,$3,0,0,0); }
  | TK_IDENTIFIER '[' expr ']' '=' expr                       { $$ = astCreate(AST_VECTOR_ATRIB,$1,$3,$6,0,0); }
  | KW_PRINT lprint                                           { $$ = astCreate(AST_KW_PRINT,0,$2,0,0,0); }
  | KW_RETURN expr                                            { $$ = astCreate(AST_KW_RETURN,0,$2,0,0,0); }
  | KW_READ TK_IDENTIFIER                                     { $$ = astCreate(AST_KW_READ,$2,0,0,0,0); }
  | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd                { $$ = astCreate(AST_KW_IF,0,$3,$6,$8,0); }
  | KW_IF '(' expr ')' KW_THEN cmd                            { $$ = astCreate(AST_KW_IF,0,$3,$6,0,0); }
  | KW_WHILE '(' expr ')' cmd                                 { $$ = astCreate(AST_KW_WHILE,0,$3,$5,0,0); }
  | block cmd                                                 { $$ = astCreate(AST_NEW_BLOCK,0,$1,$2,0,0); }
  |                                                           { $$ = 0; }
  ;

// Impressão
lprint: printelement printrest                                { $$ = astCreate(AST_PRINT_ARG,0,$1,$2,0,0); }
  ;

// Lista de impressão
printrest: printelement printrest                             { $$ = astCreate(AST_PRINT_ARG,0,$1,$2,0,0); }
  |                                                           { $$ = 0; }
  ;

printelement: expr                                            { $$ = $1; }
  | LIT_STRING                                                { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
  ;

// Parâmetros da função
lparam: expr lparamrest                                       { $$ = astCreate(AST_PARAML,0,$1,$2,0,0); }
  |                                                           { $$ = 0; }
  ;

// Lista de parâmetros
lparamrest: ',' expr lparamrest                               { $$ = astCreate(AST_PARAML,0,$2,$3,0,0); }
  |                                                           { $$ = 0; }
  ;

// Expressões 
expr: TK_IDENTIFIER                                           { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }        
  | LIT_CHAR                                                  { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); } 
  | LIT_INT                                                   { $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
  | expr '+' expr                                             { $$ = astCreate(AST_ADD,0,$1,$3,0,0); }   
  | expr '-' expr                                             { $$ = astCreate(AST_SUB,0,$1,$3,0,0); } 
  | expr '*' expr                                             { $$ = astCreate(AST_MUL,0,$1,$3,0,0); }
  | expr '/' expr                                             { $$ = astCreate(AST_DIV,0,$1,$3,0,0); } 
  | expr '<' expr                                             { $$ = astCreate(AST_LESS,0,$1,$3,0,0); } 
  | expr '>' expr                                             { $$ = astCreate(AST_GREATER,0,$1,$3,0,0); } 
  | expr '&' expr                                             { $$ = astCreate(AST_AND,0,$1,$3,0,0); } 
  | expr '|' expr                                             { $$ = astCreate(AST_OR,0,$1,$3,0,0); }
  | expr '~' expr                                             { $$ = astCreate(AST_NEG,0,$1,$3,0,0); } 
  | expr '=' expr                                             { $$ = astCreate(AST_EQ,0,$1,$3,0,0); } 
  | '(' expr ')'                                              { $$ = astCreate(AST_EXP_P,0,$2,0,0,0); }
  | TK_IDENTIFIER '(' lparam ')'                              { $$ = astCreate(AST_FUNCALL,$1,$3,0,0,0); }               
  | TK_IDENTIFIER '[' expr ']'                                { $$ = astCreate(AST_ARRAY_POS,$1,$3,0,0,0); }                                
  ;

%% 

int yyerror(string msg) {
  cerr << "Erro de sintaxe na linha " << getLineNumber() << "\n";
  exit(3);
}