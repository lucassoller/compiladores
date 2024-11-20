%{
// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <fstream>
#include <string>
#include "symbols.h"
#include "tokens.h"

int running = 1;
int lineNumber = 1;
ifstream yyin;
FlexLexer* scanner;

%}

%option c++

%x COMMENT
%%

[ \t\r]+                   /* Ignorar espaços em branco */
\n                         { lineNumber++; }  /* Contar a nova linha */

"char"                     {return KW_CHAR;} 
"int"                      {return KW_INT;}  
"if"                       {return KW_IF;}   
"then"                     {return KW_THEN;} 
"else"                     {return KW_ELSE;} 
"while"                    {return KW_WHILE;}
"read"                     {return KW_READ;} 
"print"                    {return KW_PRINT;}
"return"                   {return KW_RETURN;}

[-,;:()\[\]{}=+*/%<>&|~]   {return YYText()[0];}
#[0-9]+                    { insertSymbol(LIT_INT, yytext); return LIT_INT;}
[a-zA-Z0-9]*               { insertSymbol(TK_IDENTIFIER, yytext); return TK_IDENTIFIER;}
\'([^\\\']|\\.)\'          { insertSymbol(LIT_CHAR, yytext); return LIT_CHAR;}
\"([^\\\"]|\\.)*\"         { insertSymbol(LIT_STRING, yytext); return LIT_STRING;}

"//".*                     /* Ignorar comentários de linha */
"/**"                      {BEGIN (COMMENT);}
.                          {return TOKEN_ERROR;}         
<COMMENT>"**/"             {BEGIN (INITIAL);} 
<COMMENT>"\n"              {lineNumber++;}    /* Contar novas linhas dentro de comentários */
<COMMENT>.                 /* Ignorar caracteres em comentários */

%%

int yyFlexLexer::yywrap() {
   running = 0;
   return 1;
}

int getLineNumber(void){
   return lineNumber;
}

void initMe(){

}

int yylex(){
   if(!scanner)
      scanner = new yyFlexLexer(&yyin, &cout);
   return scanner->yylex();
}

int isRunning(){
   return running;
}