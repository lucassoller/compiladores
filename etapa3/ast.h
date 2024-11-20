// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#ifndef AST_HEADER
#define AST_HEADER
#pragma once

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"

#define MAX_SONS 4

//BEGINNING
#define AST_DEC	0

//VARIABLES AND VECTORS DECLARATIONS
#define AST_VAR_DECL 1
#define AST_VECTOR_DECL 2
#define AST_VECTOR_DECL_EMPTY 3

//COMMAND LIST AND SYMBOL
#define AST_LCMD 4
#define AST_SYMBOL 5

//FUNCTIONS DECLARATION
#define AST_FUN_DECL 6
#define AST_FUN_PARAML 7
#define AST_FUN_PARAMREST 8

//BLOCK
#define AST_BLOCK 9

//ATRIBUTORS
#define AST_VAR_ATRIB 10
#define AST_VECTOR_ATRIB 11

//OPERATORS
#define AST_ADD 12
#define AST_SUB 13
#define AST_MUL 14
#define AST_DIV 15
#define AST_LESS 16
#define AST_GREATER	17
#define AST_NEG 18

//COMPARATORS
#define AST_EQ 19
#define AST_AND 20
#define AST_OR 21

//CALLERS
#define AST_ARRAY_POS 22
#define AST_FUNCALL 23

//SPECIAL CHARACTERS
#define AST_KW_CHAR	24
#define AST_KW_INT 25
#define AST_KW_READ 26
#define AST_KW_RETURN 27
#define AST_KW_PRINT 28
#define AST_KW_WHILE 29
#define AST_KW_IF 30
#define AST_NEW_BLOCK 31

//PARAMETERS
#define AST_PRINT_ARG 32
#define AST_PARAML 33
#define AST_EXP_P 34

#define AST_LIT_CHAR 35
#define AST_LIT_INT 36

extern ofstream outputfile;

typedef struct astnode
{
    int type;
    Symbol *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *astCreate(int type, Symbol *symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST *node, int level);
void astGenerateFile(AST *node, ofstream& output_file);
void astGenerateFile(AST *node);

#endif