%{

// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <string>
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

%token TK_IDENTIFIER     

%token LIT_INT           
%token LIT_CHAR         
%token LIT_STRING        

%token TOKEN_ERROR      

// Precedência e associatividade
%left '~' '|' '&'
%left '<' '>' '='
%left '[' ']'
%left '(' ')'
%left '+' '-'
%left '*' '/'

%nonassoc KW_THEN
%nonassoc KW_ELSE

%%

// Início do programa
start: decl start
  | func start
  | 
  ;

// Declarações
decl: tipo declrest ';'
  ;

// Atribuição e inicialização de vetor para declaração
declrest: vetordecl
  | TK_IDENTIFIER '=' valor
  ;

// Declaração de vetor
vetordecl: TK_IDENTIFIER '[' LIT_INT ']' vetorrest
  ;

// Declaração do resto do vetor
vetorrest: '=' valorvetor
  |
  ;

// Valores do vetor
valorvetor: valor valorvetor
  | 
  ;

// Atribuição e inicialização de vetor
atribuicao: vetor
  | TK_IDENTIFIER '=' expr
  ;

// Declaração de vetor
vetor: TK_IDENTIFIER '[' expr ']'  '=' expr
  ;

// Tipo de variável
tipo: KW_INT
  | KW_CHAR
  ;

// Valor de variável
valor: LIT_INT
  | LIT_CHAR
  ;

// Chamada de função
funcreturn: TK_IDENTIFIER '(' lparam ')' 
  ;

// Parâmetros da função
lparam: lparamini
  | 
  ;

// Parâmetros iniciais da função
lparamini: expr lparamrest
  ;

// Lista de parâmetros
lparamrest: ',' lparamini
  | 
  ;

// Definição de função
func: tipo TK_IDENTIFIER '(' defbodyfunc ')' blockfunc
  ;

// Corpo da função
defbodyfunc: tipo TK_IDENTIFIER defbodyfuncrest
  | 
  ;

// Corpo da função com parâmetros adicionais
defbodyfuncrest: ',' tipo TK_IDENTIFIER defbodyfunc
  | 
  ;

// Comando if-else
comando_if: KW_IF '(' expr ')' KW_THEN lcmd KW_ELSE lcmd
  | KW_IF '(' expr ')' KW_THEN lcmd
  ;

// Comando while
comando_while: KW_WHILE '(' expr ')' lcmd
  ;

// Bloco de código 
blockfunc: '{' lcmd '}'
  ;

// Comandos iniciais
lcmdinit: KW_PRINT lprint ';'
  | KW_RETURN expr ';'
  | KW_READ TK_IDENTIFIER ';'
  | atribuicao ';'
  | ';'                      // Comando vazio permitido
  ;

// Lista de comandos (recursão à direita)
lcmd: lcmdinit lcmd                // Comandos iniciais em sequência
  | comando_if                     // Comando if seguido de mais comandos
  | comando_while                  // Comando while seguido de mais comandos
  | blockfunc lcmd                 // Bloco de código seguido de mais comandos
  |                                // Permite lcmd vazio (comando vazio)
  ;

// Impressão
lprint: LIT_STRING printrest
  | expr printrest
  ;

// Lista de impressão
printrest: lprint
  | 
  ;

// Expressões
expr: LIT_INT
  | LIT_CHAR
  | TK_IDENTIFIER
  | funcreturn
  | TK_IDENTIFIER '[' expr ']'
  | expr '+' expr
  | expr '-' expr
  | expr '*' expr
  | expr '/' expr
  | expr '<' expr
  | expr '>' expr
  | expr '&' expr
  | expr '|' expr
  | expr '~' expr
  | expr '=' expr
  | '(' expr ')'
  ;

%% 

int yyerror(string msg) {
  cerr << "Erro de sintaxe na linha " << getLineNumber() << "\n";
  exit(3);
}