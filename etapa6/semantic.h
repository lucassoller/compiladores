// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include <stdio.h>
#include <string.h>
#include <map>
#include "symbols.h"
#include "ast.h"

#define MAX_NUM_PARAM 10

struct FunctionData{
	int numParam;
	int paramType[MAX_NUM_PARAM];
	Symbol* function;
};

// Mapa global para armazenar os símbolos
map<string, FunctionData> extern tabelaFuncoes;
extern FunctionData* lastFunction;

//Variable which will increment at each semantic error.
extern int semanticError;

//Return the number of semantic errors.
int getSemanticErrorsNumber();

//Check all declarations in program.
void checkAndSetDeclarations(AST* node);

//Check if all index of an vector are Integers.
void checkVectorIndex(AST* node);

//Check if the node is an Integer or a Real.
//@return 1 if node is type REAL, 0 if is BOOLEAN, -1 otherwise.
int checkNodeNumType(AST *node);

//Checks if all declarations are in correct use.
//Guarantee if the type is the same of the expression.
void checkDeclarationUsage(AST* node);

//Check if all index of an vector are Integers.
void checkVectorIndex(AST* node);

//Check all the functions parameters
void checkFuncParam(AST* node, const char* function_name, int line);

//Add all functions.
FunctionData* addFunction(Symbol* function);

//Find the function by name.
FunctionData* findFunction(const char* function_name);