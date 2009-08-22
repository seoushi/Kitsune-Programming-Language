/*
 *  array.c
 *  kitsune runtime
 *
 * Copyright (c) 2009, Seoushi Games
 * All rights reserved.
 *	
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Seoushi Games nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *	                                          
 * THIS SOFTWARE IS PROVIDED BY Seoushi Games ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Seoushi Games BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "array.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>

#include "object.h"
#include "integer.h"

/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_Array_at(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_append(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_slice(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_clear(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_remove(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_removeLast(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_count(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_add(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_set(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Array_each(Kitsune_Object* obj, ...);

/****************************
 * end prototypes
 ****************************/


Kitsune_Object* Kitsune_InitArray()
{
	static Kitsune_Object* obj = NULL;

	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = Kitsune_SendMessage( Kitsune_InitObject(), "clone");		
		
		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "at",			&Kitsune_Array_at);
		Kitsune_SendMessage(obj, "set-method", "append",		&Kitsune_Array_append);
		Kitsune_SendMessage(obj, "set-method", "slice",			&Kitsune_Array_slice);
		Kitsune_SendMessage(obj, "set-method", "clear",			&Kitsune_Array_clear);
		Kitsune_SendMessage(obj, "set-method", "remove",		&Kitsune_Array_remove);
		Kitsune_SendMessage(obj, "set-method", "remove-last",	&Kitsune_Array_removeLast);
		Kitsune_SendMessage(obj, "set-method", "count",			&Kitsune_Array_count);
		Kitsune_SendMessage(obj, "set-method", "add",			&Kitsune_Array_add);	
		Kitsune_SendMessage(obj, "set-method", "set",			&Kitsune_Array_set);
		Kitsune_SendMessage(obj, "set-method", "each",			&Kitsune_Array_each);

		/* add values */
		Kitsune_SendMessage(obj, "set-value", "c-array-value", 0);
		Kitsune_SendMessage(obj, "set-value", "c-count-value", 0);
	}

	return obj;
}


Kitsune_Object* Kitsune_MakeArray(int size, ...)
{
	va_list				args;
	int					i;
	Kitsune_Object**	array = (Kitsune_Object**)GC_MALLOC(sizeof(Kitsune_Object*) * size);
	
	va_start(args, size);
	
	for(i = 0; i < size; i++)
	{
		array[i] = va_arg(args, Kitsune_Object*);
	}
	
	return Kitsune_MakeArrayVec(size, array);
}

Kitsune_Object* Kitsune_MakeArrayVec(int size, Kitsune_Object** objects)
{
	Kitsune_Object* obj = Kitsune_SendMessage( Kitsune_InitArray(), "clone");

	Kitsune_SendMessage(obj, "set-value", "c-array-value", objects);
	Kitsune_SendMessage(obj, "set-value", "c-count-value", size);

	return obj;
}


Kitsune_Object* Kitsune_Array_at(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object**	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	int					index;
	
	va_start(args, obj);
	index = (int)Kitsune_SendMessage( Kitsune_SendMessage((Kitsune_Object*)va_arg(args, Kitsune_Object*), "to-i"), "c-integer-value");
	
	return array[index];
}


Kitsune_Object* Kitsune_Array_append(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		arg;
	Kitsune_Object**	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	Kitsune_Object**	argArray;
	int					i;
	int					arraySize;
	int					argArraySize;
	
	va_start(args, obj);
	arg = va_arg(args, Kitsune_Object*);
	
	argArray = (Kitsune_Object**)Kitsune_SendMessage(arg, "c-array-value");
	
	arraySize = (int)Kitsune_SendMessage(obj, "c-count-value");
	argArraySize = (int)Kitsune_SendMessage(arg, "c-count-value");

	/* resize array */
	GC_REALLOC(array, sizeof(Kitsune_Object*) * (arraySize + argArraySize));
	Kitsune_SendMessage(obj, "set-value", "c-count-value", arraySize + argArraySize);
	
	/* add new items from the argument */
	for(i = arraySize; i < (arraySize + argArraySize); i++)
	{
		array[i] = argArray[i - arraySize];
	}
	
	return NULL;
}


Kitsune_Object* Kitsune_Array_slice(Kitsune_Object* obj, ...)
{
	va_list				args;
	int					startIndex;
	int					endIndex;
	int					newArraySize;
	Kitsune_Object**	curArray = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	Kitsune_Object**	newArray;
	int					i;
	
	va_start(args, obj);
	startIndex = (int)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-i"), "c-integer-value");
	endIndex = (int)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-i"), "c-integer-value");
	newArraySize = endIndex - startIndex;
	
	/* make new array */
	newArray = GC_MALLOC(sizeof(Kitsune_Object*) * newArraySize);
	
	/* 'copy' items to the new array */
	for(i = startIndex; i <= endIndex; i++)
	{
		newArray[i - startIndex] = curArray[i];
	}
	
	return Kitsune_MakeArrayVec(newArraySize, newArray);
}


Kitsune_Object* Kitsune_Array_clear(Kitsune_Object* obj, ...)
{
	Kitsune_SendMessage(obj, "set-value", "c-array-value", 0);
	Kitsune_SendMessage(obj, "set-value", "c-count-value", 0);
	
	return NULL;
}


Kitsune_Object* Kitsune_Array_remove(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object**	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	int					arraySize = (int)Kitsune_SendMessage(obj, "c-count-value");
	int					index;
	int					i;
	
	va_start(args, obj);
	index = (int)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-i"), "c-integer-value");
	
	
	/* shift everything up one */
	for(i = index + 1; i < arraySize; i++)
	{
		array[i - 1] = array[i];
	}
	array[arraySize - 1] = 0;
	
	
	/* add change count */
	arraySize--;
	(int)Kitsune_SendMessage(obj, "set-value", "c-count-value", arraySize);
	
	
	/* resize array */
	GC_REALLOC(array, sizeof(Kitsune_Object*) * arraySize);
	
	return NULL;
}


Kitsune_Object* Kitsune_Array_removeLast(Kitsune_Object* obj, ...)
{
	Kitsune_Object** 	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	int 				count = (int)Kitsune_SendMessage(obj, "c-count-value");
	
	/* resize array */
	count--;
	array[count] = 0;

	GC_REALLOC(array, sizeof(Kitsune_Object*) * count);	
	Kitsune_SendMessage(obj, "set-value", "c-count-value", count);
	
	return NULL;
}


Kitsune_Object* Kitsune_Array_count(Kitsune_Object* obj, ...)
{
	return Kitsune_MakeInteger((int)Kitsune_SendMessage(obj, "c-count-value"));
}


Kitsune_Object* Kitsune_Array_add(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		arg;
	Kitsune_Object**	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	int					arraySize = (int)Kitsune_SendMessage(obj, "c-count-value");
	
	va_start(args, obj);
	arg = va_arg(args, Kitsune_Object*);
	
	
	/* resize array */
	arraySize++;
	GC_REALLOC(array, sizeof(Kitsune_Object*) * arraySize);

	/* add object and change count */
	array[arraySize - 1] = arg;
	(int)Kitsune_SendMessage(obj, "set-value", "c-count-value", arraySize);
	
	return NULL;
}


Kitsune_Object* Kitsune_Array_set(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		arg;
	Kitsune_Object**	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	int					index;
	
	va_start(args, obj);
	index = (int)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-i"), "c-integer-value");
	arg = va_arg(args, Kitsune_Object*);

	
	array[index] = arg;
	
	return NULL;
}


Kitsune_Object* Kitsune_Array_each(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object**	array = (Kitsune_Object**)Kitsune_SendMessage(obj, "c-array-value");
	int					arraySize = (int)Kitsune_SendMessage(obj, "c-count-value");
	int					i;
	Kitsune_FunctionPtr	function;
	
	va_start(args, obj);
	function = va_arg(args, Kitsune_FunctionPtr);
	
	
	for(i = 0; i < arraySize; i++)
	{
		function(obj, array[i]);
	}
	
	return NULL;
}

