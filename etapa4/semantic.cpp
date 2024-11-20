// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include "semantic.h"
#include <string>

using namespace std;

int semanticError = 0;
// Mapa global para armazenar os símbolos
map<string, FunctionData> tabelaFuncoes;
FunctionData* lastFunction;

int getSemanticErrorsNumber(){
    return semanticError;
}

void checkAndSetDeclarations(AST* node){

  if(!node) return;
  //process this node

  if(node->type == AST_VAR_DECL){

    if(node->symbol->type != SYMBOL_IDENTIFIER){
      cerr << "Erro semântico na linha " << node->line << " [VAR_DECL]: identificador " << node->symbol->text << " já foi declarado. \n";
      semanticError++;
    }
    else{
      node->symbol->type = SYMBOL_VAR;
      if(node->son[0]->type == AST_KW_CHAR) node->symbol->datatype = DATATYPE_CHAR;
      if(node->son[0]->type == AST_KW_INT) node->symbol->datatype = DATATYPE_INT;
    }
  }

  if(node->type == AST_VECTOR_DECL || node->type == AST_VECTOR_DECL_EMPTY){
    if(node->symbol->type != SYMBOL_IDENTIFIER){
      cerr << "Erro semântico na linha " << node->line << " [VECTOR_DECL]: identificador " << node->symbol->text << " já foi declarado. \n";
      semanticError++;
    }
    else{
      checkVectorIndex(node);
      node->symbol->type = SYMBOL_VEC;
      if(node->son[0]->type == AST_KW_CHAR) node->symbol->datatype = DATATYPE_CHAR;
      if(node->son[0]->type == AST_KW_INT) node->symbol->datatype = DATATYPE_INT;
    }
  }

  if(node->type == AST_FUN_DECL){
    if(node->symbol->type != SYMBOL_IDENTIFIER && node->symbol->type != SYMBOL_VAR){
      cerr << "Erro semântico na linha " << node->line << " [FUN_DECL]: identificador " << node->symbol->text << " já foi declarado. \n";
      semanticError++;
    }
    else{
      node->symbol->type = SYMBOL_FUNC;
      lastFunction = addFunction(node->symbol);
      if(node->son[0]->type == AST_KW_CHAR) node->symbol->datatype = DATATYPE_CHAR;
      if(node->son[0]->type == AST_KW_INT) node->symbol->datatype = DATATYPE_INT;
    }
  }

  //Para declaração dos argumentos das funções como variaveis válidas
  if(node->type == AST_FUN_PARAML || node->type == AST_FUN_PARAMREST){

    if(node->son[0]->symbol->type != SYMBOL_IDENTIFIER){
      cerr << "Erro semântico na linha " << node->line << " [PARAM]: identificador " << node->son[0]->symbol->text << " já foi declarado. \n";
      semanticError++;
    }
    else{
      node->son[0]->symbol->type = SYMBOL_VAR;
      if(node->son[0]->son[0]->type == AST_KW_CHAR) node->son[0]->symbol->datatype = DATATYPE_CHAR;
      if(node->son[0]->son[0]->type == AST_KW_INT) node->son[0]->symbol->datatype = DATATYPE_INT;
        
        lastFunction->paramType[lastFunction->numParam] = node->son[0]->symbol->datatype;
        lastFunction->numParam ++;
    }
  }

  int i;
  for(i=0; i<MAX_SONS; i++){
    checkAndSetDeclarations(node->son[i]);
  }
}

int checkNodeNumType(AST *node){
  if(!node) return -1;

  //literas, variables, access to array and function can be float or integer
  else if(node->type == AST_SYMBOL || node->type == AST_ARRAY_POS || node->type == AST_FUNCALL) {
    if(node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR || node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CARAC) {
      return 1;
    }
    return -1;
  }
  //logical operators dont return real
  else if(node->type == AST_LESS || node->type == AST_GREATER || node->type == AST_NEG
    || node->type == AST_EQ || node->type == AST_AND || node->type == AST_OR)
    return 0;

  //arithmetical operations require more checks
  else if(node->type == AST_ADD || node->type == AST_SUB || node->type == AST_MUL || node->type == AST_DIV){
    if(checkNodeNumType(node->son[1]) == -1 || checkNodeNumType(node->son[0]) == -1 ){
      return -1;
    }
    else if(checkNodeNumType(node->son[1]) == 0 || checkNodeNumType(node->son[0]) == 0 ){
      return 0;
    }
    return 1;
  }

  //expressions between parentesis must be checked
  else if(node->type == AST_EXP_P)
    return checkNodeNumType(node->son[0]);
  
  return -1;
}

void checkDeclarationUsage(AST* node){

  //If node is NULL return
  if(!node) return;

  	//check if variables calls are calling variables
    if(node->type == AST_VAR_ATRIB) {
      if(node->symbol->type != SYMBOL_VAR ){
        cerr << "Erro semântico na linha " << node->line << " [VAR_ATRIB]: identificador " << node->symbol->text << " precisa ser uma variável.\n";
        semanticError++;
      }
      //check if the variable and expression are the same type
      if(node->symbol->datatype == DATATYPE_CHAR || node->symbol->datatype == DATATYPE_INT ){
        if(checkNodeNumType(node->son[0]) != 1){
          cerr << "Erro semântico na linha "<< node->line << " [VAR_ATRIB]: os tipos de dados de variável e expressão não correspondem.\n";
          semanticError++;
        }
      }

    }

    //check if vectors calls are calling vectors
    if(node->type == AST_VECTOR_ATRIB) {
      if(node->symbol->type != SYMBOL_VEC){
        cerr << "Erro semântico na linha " << node->line << " [VECTOR_ATRIB]: identificador "<< node->symbol->text << " precisa ser um vetor.\n";
        semanticError++;
      }

      checkVectorIndex(node);

      //check if the vector and expression are the same type
      if(node->symbol->datatype == DATATYPE_CHAR || node->symbol->datatype == DATATYPE_INT ){
        if(checkNodeNumType(node->son[1]) != 1){
          cerr << "Erro semântico na linha " << node->line << " [VECTOR_ATRIB]: os tipos de dados de variável e expressão não correspondem.\n";
          semanticError++;
        }
      }
    }

    //check if vectors calls are calling vectors and if it's index is valid
    if(node->type == AST_ARRAY_POS){
      if(node->symbol->type != SYMBOL_VEC){
        cerr << "Erro semântico na linha " << node->line << " [ARRAY_POS]: identificador "<< node->symbol->text <<" precisa ser um vetor.\n";
        semanticError++;
      }
      else checkVectorIndex(node);
    }

    //check if functions calls are calling functions and if it's arguments are valid
    if(node->type == AST_FUNCALL){
      if(node->symbol->type != SYMBOL_FUNC){
        cerr << "Erro semântico na linha " << node->line << " [FUNCALL]: identificador " << node->symbol->text << " precisa ser uma função.\n";
        semanticError++;
      }
      else checkFuncParam(node->son[0], node->symbol->text.c_str(), node->line);
    }

    //check if variables declarations and literal are the same type
    if(node->type == AST_VAR_DECL){
      if(node->symbol->datatype == DATATYPE_CHAR || node->symbol->datatype == DATATYPE_INT ){
        if(checkNodeNumType(node->son[1]) != 1){
          cerr << "Erro semântico na linha " << node->line << " [VAR_DECL]:  os tipos de dados de variável e expressão não correspondem.\n";
          semanticError++;
        }
      }
    }

    //check if vectors declarations and literals are the same type
    if(node->type == AST_VECTOR_DECL){
      checkVectorIndex(node);
      AST* current_node;

      current_node = node->son[2];
      int pos = 0;

      while(current_node != NULL){
        if(node->symbol->datatype == DATATYPE_CHAR || node->symbol->datatype == DATATYPE_INT ){
          if(checkNodeNumType(current_node) != 1){
            cerr << "Erro semântico na linha " << node->line << " [VECTOR_DECL]:  os tipos de dados de vetor e literal_" << pos << " não correspondem.\n";
            semanticError++;
          }
        }
        current_node = current_node->son[0];
        pos++;
      }

      int num = stoi(node->son[1]->symbol->text.substr(1));

      if(pos == 0 || pos != num){
        cerr << "Erro semântico na linha " << node->line << " [VECTOR_DECL]: o número de elementos do vetor não corresponde ao índice\n";
        semanticError++;
      }
    }

    if(node->type == AST_VECTOR_DECL_EMPTY){
      checkVectorIndex(node);
    }

    int i;
    for(i=0; i<MAX_SONS; i++){
      checkDeclarationUsage(node->son[i]);
    }
}

void checkVectorIndex(AST* node) {
  //IF SON[0] IS TO CHECK THE AST_ARRAY_POS INDEX. IF SON[1] IS TO CHECK THE VECTOR_DECL INDEX.
  if (node->type == AST_ARRAY_POS || node->type == AST_VECTOR_ATRIB){
    if(checkNodeNumType(node->son[0]) != 1){
      cerr << "Erro semântico na linha " << node->line << ": índice do vetor não é um número inteiro.\n";
      semanticError++;
    }
  }
  else if (node->type == AST_VECTOR_DECL || node->type == AST_VECTOR_DECL_EMPTY){
    if(checkNodeNumType(node->son[1]) != 1){
      cerr << "Erro semântico na linha " << node->line << ": índice do vetor não é um número inteiro.\n";
      semanticError++;
    }
  }
}

void checkReturnType(AST* node) {
  if(!node) return;

  if(node->type == AST_KW_RETURN) {
      //check the type of the expression that is being returned
      //in this case, the defined types are interchangeable
      if(checkNodeNumType(node->son[0]) != 1) {
        cerr << "Erro semântico na linha " << node->line << ": valor de retorno inválido.\n";
        semanticError++;
        return;
      }
  }

  for(int i=0; i<MAX_SONS; i++) { 
    checkReturnType(node->son[i]);
  }                               
}


void checkFuncParam(AST* node, const char* function_name, int line){
	AST* current_node;
	current_node = node;
	int param_count = 0;

	FunctionData* func_data;
	func_data = findFunction(function_name);
  if(func_data == NULL) return;

	int numParam = func_data->numParam;

	while(current_node!=NULL && param_count<=numParam){
      //verify if the parameter is type integer
      if(func_data->paramType[param_count] == DATATYPE_INT || func_data->paramType[param_count] == DATATYPE_CHAR)
        if(checkNodeNumType(current_node->son[0]) != 1 ){
          cerr << "Erro semântico na linha " << node->line << ": tipo de parâmetro da função inválido. (parâmetro " << param_count+1 << ")\n";
          semanticError++;
        }

			param_count++;
			current_node = current_node->son[1];
	}

	if(param_count > numParam){
    cerr << "Erro semântico na linha " << node->line << ": muitos argumentos na função " << function_name << ".\n";
  	semanticError++;
	}
	if(param_count < numParam){
    cerr << "Erro semântico na linha " << node->line << ": argumentos faltando na função " << function_name << ".\n";
  	semanticError++;
	}
}

FunctionData* addFunction(Symbol* function){
	FunctionData newNode;
	newNode.function = function;
	newNode.numParam = 0;

  tabelaFuncoes[function->text] = newNode;
  return &tabelaFuncoes[function->text];
}

FunctionData* findFunction(const char* function_name){
  for(const auto& i: tabelaFuncoes) {
    if(i.first == function_name) {
      return &tabelaFuncoes[function_name];
    }
  }

  return NULL;
}
