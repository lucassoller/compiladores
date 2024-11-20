// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <fstream>

using namespace std;

extern ifstream yyin;

int yyparse();
int yylex();
int getLineNumber();
void initMe();
void printSymbols();

int main(int argc, char **argv) {
    initMe();
    if (argc < 2) {
        cerr << "Erro: Arquivo não especificado. Uso: ./programa nome_do_arquivo" << endl;
        exit(1);  
    }

    yyin.open(argv[1]);
    if (!yyin.is_open()) {
        cerr << "Falha ao abrir o arquivo: " << argv[1] << endl;
        exit(2);  
    }

    yyparse();

    // Mostrar a tabela de símbolos ao final
    printSymbols();

    cout << "Total de linhas lidas: " << getLineNumber() << endl;

    yyin.close();

    exit(0); // 0 is CODE success       
}
