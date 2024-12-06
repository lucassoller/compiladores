// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#ifndef TAC_HEADER
#define TAC_HEADER

#include <stdio.h>
#include "symbols.h"
#include "ast.h"

#define TAC_SYMBOL 	1
#define TAC_ADD 	2
#define TAC_SUB 	3
#define TAC_MUL 	4
#define TAC_DIV 	5

#define TAC_LESS 6
#define TAC_GREATER 7
#define TAC_NEG 8

#define TAC_EQ 9
#define TAC_AND 10
#define TAC_OR 11

#define TAC_ASS 12
#define TAC_VEC_ATRIB 13
#define TAC_VEC_INDEX 14
#define TAC_READ 15
#define TAC_PRINT 16
#define TAC_RET 17
#define TAC_FUN_START 18
#define TAC_FUN_END 19
#define TAC_FUNCALL 20
#define TAC_FUNARG 21
#define TAC_VARDEC 22
#define TAC_VECDEC 23

#define TAC_IFZ		24
#define TAC_LABEL	25

#define TAC_JZ 26
#define TAC_JUMP 27
#define TAC_JE 28
#define TAC_PRINT_ARG 29

typedef struct tac {

int type;
Symbol* res;
Symbol* op1;
Symbol* op2;
struct tac *prev;
struct tac *next;

} TAC;

//Function tacCreate.
TAC* tacCreate(int type, Symbol* res, Symbol* op1, Symbol* op2);

//Function codeGenerator.
TAC* codeGenerator(AST* node);

//Function makeBinOp.
TAC* makeBinOp(int type, TAC* code0, TAC* code1);

//Function tacJoin.
TAC* tacJoin (TAC* l1, TAC* l2);

//Function tacPrintSingle.
void tacPrint(TAC* tac);

//Function tacPrintBack.
void tacPrintBack(TAC* tac);

//Function Print
TAC* makePrint(TAC* code0, TAC* code1);

//Function IF ELSE
TAC* makeIfThenElse (TAC* code0, TAC* code1, TAC* code2);

//Function While
TAC* makeWhile(TAC* code0, TAC* code1);

//Create functions
TAC* makeFun(Symbol* funSymbol, TAC* code3);

//Update function args
void updateFuncArgs(TAC* func, Symbol* symbol);
#endif
