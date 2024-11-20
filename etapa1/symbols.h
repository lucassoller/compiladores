// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <string>

using namespace std;

struct Symbol {
    int type;
    string text;
};

Symbol* insertSymbol(int type, const char* text);
void printSymbols();
