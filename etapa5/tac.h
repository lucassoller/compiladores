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

#define TAC_LE 9
#define TAC_GE 10
#define TAC_EQ 11
#define TAC_NE 12
#define TAC_AND 13
#define TAC_OR 14

#define TAC_ASS 15
#define TAC_POINTER_ASS 16
#define TAC_VEC_ATRIB 17
#define TAC_VEC_INDEX 18
#define TAC_READ 19
#define TAC_PRINT 20
#define TAC_RET 21
#define TAC_FUN_START 22
#define TAC_FUN_END 23
#define TAC_FUNCALL 24
#define TAC_FUNARG 25
#define TAC_VARDEC 26
#define TAC_VECDEC 27
#define TAC_POINTERDEC 28

#define TAC_IFZ		30
#define TAC_LABEL	31

#define TAC_JZ 34
#define TAC_JUMP 35
#define TAC_JE 36
#define TAC_PRINT_ARG 37

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
void tacPrintSingle(TAC* tac);

//Function tacPrintBack.
void tacPrintBack(TAC* tac);

//Function tacPrintReverse.
TAC* tacPrintReverse(TAC* last);

//Function tacPrintForward.
void tacPrintForward(TAC* tac);

#endif
