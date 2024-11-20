// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <string>

using namespace std;

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_CARAC 2
#define SYMBOL_LIT_TEXT 3
#define SYMBOL_IDENTIFIER 4

struct Symbol {
    int type;
    string text;
};

Symbol* insertSymbol(int type, const char* text);
void printSymbols();
