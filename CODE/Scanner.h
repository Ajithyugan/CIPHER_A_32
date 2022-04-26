/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2021
*************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2019
* Author: Svillen Ranev - Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A2.
* Date: May 01 2021
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 8   /* variable identifier length */
#define ERR_LEN 20  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for DL */
#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO 2: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/* Error token */
	GID_T,      /* Generic variable identifier*/
	DL_T,		/* Integer literal token */
	FPL_T,		/* Floating point literal token */
	STR_T,		/* String literal token */
	SCC_OP_T,	/* String concatenation operator token: $$ (old: ## or >>) */
	ASS_OP_T,	/* Assignment operator token */
	ART_OP_T,	/* Arithmetic operator token */
	REL_OP_T,	/* Relational operator token */
	LOG_OP_T,	/* Logical operator token */
	LPR_T,		/* Left parenthesis token */
	RPR_T,		/* Right parenthesis token */
	LBR_T,		/* Left brace token */
	RBR_T,		/* Right brace token */
	KW_T,		/* Keyword token */
	COM_T,		/* Comma token */
	RT_T,		/* Run-time error token */
	EOS_T,		/* End of statement (period) */
	RTE_T,		/* Run-time error token */
	SEOF_T		/* Source end-of-file token */
};

/* CHECK: Operators token attributes */
typedef enum ArithmeticOperators { ADD, SUB, MUL, DIV } AriOperator;
typedef enum RelationalOperators { EQ, NE, GT, LT } RelOperator;
typedef enum LogicalOperators { AND, OR, NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_EOF } EofOperator;

/* TO_DO 3A: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	cipher_int codeType;				/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	cipher_int intValue;				/* integer literal attribute (value) */
	cipher_int keywordIndex;			/* keyword index in the keyword table */
	cipher_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	cipher_float floatValue;			/* floating-point literal attribute (value) */
	cipher_chr specialValue;			/* special characters*/
	cipher_chr vidLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	cipher_chr errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO 3B: Should be used if no symbol table is implemented */
typedef struct GidAttributes {
	cipher_flg flags;			/* Flags information */
	union {
		cipher_int intValue;				/* Integer value */
		cipher_float floatValue;			/* Float value */
		cipher_null* stringContent;		/* String value */
	} values;
} GidAttributes;

/* CHECK: Token declaration */
typedef struct Token {
	cipher_int code;					    /* token code */
	TokenAttribute attribute;		/* token attribute */
	GidAttributes   gidAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* CHECK: EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO 4: Error states and illegal state */
#define ES  13		/* Error state  with no retract */
#define ER  14		/* Error state  with retract */
#define IS -1		/* Illegal state */

 /* TO_DO 5: State transition table definition */
#define TABLE_COLUMNS 7

/* TO_DO 6: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '.'
#define CHRCOL4 '"'



/* TO_DO 7: Transition table - type of states defined in separate table */
static cipher_int transitionTable[][TABLE_COLUMNS] = {
	/*			[A-z],	[0-9],	_,		.,		",		EOFS,	Other	*/
	/*			C(0),	N(1),	U(2),	P(3),	T(4),	E(5),	O(6)			*/
	/* S00 */	{1,		3,		10,		ES,		8,		ER,		ES},	/* NOAS */
	/* S01 */	{1,		1,		1,		2,		2,		ER,		2},		/* NOAS */
	/* S02 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (GID) */
	/* S03 */	{4,		3,		4,		5,		4,		ER,		4},		/* NOAS */
	/* S04 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASNR (DL) */
	/* S05 */	{ES,	6,		ES,		ES,		ES,		ER,		ES},	/* NOAS */
	/* S06 */	{7,		6,		7,		7, 		7,		ER,		7},		/* NOAS */
	/* S07 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (FPL) */
	/* S08 */	{8,		8,		8,		8,		9,		ER,		8},		/* NOAS */
	/* S09 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASNR (SL) */
	/* S10 */	{10,	ES,		11,		ES,		ES,		ER,		ES},	/* NOAS */
	/* S11 */	{12,	12,		12,		12,		12,		ER,		12},	/* NOAS */
	/* S12 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (KEY) */
	/* S13 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASNR (Err) */
	/* S14 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS}		/* ASWR (Err) */
};

/* CHECK: Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO 8: Define list of acceptable states */
static cipher_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASWR, /* 02 (GID) */
	NOAS, /* 03 */
	ASWR, /* 04 (DL) */
	NOAS, /* 05 */
	NOAS, /* 06 */
	ASWR, /* 07 (FPL) */
	NOAS, /* 08 */
	ASNR, /* 09 (SL) */
	NOAS, /* 10 */
	NOAS, /* 11 */
	ASWR, /* 12 (KEY) */
	ASNR, /* 13 (ERR) */
	ASWR  /* 14 (ERR) */
};

/*
-------------------------------------------------
TO_DO 9: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
cipher_int startScanner(BufferPointer psc_buf);
static cipher_int nextClass(cipher_chr c);			/* character class function */
static cipher_int nextState(cipher_int, cipher_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO_10A: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(cipher_chr* lexeme);

/* Declare accepting states functions */
Token funcGID(cipher_chr* lexeme);
Token funcDL(cipher_chr* lexeme);
Token funcFPL(cipher_chr* lexeme);
Token funcSL(cipher_chr* lexeme);
Token funcKEY(cipher_chr* lexeme);
Token funcErr(cipher_chr* lexeme);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* TO_DO_10B: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* 00 */
	NULL,		/* 01 */
	funcGID,	/* 02 */
	NULL,		/* 03 */
	funcDL,		/* 04 */
	NULL,		/* 05 */
	NULL,		/* 06 */
	funcFPL,	/* 07 */
	NULL,		/* 08 */
	funcSL,		/* 09 */
	NULL,		/* 10 */
	NULL,		/* 11 */
	funcKEY,	/* 12 */
	funcErr,	/* 13 */
	funcErr		/* 14 */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO 11A: Define the number of Keywords from the language */
#define KWT_SIZE 18

/* TO_DO 11B: Define the list of keywords */
static cipher_chr* keywordTable[KWT_SIZE] = {
	"_prime_",
	"_small_",
	"_int_",
	"_large_",
	"_decimal_",
	"_precise_",
	"_char_",
	"_string_",
	"_boolean_",
	"_accept_",
	"_display_",
	"_if_",
	"_elif_",
	"_else_",
	"_repeat_",
	"_during_",
	"_execute_",
	"_fixed_"
};

#endif
