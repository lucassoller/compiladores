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
start: decl 

decl: dec decl                                                
  |                                                           
  ;

dec: vardec                                                   
  | fundec                                                    
  ;

// Declarações
vardec: tipo TK_IDENTIFIER '=' valor ';'                      
  | tipo TK_IDENTIFIER '[' valorvec ']' '=' valorvetor ';'    
  | tipo TK_IDENTIFIER '[' valorvec ']' ';'                   
  ;

// Valores do vetor
valorvetor: LIT_INT valorvetor                                
  | LIT_CHAR valorvetor                                       
  |                                                          
  ;


// Tipo de variável
tipo: KW_INT                                                 
  | KW_CHAR                                                   
  ;

// Valor de variável
valor: LIT_INT                                                
  | LIT_CHAR                                                  
  ;

// Valor de variável
valorvec: LIT_INT                                             
  ;

// Definição de função
fundec: tipo TK_IDENTIFIER '(' funlparam ')' block            
  ;

  
funlparam: paramdecl paramrest			                          
	|			                                                     
	;

paramdecl: tipo TK_IDENTIFIER			                            
	;

paramrest: ',' paramdecl paramrest			                      
	|			                                                     
	;

// Bloco de código 
block: '{' lcmd '}'                                          
  ;

lcmd: cmd lcmd                                               
  | cmd                                                      
  ;

// Lista de comandos (recursão à direita)
cmd: TK_IDENTIFIER '=' expr ';'                               
  | TK_IDENTIFIER '[' expr ']' '=' expr ';'                   
  | KW_PRINT lprint ';'                                       
  | KW_RETURN expr ';'                                        
  | KW_READ TK_IDENTIFIER ';'                                 
  | KW_IF '(' expr ')' KW_THEN cmd KW_ELSE cmd                
  | KW_IF '(' expr ')' KW_THEN cmd                            
  | KW_WHILE '(' expr ')' cmd                                 
  | block                                                     
  | ';'                                                       
  |                                                          
  ;

// Impressão
lprint: printelement printrest                                
  ;

// Lista de impressão
printrest: printelement printrest                             
  |                                                          
  ;

printelement: expr                                           
  | LIT_STRING                                                
  ;

// Parâmetros da função
lparam: expr lparamrest                                       
  |                                                          
  ;

// Lista de parâmetros
lparamrest: ',' expr lparamrest                               
  |                                                          
  ;

// Expressões 
expr: TK_IDENTIFIER                                           
  | LIT_CHAR                                                  
  | LIT_INT                                                   
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
  | TK_IDENTIFIER '(' lparam ')'                                          
  | TK_IDENTIFIER '[' expr ']'                                                           
  ;

%% 

int yyerror(string msg) {
  cerr << "Erro de sintaxe na linha " << getLineNumber() << "\n";
  exit(3);
}