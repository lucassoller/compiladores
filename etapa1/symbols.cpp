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
        
        // Insere no map
        tabelaSimbolos[chave] = simbolo;
        cout << "Símbolo inserido: " << chave << endl;
        
        // Retorna um ponteiro para o símbolo recém-inserido
        return &tabelaSimbolos[chave];
    } else {
        cout << "Símbolo já existe: " << chave << endl;
        return nullptr; // Retorna nullptr se o símbolo já existir
    }
}

// Função que imprime todos os símbolos
void printSymbols() {
    cout << "Tabela de símbolos:" << endl;
    for (const auto& par : tabelaSimbolos) {
        cout << "Chave: " << par.first << ", Tipo: " << par.second.type 
             << endl;
    }
}
