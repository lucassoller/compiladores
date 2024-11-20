// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <fstream>
#include "tokens.h"

using namespace std;

extern ifstream yyin;
extern char text;

int yylex();
int getLineNumber();
int isRunning();
void initMe();
void printSymbols();

int main(int argc, char **argv) {
    int tok;
    initMe();
    if (argc < 2) {
        cerr << "Erro: Arquivo não especificado. Uso: ./programa nome_do_arquivo" << endl;
        return -1;  
    }

    yyin.open(argv[1]);
    if (!yyin.is_open()) {
        cerr << "Falha ao abrir o arquivo: " << argv[1] << endl;
        return -1;  
    }

    while (isRunning()) {
        tok = yylex();
        switch(tok){
        case KW_CHAR: cerr << "CHAR\n";
            break;
        case KW_INT: cerr << "INT\n";
            break;
        case KW_IF: cerr << "IF\n";
            break;
        case KW_THEN: cerr << "THEN\n";
            break;
        case KW_ELSE: cerr << "ELSE\n";
            break;
        case KW_WHILE: cerr << "WHILE\n";
            break;
        case KW_READ: cerr << "READ\n";
            break;
        case KW_RETURN: cerr << "RETURN\n";
            break;
        case KW_PRINT: cerr << "PRINT\n";
            break;
        case TK_IDENTIFIER:
            printf("IDENTIFIER\n");
            break;
        case LIT_INT:
            printf("INT\n");
            break;
        case LIT_CHAR:
            printf("CHAR\n");
            break;
        case LIT_STRING:
            printf("STRING\n");
            break;
        case TOKEN_ERROR:
            printf("ERRO in line %d\n",getLineNumber());
            break;
        case 0:
            break;
        default:
            printf("%c\n",tok);
            break;
      }
    }

    // Mostrar a tabela de símbolos ao final
    printSymbols();

    cout << "Total de linhas lidas: " << getLineNumber() << endl;

    yyin.close();

    return 0;        
}
