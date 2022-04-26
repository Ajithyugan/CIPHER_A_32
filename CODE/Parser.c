/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2021
* Author: Svillen Ranev - Paulo Sousa.
*************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2019
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A3.
* Date: May 01 2021
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*************************************************************
 * Process Parser
 ************************************************************/

cipher_null startParser() {
	lookahead = tokenizer();
	program();
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*************************************************************
 * Match Token
 ************************************************************/
 /* TODO: This is the main code for match - check your definition */
cipher_null matchToken(cipher_int tokenCode, cipher_int tokenAttribute) {
	cipher_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		// TO_DO: Include the other cases
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/
 /* TODO: This is the function to handler error - adjust basically datatypes */
cipher_null syncErrorHandler(cipher_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*************************************************************
 * Print Error
 ************************************************************/
 /* TODO: This is the function to error printing - adjust basically datatypes */
cipher_null printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case STR_T:
		printf("%s\n", bufferGetSubString(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case ASS_OP_T:
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case EOS_T:
		printf("NA\n");
		break;
		// TO_DO: Adjust the other cases
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ************************************************************/
cipher_null program() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == _prime_) {
			matchToken(KW_T, _prime_);
			matchToken(LBR_T, NO_ATTR);
			optVarListDeclarations();
			optionalStatements();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printError();
		}
	case SEOF_T:
		; // Empty: for optional
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ************************************************************/
 /*
 cipher_null dataSession() {
	 matchToken(KW_T, DATA);
	 matchToken(LBR_T, NO_ATTR);
	 optVarListDeclarations();
	 matchToken(RBR_T, NO_ATTR);
	 printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
 }
 */

 /*************************************************************
  * Optional Var List Declarations
  * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
  * FIRST(<opt_varlist_declarations>) = { e, KW_T (INT), KW_T (FLOAT), KW_T (STRING)}.
  ************************************************************/
cipher_null optVarListDeclarations() {
	// TO_DO: Basic implementation
	; // Empty
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*************************************************************
 * codeSession statement
 * BNF: <codeSession> -> CODE { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (CODE)}.
 ************************************************************/
 /*
 cipher_null codeSession() {
	 matchToken(KW_T, CODE);
	 matchToken(LBR_T, NO_ATTR);
	 optionalStatements();
	 matchToken(RBR_T, NO_ATTR);
	 printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
 }
 */
 /* TODO_205: Continue the development (all non-terminal functions) */

 /*************************************************************
  * Optional statement
  * BNF: <opt_statements> -> <statements> | ϵ
  * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(IF),
  *				KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
  ************************************************************/
cipher_null optionalStatements() {	// TO_DO: Basic implementation
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == _display_) {
			statements();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(IF),
 *		KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ************************************************************/
cipher_null statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE) }
 ************************************************************/
cipher_null statementsPrime() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == _display_) {
			statement();
			statementsPrime();
			break;
		}
	default:
		; //empty string
	}
}

/*************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(IF), KW_T(WHILE),
 *			KW_T(READ), KW_T(WRITE) }
 ************************************************************/
cipher_null statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case _display_:
			outputStatement();
			break;
		/*case _accept_:
			inputStatement();
			break;
		case _during_:
			iterationStatement();
			break;
		case _if_:
			selectionStatement();
			break;
		case GID_T:
			assignmentStatement();*/

			/*
	case GID_T:
	case  _accept_:
	case _display_:
	case	  _if_:
	case  _during_:
		inputStatement();
		break;
		*/
		default:
			printError();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*************************************************************
 * Output Statement
 * BNF: <output statement> -> WRITE (<output statementPrime>);
 * FIRST(<output statement>) = { KW_T(WRITE) }
 ************************************************************/
cipher_null outputStatement() {
	matchToken(KW_T, _display_);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ************************************************************/
cipher_null outputVariableList() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/* TO_DO: Continue developing the non-terminal functions 

cipher_null additiveArithmeticExpression() {

}
cipher_null additiveArithmeticExpressionPrime() {

}
cipher_null arithmeticExpression() {

}
cipher_null assignmentExpression() {

}
cipher_null assignmentStatement() {

}


cipher_null conditionalExpression();
cipher_null fltVariableIdentifier();
cipher_null fltVarList();
cipher_null fltVarListPrime();
cipher_null inputStatement();
cipher_null intVariableIdentifier();
cipher_null intVarList();
cipher_null intVarListPrime();
cipher_null iterationStatement();
cipher_null logicalAndExpression();
cipher_null logicalAndExpressionPrime();
cipher_null logicalNotExpression();
cipher_null logicalOrExpression();
cipher_null logicalOrExpressionPrime();
cipher_null multiplicativeArithmeticExpression();
cipher_null multiplicativeArithmeticExpressionPrime();
cipher_null fltVarListDeclaration();
cipher_null intVarListDeclaration();
cipher_null strVarListDeclaration();
cipher_null primaryArithmeticExpression();
cipher_null primaryRelationalArithmeticExpression();
cipher_null primaryRelationalStringExpression();
cipher_null primaryStringExpression();
cipher_null relationalArithmeticExpression();
cipher_null relationalArithmeticOperator();
cipher_null relationalExpression();
cipher_null relationalStringExpression();
cipher_null relationalStringOperator();
cipher_null selectionStatement();
cipher_null stringExpression();
cipher_null stringExpressionPrime();
cipher_null strVariableIdentifier();
cipher_null strVarList();
cipher_null strVarListPrime();
cipher_null variableIdentifier();
cipher_null variableList();
cipher_null variableListPrime();
cipher_null varListDeclarations();
cipher_null varListDeclaration();
cipher_null varListDeclarationsPrime();
*/