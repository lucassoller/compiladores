// UFRGS - COMPILADORES 2024-2 - TURMA B
// LUCAS SOLLER

#include "symbols.h"
#include "tac.h"
#include "ast.h"
#include <stdio.h>
#include "y.tab.h"

TAC* tacCreate(int type, Symbol* res, Symbol* op1, Symbol* op2) {

	TAC* newtac;
	newtac= (TAC*) calloc (1,sizeof(TAC));
	newtac->type = type;
	newtac->res = res;
	newtac->op1 = op1;
	newtac->op2 = op2;
	newtac->prev = 0;
	newtac->next = 0;
	return newtac;
}

TAC* codeGenerator(AST* node) {

	int i;
	TAC* result = 0;
	TAC* code[MAX_SONS];
	if (!node) return 0;
	for(i = 0; i < MAX_SONS; ++i) {

		code[i] = codeGenerator(node->son[i]);
	}

	switch (node->type) {

		case AST_SYMBOL: result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0); break;
		case AST_ADD: result = makeBinOp (TAC_ADD, code[0], code[1]); break;
		case AST_SUB: result = makeBinOp (TAC_SUB, code[0], code[1]); break;
		case AST_MUL: result = makeBinOp (TAC_MUL, code[0], code[1]); break;
		case AST_DIV: result = makeBinOp (TAC_DIV, code[0], code[1]); break;
		case AST_LESS: result = makeBinOp (TAC_LESS, code[0], code[1]); break;
		case AST_GREATER: result = makeBinOp (TAC_GREATER, code[0], code[1]); break;
		case AST_NEG: result = makeBinOp (TAC_NEG, code[0], code[1]); break;
		case AST_EQ: result = makeBinOp (TAC_EQ, code[0], code[1]); break;
		case AST_AND: result = makeBinOp (TAC_AND, code[0], code[1]); break;
		case AST_OR: result = makeBinOp (TAC_OR, code[0], code[1]); break;
		case AST_VAR_ATRIB: return tacJoin(code[0],tacCreate(TAC_ASS, node->symbol,code[0]?code[0]->res:0,0));break;
		case AST_VECTOR_ATRIB: return tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_VEC_ATRIB, node->symbol, code[1]?code[1]->res:0, code[0]?code[0]->res:0)); break;
		case AST_ARRAY_POS: return tacJoin(code[0], tacCreate(TAC_VEC_INDEX, makeTemp(), node->symbol, code[0]?code[0]->res:0)); break;
		case AST_KW_READ: return tacCreate(TAC_READ, node->symbol, 0, 0); break;
		case AST_PRINT_ARG:  return makePrint(code[0], code[1]); break;
		case AST_KW_RETURN: return tacJoin(code[0], tacCreate(TAC_RET, code[0]?code[0]->res:0, 0, 0)); break;
		case AST_KW_IF: return makeIfThenElse(code[0], code[1], code[2]); break;
		case AST_KW_WHILE: return makeWhile(code[0], code[1]); break;
		case AST_FUN_DECL: return makeFun(node->symbol, code[2]); break;
		case AST_FUNCALL: result = tacJoin(code[0], tacCreate(TAC_FUNCALL, makeTemp(), node->symbol, 0)); updateFuncArgs(result, node->symbol); return result; break;
		case AST_PARAML: return tacJoin(tacJoin(code[0], tacCreate(TAC_FUNARG, 0, code[0]?code[0]->res:0, 0)), code[1]); break;
		case AST_VAR_DECL: return tacJoin(code[0], tacCreate(TAC_VARDEC, node->symbol, code[1]?code[1]->res:0, 0)); break;
		case AST_VECTOR_DECL: return tacJoin(code[0], tacCreate(TAC_VECDEC, node->symbol, code[1]?code[1]->res:0, 0)); break;
		case AST_VECTOR_DECL_EMPTY: return tacJoin(code[0], tacCreate(TAC_VECDEC, node->symbol, code[1]?code[1]->res:0, 0)); break;
		default: result = tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]); break;
	}

	return result;
}

TAC* makePrint(TAC* code0, TAC* code1){

	if(code0){
		if (code0->res->type == SYMBOL_LIT_TEXT)
			return tacJoin(code1, tacCreate(TAC_PRINT, code0?code0->res:0, 0, 0));
		else
			return tacJoin(tacJoin(code0, tacCreate(TAC_PRINT_ARG, code0?code0->res:0, 0, 0)), code1);
	}
	return NULL;

}

TAC* makeIfThenElse (TAC* code0, TAC* code1, TAC* code2) {

	TAC* newIfTac = 0;
	TAC* newLabelTac= 0;
	TAC* ElseJmpTac = 0;
	TAC* newLabelElseTac = 0;

	Symbol* newLabel = 0;
	Symbol* elseLabel = 0;

	newLabel = makeLabel();
	elseLabel = makeLabel();

	newIfTac = tacCreate(TAC_JZ, newLabel, code0?code0->res:0, 0);
	ElseJmpTac = tacCreate(TAC_JUMP, elseLabel, code0?code0->res:0, 0);

	newLabelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
	newLabelElseTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);

	if(code2 == NULL){
		return tacJoin(tacJoin(tacJoin (code0, newIfTac), code1), newLabelTac);
	} else {
		return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin (code0, newIfTac), code1), newLabelTac), ElseJmpTac), code2), newLabelElseTac);
	}
}

TAC* makeWhile(TAC* code0, TAC* code1){
	TAC* preConditionLabelTac = 0;
	TAC* postBlockLabelTac = 0;
	TAC* JmpTac = 0;
	TAC* JzTac = 0;

	Symbol* newPreConditionLabel;
	Symbol* newPostBlockLabel;

	newPreConditionLabel = makeLabel();
	newPostBlockLabel = makeLabel();

	preConditionLabelTac = tacCreate(TAC_LABEL, newPreConditionLabel, 0, 0);
	postBlockLabelTac = tacCreate(TAC_LABEL, newPostBlockLabel, 0, 0);
	JmpTac = tacCreate(TAC_JUMP, newPreConditionLabel, 0, 0);
	JzTac = tacCreate(TAC_JZ, newPostBlockLabel, code0?code0->res:0, 0);

	return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(preConditionLabelTac, code0), JzTac), code1), JmpTac), postBlockLabelTac);
}

TAC* makeFun(Symbol* funSymbol, TAC* code2){
	TAC* beginFunTac = 0;
	TAC* endFunTac = 0;

	beginFunTac = tacCreate(TAC_FUN_START, funSymbol, 0, 0);
	endFunTac = tacCreate(TAC_FUN_END, funSymbol, 0, 0);

	return tacJoin(tacJoin(beginFunTac, code2), endFunTac);
}

void updateFuncArgs(TAC* func, Symbol* symbol){
	TAC* curr_arg;
	curr_arg = func;

	if(!curr_arg) return;

	while(curr_arg->prev != NULL){
		curr_arg = curr_arg->prev;

		if(curr_arg->type == TAC_FUNARG)
			curr_arg->res = symbol;
	}
}

TAC* makeBinOp(int type, TAC* code0, TAC* code1) {

	TAC* newtac = tacCreate(type, makeTemp(), code0?code0->res:0, code1?code1->res:0);
	return tacJoin(code0, tacJoin(code1, newtac));
}

TAC* tacJoin (TAC* l1, TAC* l2) {

	TAC* tac = l2;
	if (!l1) return l2;
	if (!l2) return l1;

	while(tac->prev)
		tac = tac->prev;

	tac->prev = l1;
	return l2;
}

void tacPrint(TAC* tac) {

	if (!tac) return;
	if (tac->type == TAC_SYMBOL) return;
	cerr << " TAC(";
	switch(tac->type) {

		case TAC_SYMBOL: cerr << "TAC_SYMBOL ";	break;
		case TAC_ADD: cerr << "TAC_ADD ";	break;
		case TAC_SUB: cerr << "TAC_SUB ";	break;
		case TAC_MUL: cerr << "TAC_MUL ";	break;
		case TAC_DIV: cerr << "TAC_DIV ";	break;
		case TAC_LESS: cerr << "TAC_LESS ";	break;
		case TAC_GREATER: cerr << "TAC_GREATER ";	break;
		case TAC_NEG: cerr << "TAC_NEG ";	break;
		case TAC_EQ: cerr << "TAC_EQ ";	break;
		case TAC_AND: cerr << "TAC_AND ";	break;
		case TAC_OR: cerr << "TAC_OR ";	break;
		case TAC_ASS: cerr << "TAC_ASS ";	break;
		case TAC_VEC_ATRIB: cerr << "TAC_VEC_ATRIB "; break;
		case TAC_VEC_INDEX: cerr << "TAC_VEC_INDEX "; break;
		case TAC_READ: cerr << "TAC_READ "; break;
		case TAC_PRINT: cerr << "TAC_PRINT "; break;
		case TAC_PRINT_ARG: cerr << "TAC_PRINT_ARG "; break;
		case TAC_RET: cerr << "TAC_RET "; break;
		case TAC_LABEL: cerr << "TAC_LABEL "; break;
		case TAC_JZ: cerr << "TAC_JZ "; break;
		case TAC_JUMP: cerr << "TAC_JUMP "; break;
		case TAC_JE: cerr << "TAC_JE "; break;
		case TAC_FUN_START: cerr << "TAC_FUN_START "; break;
		case TAC_FUN_END: cerr << "TAC_FUN_END "; break;
		case TAC_FUNCALL: cerr << "TAC_FUNCALL "; break;
		case TAC_FUNARG: cerr << "TAC_FUNARG "; break;
		case TAC_VARDEC: cerr << "TAC_VARDEC "; break;
		case TAC_VECDEC: cerr << "TAC_VECDEC "; break;
		default: cerr << "UNKNOW ";	break;
	}
	
	if(tac->res) {
		cerr << tac->res->text << " ";
	}

	if (tac->op1) {
		cerr << tac->op1->text << " ";
	}

	if (tac->op2) {
		cerr << tac->op2->text << " ";
	}
	
	cerr << ")\n";
}

void tacPrintBack(TAC* tac) {

	if (!tac) return;

	tacPrintBack(tac->prev);
	tacPrint(tac);
}