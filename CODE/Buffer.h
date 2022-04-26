/*************************************************************
* File name : Buffer.h
* Compiler : MS Visual Studio 2019
* Modified by : Mathieu Theriault and Ajithyugan Jeyakumar
* Course : CST 8152 – Compilers, Lab Section : [011, 012, 013]
* Assignment : A21, A22, A23.
* Date : July 12 2021
* Professor : Paulo Sousa
* Purpose: This file is the main header for Buffer (.h)
* ************************************************************/

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'F',
	MODE_ADDIT = 'A',
	MODE_MULTI = 'M'
};

/* TO_DO: Adjust all constants to your Language */
#define BUFFER_ERROR (-1)				/* General error message */
#define BUFFER_EOF '\0'					/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (CIPHER) .................................. */
#define CIPHER_DEFAULT_SIZE 100			/* default initial buffer capacity */
#define CIPHER_DEFAULT_INCREMENT 10		/* default increment factor */

/* You should add your own constant definitions here */
#define CIPHER_MAX_SIZE SHRT_MAX-1		/*maximum capacity*/ 

/* TO_DO: Adjust to operate with MSB */

#define CIPHER_DEFAULT_FLAG 0x3F /*0011.1111*/
#define CIPHER_SET_FLAG_RLC 0x80 /*1000.0000*/
#define CIPHER_RST_FLAG_RLC 0x7F /*0111.1111*/
#define CIPHER_CHK_FLAG_RLC 0x80 /*1000.0000*/
#define CIPHER_SET_FLAG_EOB 0x40 /*0100.0000*/
#define CIPHER_RST_FLAG_EOB 0xBF /*1011.1111*/
#define CIPHER_CHK_FLAG_EOB 0x40 /*0100.0000*/


/* Logical constants - adapt for your language */
#define CIPHER_TRUE 1
#define CIPHER_FALSE 0

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (CIPHER) .................................. */

typedef short			cipher_small;       /* SHORT */
typedef int			    cipher_int;         /* INTEGER*/
typedef long			cipher_long;	    /* LONG */
typedef float			cipher_float;       /* FLOAT */
typedef double			cipher_precise;	    /* DOUBLE */
typedef char			cipher_chr;         /* CHAR */
typedef int 			cipher_bol;         /* BOOLEAN */
typedef unsigned char	cipher_flg;         /* FLAGS */
typedef void			cipher_null;		/* VOID */

/* user data type declarations */
typedef struct OffsetCipher {
	cipher_int addC;      /* the offset (in chars) to the add-character location */
	cipher_int getC;      /* the offset (in chars) to the get-character location */
	cipher_int mark;      /* the offset (in chars) to the mark location */
} Offset;

/* user data type declarations */
typedef struct BufferCipher {
	cipher_chr* string;         /* pointer to the beginning of character array (character buffer) */
	cipher_int size;            /* current dynamic memory size (in bytes) allocated to character buffer */
	cipher_int increment;       /* character array increment factor */
	cipher_int mode;            /* operational mode indicator*/
	cipher_flg flags;  /* contains character array reallocation flag and end-of-buffer flag */
	Offset offset;
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* Function declarations */
BufferPointer bufferCreate(cipher_int, cipher_int, cipher_int);
BufferPointer bufferAddChar(BufferPointer const, cipher_chr);
cipher_bol bufferRetract(BufferPointer const);
cipher_bol bufferRestore(BufferPointer const);
cipher_bol bufferRewind(BufferPointer const);
cipher_bol bufferClean(BufferPointer const);
cipher_bol bufferDestroy(BufferPointer const);

cipher_bol bufferSetOffsetMark(BufferPointer const, cipher_int);
cipher_int bufferPrint(BufferPointer const);
cipher_int bufferLoad(BufferPointer const, FILE* const);

cipher_bol bufferCheckFull(BufferPointer const);
cipher_bol bufferCheckEmpty(BufferPointer const);

cipher_int bufferGetSize(BufferPointer const);
cipher_int bufferGetOffsetAddC(BufferPointer const);
cipher_int bufferGetOffsetMark(BufferPointer const);
cipher_int bufferGetOffsetGetC(BufferPointer const);
cipher_int bufferGetIncrement(BufferPointer const);
cipher_int bufferGetMode(BufferPointer const);
cipher_chr bufferGetChar(BufferPointer const);
cipher_chr* bufferGetSubString(BufferPointer const, cipher_int);
cipher_flg bufferGetFlags(BufferPointer const);


#endif