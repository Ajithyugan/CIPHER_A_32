/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2020
* This Code is EXCLUSIVE for professors (must not be shared)
* Author: Svillen Ranev - Paulo Sousa
*************************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2019
* Author: Paulo Sousa
* Modified by: Mathieu Theriault and Ajithyugan Jeyakumar
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013, 014]
* Assignment: A1.
* Date: July 12 2021
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffeer (A1)
* Function list: (bufferCreate, bufferAddChar, bufferRetract, bufferRestore,
	bufferRewind, bufferClean, bufferDestroy, bufferSetOffsetMark, bufferPrint, bufferLoad, bufferCheckFull, bufferCheckEmpty, 
	bufferGetSize, bufferGetOffsetAddC, bufferGetOffsetMark, bufferGetOffsetGetC, bufferGetMode, bufferGetChar,
	bufferGetSubString, bufferGetFlag)
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

/************************************************************
* Function name: bufferCreate
* Purpose: Creates the buffer according to capacity, increment
	factor and operational mode (F, A, M)
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: Ver 2.0
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to Buffer)
* Algorithm: Allocation of memory according to inicial (default) values.
**************************************************************/

BufferPointer bufferCreate(cipher_int size, cipher_int increment, cipher_int mode) {
	BufferPointer b;
	if (size<0 || size>CIPHER_MAX_SIZE)
		return NULL;
	if (!size) {
		size = CIPHER_DEFAULT_SIZE;
		increment = CIPHER_DEFAULT_INCREMENT;
	}
	if (!increment)
		mode = MODE_FIXED;
	b = (BufferPointer)calloc(1, sizeof(Buffer));
	if (b == NULL)
		return NULL;
	b->string = (cipher_chr*)malloc(size);
	if (!b->string) {
		free(b);
		return NULL;
	}
	b->mode = mode;
	b->size = size;
	b->increment = increment;
	b->flags = CIPHER_DEFAULT_FLAG;
	return b;
}


/************************************************************
* Function name: bufferAddChar
* Purpose: Adds a char to buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   ch = char to be added
* Return value: bPointer (pointer to Buffer)
**************************************************************/

BufferPointer bufferAddChar(BufferPointer const pBuffer, cipher_chr ch) {
	cipher_chr* tempbuf;
	cipher_int newSize;
	if (!pBuffer)
		return NULL;
	pBuffer->flags = pBuffer->flags & CIPHER_RST_FLAG_RLC;
	if (pBuffer->offset.addC * sizeof(cipher_chr) == CIPHER_MAX_SIZE)
		return NULL;
	if (bufferCheckFull(pBuffer) == CIPHER_TRUE) {
		switch (pBuffer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			newSize = pBuffer->size + pBuffer->increment;
			if (newSize < 0 || newSize <= pBuffer->size)
				return NULL;
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		case MODE_MULTI:
			newSize = pBuffer->size * pBuffer->increment;
			if (newSize < 0 || newSize <= pBuffer->size)
				return NULL;
			if (DEBUG)
				printf("%s%d%s", "\n................\n* New size: ",
					newSize, "\n................\n");
			break;
		default:
			return NULL;
		}
		tempbuf = (char*)realloc(pBuffer->string, newSize);
		if (!tempbuf)
			return NULL;
		if (tempbuf != pBuffer->string) {
			pBuffer->flags = pBuffer->flags | CIPHER_SET_FLAG_RLC;
			pBuffer->string = tempbuf;
		}
		pBuffer->size = newSize;
	}
	pBuffer->string[pBuffer->offset.addC++] = ch;
	return pBuffer;
}

/************************************************************
* Function name: bufferClean
* Purpose: Clears the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferClean(BufferPointer const pBuffer) {
	if (!pBuffer)
		return CIPHER_FALSE;
	pBuffer->offset.addC = pBuffer->offset.mark = pBuffer->offset.getC = 0;
	pBuffer->flags = pBuffer->flags & CIPHER_RST_FLAG_EOB;
	pBuffer->flags = pBuffer->flags & CIPHER_RST_FLAG_RLC;
	return CIPHER_TRUE;
}

/************************************************************
* Function name: bufferDestroy
* Purpose: Releases the buffer address
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferDestroy(BufferPointer const pBuffer) {
	if (!pBuffer)
		return CIPHER_FALSE;
	free(pBuffer->string);
	free(pBuffer);
	return CIPHER_TRUE;
}

/************************************************************
* Function name: bufferCheckFull
* Purpose: Checks if buffer is full
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferCheckFull(BufferPointer const pBuffer) {
	if (!pBuffer)
		return CIPHER_FALSE;
	if (pBuffer->size == pBuffer->offset.addC * (int)sizeof(char))
		return CIPHER_TRUE;
	return CIPHER_FALSE;
}

/************************************************************
* Function name: bufferGetOffsetAddC
* Purpose: Returns the position of char to be added
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: addcPosition value
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferGetOffsetAddC(BufferPointer const pBuffer) {
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->offset.addC;
}

/************************************************************
* Function name: bufferGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Size of buffer.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferGetSize(BufferPointer const pBuffer) {
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->size;
}

/************************************************************
* Function name: bufferGetMode
* Purpose: Returns the operational mode
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: operational mode.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferGetMode(BufferPointer const pBuffer) {
	if (!pBuffer)
		return (char)BUFFER_ERROR;
	return pBuffer->mode;
}


/************************************************************
* Function name: bufferGetOffsetMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: mark offset.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/

cipher_int bufferGetOffsetMark(BufferPointer const pBuffer) {
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->offset.mark;
}


/************************************************************
* Function name: bufferSetOffsetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   mark = mark position for char
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferSetOffsetMark(BufferPointer const pBuffer, cipher_int mark) {
	if (!pBuffer || mark<0 || mark > pBuffer->offset.addC)
		return CIPHER_FALSE;
	pBuffer->offset.mark = mark;
	return CIPHER_TRUE;
}

/************************************************************
* Function name: bufferPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Number of chars printed.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferPrint(BufferPointer const pBuffer) {
	cipher_int cont = 0;
	char c;
	if (!pBuffer || !(pBuffer->string))
		return BUFFER_ERROR;
	c = bufferGetChar(pBuffer);
	while (!(pBuffer->flags & CIPHER_CHK_FLAG_EOB)) {
		cont++;
		printf("%c", c);
		c = bufferGetChar(pBuffer);
	}
	return cont;
}

/************************************************************
* Function name: bufferLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   fi = pointer to file descriptor
* Return value: Number of chars read and put in buffer.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferLoad(BufferPointer const pBuffer, FILE* const fi) {
	cipher_int size = 0;
	char c;
	if (!fi || !pBuffer)
		return BUFFER_ERROR;
	c = (char)fgetc(fi);
	while (!feof(fi)) {
		if (!bufferAddChar(pBuffer, c)) {
			ungetc(c, fi);
			return BUFFER_ERROR;
		}
		c = (char)fgetc(fi);
		size++;
	}
	if (ferror(fi))
		return BUFFER_ERROR;
	return size;
}

/************************************************************
* Function name: bufferCheckEmpty
* Purpose: Checks if buffer is empty.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferCheckEmpty(BufferPointer const pBuffer) {
	if (!pBuffer)
		return CIPHER_FALSE;
	if (pBuffer->offset.addC == 0)
		return CIPHER_TRUE;
	return CIPHER_FALSE;
}

/************************************************************
* Function name: bufferGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Char in the getC position.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_chr bufferGetChar(BufferPointer const pBuffer) {
	if (!pBuffer)
		return BUFFER_ERROR;
	if (pBuffer->offset.getC == pBuffer->offset.addC) {
		pBuffer->flags = pBuffer->flags | CIPHER_SET_FLAG_EOB;
		return '\0';
	}
	pBuffer->flags = pBuffer->flags & CIPHER_RST_FLAG_EOB;
	return pBuffer->string[pBuffer->offset.getC++];
}


/************************************************************
* Function name: bufferRewind
* Purpose: Rewinds the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferRewind(BufferPointer const pBuffer) {
	if (!pBuffer)
		return CIPHER_FALSE;
	pBuffer->offset.getC = 0;
	pBuffer->offset.mark = 0;
	return CIPHER_TRUE;
}


/************************************************************
* Function name: bufferRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferRetract(BufferPointer const pBuffer) {
	if (!pBuffer || pBuffer->offset.getC == 0)
		return CIPHER_FALSE;
	pBuffer->offset.getC--;
	return CIPHER_TRUE;
}


/************************************************************
* Function name: bufferRestore
* Purpose: Resets the buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Boolean value about operation success
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_bol bufferRestore(BufferPointer const pBuffer) {
	if (!pBuffer)
		return CIPHER_FALSE;
	pBuffer->offset.getC = pBuffer->offset.mark;
	return CIPHER_TRUE;
}


/************************************************************
* Function name: bufferGetCPosition
* Purpose: Returns the value of getCPosition.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: The getC offset.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferGetOffsetGetC(BufferPointer const pBuffer) {
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->offset.getC;
}


/************************************************************
* Function name: bufferGetIncrement
* Purpose: Returns the buffer increment.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: The Buffer increment.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_int bufferGetIncrement(BufferPointer const pBuffer) {
	if (!pBuffer)
		return BUFFER_ERROR;
	return pBuffer->increment;
}


/************************************************************
* Function name: bufferGetString
* Purpose: Returns the pointer to String.
* Parameters:
*   pBuffer = pointer to Buffer Entity
*   pos = position to get the pointer
* Return value: Position of string char.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
cipher_chr* bufferGetSubString(BufferPointer const pBuffer, cipher_int pos) {
	if (!pBuffer || pos < 0 || pos > pBuffer->offset.addC)
		return NULL;
	return pBuffer->string + pos;
}


/************************************************************
* Function name: bufferGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   pBuffer = pointer to Buffer Entity
* Return value: Flags from Buffer.
* TO_DO: Adjust for your LANGUAGE.
**************************************************************/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
cipher_flg bufferGetFlags(BufferPointer const pBuffer) {
	if (!pBuffer)
		return (unsigned char)BUFFER_ERROR;
	return pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer)?(pBuffer->flags):(RT_FAIL_1))
#endif
