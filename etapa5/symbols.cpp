// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <iostream>
#include <map>
#include <string>
#include "symbols.h"

// Usando apenas os elementos que precisamos do namespace std
using std::map;
using std::string;
using std::cout;
using std::endl;

// Mapa global para armazenar os símbolos
map<string, Symbol> tabelaSimbolos;

// Função que insere um símbolo, garantindo que não sejam duplicados
Symbol* insertSymbol(int type, const char* text) {
    string chave(text); // Converte char* para string

    // Verifica se o símbolo já existe
    if (tabelaSimbolos.find(chave) == tabelaSimbolos.end()) {
        // Se não existir, insere o símbolo
        Symbol simbolo;
        simbolo.type = type;
        simbolo.text = chave;
        simbolo.datatype = 0;
        simbolo.line = getLineNumber();
        
        // Insere no map
        tabelaSimbolos[chave] = simbolo;
    
    }
    return &tabelaSimbolos[chave];
}

// Função que imprime todos os símbolos
void printSymbols() {
    cout << "Tabela de símbolos:" << endl;
    for (const auto& par : tabelaSimbolos) {
        cout << "Chave: " << par.first << ", Tipo: " << par.second.type 
             << endl;
    }
}

void checkUndeclarations() {
    for(const auto& i: tabelaSimbolos) {
        if(i.second.type == SYMBOL_IDENTIFIER) {
            cerr << "Erro semântico na linha " << i.second.line << " [UNDECLARED]: identificador " << i.second.text << " não foi declarado. \n";
        }
    }
}

Symbol* makeTemp(void) {

	static int serialNumber = 0;
	static char buffer[64];

	sprintf(buffer,"_tMpComP986%d", serialNumber++);
	return insertSymbol(SYMBOL_SCALAR, buffer);
}


Symbol* makeLabel(void) {

	static int serialNumber = 0;
	static char buffer[64];

	sprintf(buffer, "_labelLCoMp8975%d", serialNumber++);
	return insertSymbol(SYMBOL_SCALAR, buffer);
}
