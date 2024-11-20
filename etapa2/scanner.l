%{
// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <fstream>
#include <string>
#include "symbols.h"
#include "y.tab.c"

int running = 1;
int lineNumber = 1;
ifstream yyin;
FlexLexer* scanner;

%}

%option c++

%x COMMENT
%%

"char"                     {return KW_CHAR;} 
"int"                      {return KW_INT;}  
"if"                       {return KW_IF;}   
"then"                     {return KW_THEN;} 
"else"                     {return KW_ELSE;} 
"while"                    {return KW_WHILE;}
"read"                     {return KW_READ;} 
"print"                    {return KW_PRINT;}
"return"                   {return KW_RETURN;}

#[0-9]+                    {insertSymbol(SYMBOL_LIT_INT, yytext); return LIT_INT; }
[a-zA-Z0-9]+               {insertSymbol(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }
\'([^\\\']|\\.)\'          {insertSymbol(SYMBOL_LIT_CARAC, yytext); return LIT_CHAR; }
\"([^\\\"]|\\.)*\"         {insertSymbol(SYMBOL_LIT_TEXT, yytext); return LIT_STRING; }
[-,;:()\[\]{}=+*/%<>&|~]   {return yytext[0]; }

"\n"                       { lineNumber++; }  /* Contar a nova linha */
[ \t\r]                    { } /* Ignorar espaços em branco */

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