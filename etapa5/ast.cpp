// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include "ast.h"
#include <iostream>
#include <string.h>

using namespace std;

AST *astCreate(int type, Symbol *symbol, AST* s0, AST* s1, AST* s2, AST* s3)
{
    AST *newnode = (AST *) calloc(1, sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
	newnode->line = getLineNumber();
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;
    return newnode;
}

void astPrint(AST *node, int level)
{
    int i=0;
    if(node == 0)
        return ;
    for(i=0; i<level; i++)
        cout << "  ";
    cout << "AST (";
    switch(node->type)
    {
        case AST_DEC: cout << "AST_DEC"; break;
        case AST_VAR_DECL: cout << "AST_VAR_DECL"; break;
        case AST_VECTOR_DECL: cout << "AST_VECTOR_DECL"; break;
        case AST_VECTOR_DECL_EMPTY: cout << "AST_VECTOR_DECL_EMPTY"; break;
        case AST_LCMD: cout << "AST_LCMD"; break;
        case AST_SYMBOL: cout << "AST_SYMBOL"; break;
        case AST_FUN_DECL: cout << "AST_FUN_DECL"; break;
        case AST_FUN_PARAML: cout << "AST_FUN_PARAML"; break;
        case AST_FUN_PARAMREST: cout << "AST_FUN_PARAMREST"; break;
        case AST_BLOCK: cout << "AST_BLOCK"; break;
        case AST_VAR_ATRIB: cout << "AST_VAR_ATRIB"; break;
        case AST_VECTOR_ATRIB: cout << "AST_VECTOR_ATRIB"; break;
        case AST_ADD: cout << "AST_ADD"; break;
        case AST_SUB: cout << "AST_SUB"; break;
        case AST_MUL: cout << "AST_MUL"; break;
        case AST_DIV: cout << "AST_DIV"; break;
        case AST_LESS: cout << "AST_LESS"; break;
        case AST_GREATER: cout << "AST_GREATER"; break;
        case AST_NEG: cout << "AST_NEG"; break;
        case AST_EQ: cout << "AST_EQ"; break;
        case AST_AND: cout << "AST_AND"; break;
        case AST_OR: cout << "AST_OR"; break;
        case AST_ARRAY_POS: cout << "AST_ARRAY_POS"; break;
        case AST_FUNCALL: cout << "AST_FUNCALL"; break;
        case AST_KW_CHAR: cout << "AST_KW_CHAR"; break;
        case AST_KW_INT: cout << "AST_KW_INT"; break;
        case AST_KW_READ: cout << "AST_KW_READ"; break;
        case AST_KW_RETURN: cout << "AST_KW_RETURN"; break;
        case AST_KW_PRINT: cout << "AST_KW_PRINT"; break;
        case AST_KW_WHILE: cout << "AST_KW_WHILE"; break;
        case AST_KW_IF: cout << "AST_KW_IF"; break;
        case AST_NEW_BLOCK: cout << "AST_NEW_BLOCK"; break;
        case AST_PRINT_ARG: cout << "AST_PRINT_ARG"; break;
        case AST_PARAML: cout << "AST_PARAML"; break;
        case AST_EXP_P: cout << "AST_EXP_P"; break;
        case AST_LIT_CHAR: cout << "AST_LIT_CHAR"; break;
        case AST_LIT_INT: cout << "AST_LIT_INT"; break;
        default: cout << "AST_UNKNOWN"; break;
    }
    if(node->symbol != 0)
        cout << "," << node->symbol->text << "\n";
    else
        cout << ",\n";
    for(i=0; i<MAX_SONS; i++)
        astPrint(node->son[i], level+1);
    for(i=0; i<level; i++)
        cout << "  ";
    cout << ")\n";
}

void astGenerateFile(AST *node, ofstream& output_file){
	if(node){
		switch(node->type){
			case AST_DEC: astGenerateFile(node->son[0], output_file);
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_VAR_DECL: astGenerateFile(node->son[0], output_file);
					    output_file << node->symbol->text;
					    output_file << " = " ;
					    astGenerateFile(node->son[1], output_file);
					    output_file << ";\n";
					    break;
			case AST_VECTOR_DECL: astGenerateFile(node->son[0], output_file);
						output_file << node->symbol->text;
						output_file << "[";
						astGenerateFile(node->son[1], output_file);
						output_file << "] = ";
						if(node->son[2] != 0){
							output_file << " ";
							astGenerateFile(node->son[2], output_file);
						}
						output_file << ";\n";
						break;
			case AST_VECTOR_DECL_EMPTY: astGenerateFile(node->son[0], output_file);
						output_file << node->symbol->text;
						output_file << "[";
						astGenerateFile(node->son[1], output_file);
						output_file << "]";
						output_file << ";\n";
						break;
			case AST_LCMD:
						astGenerateFile(node->son[0], output_file);
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_SYMBOL:output_file << node->symbol->text;
						if(node->son[0] != 0){
							output_file << " ";
							astGenerateFile(node->son[0], output_file);
						}
						break;
			case AST_FUN_DECL: astGenerateFile(node->son[0], output_file);
						output_file << node->symbol->text << "(";
						astGenerateFile(node->son[1], output_file);
						output_file << ")";
						astGenerateFile(node->son[2], output_file);
						output_file << "\n";
						break;
			case AST_FUN_PARAML: astGenerateFile(node->son[0]->son[0], output_file);
						output_file << node->son[0]->symbol->text;
						if(node->son[1] != 0)
							astGenerateFile(node->son[1], output_file);
						break;
			case AST_FUN_PARAMREST: output_file << ", ";
						astGenerateFile(node->son[0]->son[0], output_file);
						output_file << node->son[0]->symbol->text;
						if(node->son[1] != 0)
							astGenerateFile(node->son[1], output_file);
						break;
			case AST_BLOCK: output_file << "\t{\n";
						astGenerateFile(node->son[0], output_file);
						output_file << "\n\t}\n";
						break;
			case AST_VAR_ATRIB: output_file << node->symbol->text << " = ";
						astGenerateFile(node->son[0], output_file);
						output_file << ";\n";
						break;
			case AST_VECTOR_ATRIB: output_file << node->symbol->text << "[";
						astGenerateFile(node->son[0], output_file);
						output_file << "] = ";
						astGenerateFile(node->son[1], output_file);
						output_file << ";\n";
						break;
			case AST_ADD: astGenerateFile(node->son[0], output_file);
						output_file << " + ";
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_SUB: astGenerateFile(node->son[0], output_file);
						output_file << " - ";
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_MUL: astGenerateFile(node->son[0], output_file);
						output_file << " * ";
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_DIV: astGenerateFile(node->son[0], output_file);
						output_file << " / ";
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_LESS: astGenerateFile(node->son[0], output_file);
						output_file << " < ";
						astGenerateFile(node->son[1], output_file);
						break;
      		case AST_GREATER: astGenerateFile(node->son[0], output_file);
						output_file << " > ";
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_NEG: astGenerateFile(node->son[0], output_file);
						output_file << " ~ ";
						astGenerateFile(node->son[1], output_file);
						break;
     		case AST_EQ: astGenerateFile(node->son[0], output_file);
						output_file << " = ";
						astGenerateFile(node->son[1], output_file);
						break;
      		case AST_AND: astGenerateFile(node->son[0], output_file);
						output_file << " & ";
						astGenerateFile(node->son[1], output_file);
						break;
      		case AST_OR: astGenerateFile(node->son[0], output_file);
						output_file << " | ";
						astGenerateFile(node->son[1], output_file);
						break;
      		case AST_ARRAY_POS: output_file << node->symbol->text << "[";
						astGenerateFile(node->son[0], output_file);
						output_file << "]";
						break;
      		case AST_FUNCALL:output_file << node->symbol->text << "(";
						astGenerateFile(node->son[0], output_file);
						output_file << ")";
						break;
			case AST_KW_CHAR: output_file << "char "; break;
			case AST_KW_INT: output_file << "int "; break;
			case AST_KW_READ: output_file  << "read " << node->symbol->text << ";\n"; break;
      		case AST_KW_RETURN: output_file  << "return ";
						astGenerateFile(node->son[0], output_file);
						output_file << ";\n";
						break;
      		case AST_KW_PRINT: output_file  << "print ";
						astGenerateFile(node->son[0], output_file);
						output_file << ";\n";
						break;
			case AST_KW_WHILE: output_file << "while ( ";
	          			astGenerateFile(node->son[0], output_file);
						output_file << ")\n";
						astGenerateFile(node->son[1], output_file);
						break;
			case AST_KW_IF: output_file << "if (";
						astGenerateFile(node->son[0], output_file);
						output_file << ") then\n";
						astGenerateFile(node->son[1], output_file);
						output_file << "\n";
						if(node->son[2] != 0){
							output_file << "else\n";
							astGenerateFile(node->son[2], output_file);
						}
						break;
			case AST_NEW_BLOCK: astGenerateFile(node->son[0], output_file);						
						break;
			case AST_PRINT_ARG: astGenerateFile(node->son[0], output_file);
						if(node->son[1] != 0){
							output_file << " ";
							astGenerateFile(node->son[1], output_file);
						}
						break;
			case AST_PARAML: astGenerateFile(node->son[0], output_file);
						if(node->son[1] != 0){
							output_file << ", ";
							astGenerateFile(node->son[1], output_file);
						}
						break;
			case AST_EXP_P: output_file << "( ";
						astGenerateFile(node->son[0], output_file);
						output_file << " )";
						break;
			case AST_END_ESCAPE: output_file << ";"; break;
			default: output_file << "UNKNOWN"; break;
		}
	}
}

