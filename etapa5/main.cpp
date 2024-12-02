// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <fstream>
#include "semantic.h"

using namespace std;

extern ifstream yyin;
ofstream outputfile;
int yyparse();
int yylex();
int getLineNumber();
void initMe();

int main(int argc, char **argv) {
    initMe();
    if (argc < 3) {
        cerr << "Erro: Arquivo não especificado. Uso: ./programa nome_do_arquivo nome_do_arquivp_saida" << endl;
        exit(1);
    }

    yyin.open(argv[1]);
    if (!yyin.is_open()) {
        cerr << "Falha ao abrir o arquivo: " << argv[1] << endl;
        exit(2);  
    }

    outputfile.open(argv[2]);
    if (!outputfile.is_open())
    {
        cerr << "Falha ao abrir o arquivo de saida: " << argv[2] << endl;
        exit(2);
    }

    yyparse();

    yyin.close();
    outputfile.close();

    if(semanticError > 0){
        cerr << "O programa possui "<< getSemanticErrorsNumber() << " erros semânticos.\n";
        exit(4);
    }

    cout << "Total de linhas lidas: " << getLineNumber() << endl;
    cout << "Programa compilou com sucesso" << endl;

    exit(0);
}
