/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2021
*************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2019
* Author: Svillen Ranev - Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A3.
* Date: May 01 2021
* Purpose: This file is the main header for Parser (.h)
* Function list: (...).
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
cipher_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern cipher_int line;
extern Token tokenizer();
extern cipher_chr* keywordTable[];

/* TO_DO: Define the name of your language */
#define STR_LANGNAME "Cipher"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */
/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	_prime_,
	_small_,
	_int_,
	_large_,
	_decimal_,
	_precise_,
	_char_,
	_string_,
	_boolean_,
	_accept_,
	_display_,
	_if_,
	_elif_,
	_else_,
	_repeat_,
	_during_,
	_execute_,
	_fixed_
	/* TO_DO: Continue with your keywords */
};

/* TO_DO: Update the typedefs used in your language */

/* Function definitions */
cipher_null startParser();
cipher_null matchToken(cipher_int, cipher_int);
cipher_null syncErrorHandler(cipher_int);
cipher_null printError();

/* TODO: Place ALL non-terminal function declarations */
cipher_null additiveArithmeticExpression();
cipher_null additiveArithmeticExpressionPrime();
cipher_null arithmeticExpression();
cipher_null assignmentExpression();
cipher_null assignmentStatement();// i have to call the Assignment expreson in side this method.
cipher_null codeSession();
cipher_null conditionalExpression();
cipher_null dataSession();//
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
cipher_null optVarListDeclarations();
cipher_null optionalStatements();
cipher_null outputStatement();
cipher_null outputVariableList();
cipher_null primaryArithmeticExpression();
cipher_null primaryRelationalArithmeticExpression();
cipher_null primaryRelationalStringExpression();
cipher_null primaryStringExpression();
cipher_null program();
cipher_null relationalArithmeticExpression();
cipher_null relationalArithmeticOperator();
cipher_null relationalExpression();
cipher_null relationalStringExpression();
cipher_null relationalStringOperator();
cipher_null selectionStatement();
cipher_null statement();
cipher_null statements();
cipher_null statementsPrime();
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
//cipher_null preCondition();

#endif