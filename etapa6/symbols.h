// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <string>

using namespace std;

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_CARAC 2
#define SYMBOL_LIT_TEXT 3
#define SYMBOL_IDENTIFIER 4

#define SYMBOL_VAR 5
#define SYMBOL_VEC 6
#define SYMBOL_FUNC 7

#define DATATYPE_CHAR 8
#define DATATYPE_INT 9

#define SYMBOL_SCALAR 10

struct Symbol {
    int type;
    int datatype;
	int line;
    string text;
};

Symbol* insertSymbol(int type, const char* text);
void printSymbols();
void checkUndeclarations();
int getLineNumber();
Symbol* makeLabel(void);
Symbol* makeTemp(void);

#endif