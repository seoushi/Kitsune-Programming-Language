/*
 *  string.c
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

#include "string.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>

#include "object.h"
#include "boolean.h"
#include "integer.h"
#include "float.h"
#include "character.h"

/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_String_isEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_isGreaterThan(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_isLessThan(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_isGreaterThanOrEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_isLessThanOrEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_isNotEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_length(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_charAt(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_includes(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_reverse(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_replace(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_remove(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_toInteger(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_toFloat(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_capitalize(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_toUpper(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_toLower(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_print(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_printLn(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_append(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_subString(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_split(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_String_toString(Kitsune_Object* obj, ...);

/****************************
 * end prototypes
 ****************************/


Kitsune_Object* Kitsune_InitString()
{
	static Kitsune_Object* obj = NULL;
		
	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = Kitsune_SendMessage( Kitsune_InitObject(), "clone");

		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "==",		&Kitsune_String_isEqual);
		Kitsune_SendMessage(obj, "set-method", "equal?",	&Kitsune_String_isEqual);
		
		Kitsune_SendMessage(obj, "set-method", "!=",			&Kitsune_String_isNotEqual);
		Kitsune_SendMessage(obj, "set-method", "not-equal?",	&Kitsune_String_isNotEqual);
		
		Kitsune_SendMessage(obj, "set-method", ">",		&Kitsune_String_isGreaterThan);
		Kitsune_SendMessage(obj, "set-method", "<",		&Kitsune_String_isLessThan);
		Kitsune_SendMessage(obj, "set-method", ">=",	&Kitsune_String_isGreaterThanOrEqual);
		Kitsune_SendMessage(obj, "set-method", "<=",	&Kitsune_String_isLessThanOrEqual);
		
		Kitsune_SendMessage(obj, "set-method", "length",	&Kitsune_String_length);
		Kitsune_SendMessage(obj, "set-method", "charAt",	&Kitsune_String_charAt);
		Kitsune_SendMessage(obj, "set-method", "includes?",	&Kitsune_String_includes);
		Kitsune_SendMessage(obj, "set-method", "reverse",	&Kitsune_String_reverse);	
		Kitsune_SendMessage(obj, "set-method", "replace",	&Kitsune_String_replace);
		Kitsune_SendMessage(obj, "set-method", "remove",	&Kitsune_String_remove);

		Kitsune_SendMessage(obj, "set-method", "to-i",	&Kitsune_String_toInteger);
		Kitsune_SendMessage(obj, "set-method", "to-integer",	&Kitsune_String_toInteger);

		Kitsune_SendMessage(obj, "set-method", "to-f",		&Kitsune_String_toFloat);
		Kitsune_SendMessage(obj, "set-method", "to-float",	&Kitsune_String_toFloat);
		
		Kitsune_SendMessage(obj, "set-method", "capitalize",	&Kitsune_String_capitalize);
		Kitsune_SendMessage(obj, "set-method", "to-upper",		&Kitsune_String_toUpper);
		Kitsune_SendMessage(obj, "set-method", "to-lower",		&Kitsune_String_toLower);
		
		Kitsune_SendMessage(obj, "set-method", "print",		&Kitsune_String_print);
		Kitsune_SendMessage(obj, "set-method", "print-line",	&Kitsune_String_printLn);
		
		Kitsune_SendMessage(obj, "set-method", "++",		&Kitsune_String_append);
		Kitsune_SendMessage(obj, "set-method", "append",	&Kitsune_String_append);
		
		Kitsune_SendMessage(obj, "set-method", "sub-string",	&Kitsune_String_subString);
		Kitsune_SendMessage(obj, "set-method", "split",			&Kitsune_String_split);
		
		Kitsune_SendMessage(obj, "set-method", "to-s",		&Kitsune_String_toString);
		Kitsune_SendMessage(obj, "set-method", "to-string",	&Kitsune_String_toString);

		/* add string value */
		Kitsune_SendMessage(obj, "set-value", "c-string-value", "");
	}
		
	return obj;
}


Kitsune_Object* Kitsune_MakeString(char* string)
{
	Kitsune_Object* obj = Kitsune_SendMessage( Kitsune_InitString(), "clone");
	
	Kitsune_SendMessage(obj, "set-value", "c-string-value", string);
	
	return obj;
}


Kitsune_Object* Kitsune_String_isEqual(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strcmp( (char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value") ) == 0)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_String_isGreaterThan(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strcmp( (char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value") ) > 0)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_String_isLessThan(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strcmp( (char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value") ) < 0)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_String_isGreaterThanOrEqual(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strcmp( (char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value") ) >= 0)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_String_isLessThanOrEqual(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strcmp( (char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value") ) <= 0)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_String_isNotEqual(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strcmp( (char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value") ) != 0)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_String_length(Kitsune_Object* obj, ...)
{
	return Kitsune_MakeInteger(strlen((char*)Kitsune_SendMessage(obj, "c-string-value")));
}

Kitsune_Object* Kitsune_String_charAt(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	int 			index;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	index = (int)Kitsune_SendMessage(argObj, "c-integer-value");
	
	return Kitsune_MakeCharacter( ((char*)Kitsune_SendMessage(obj, "c-string-value"))[index]);
}

Kitsune_Object* Kitsune_String_includes(Kitsune_Object* obj, ...)
{
	va_list			args;
	Kitsune_Object*	argObj;
	
	va_start(args, obj);
	argObj = va_arg(args, Kitsune_Object*);
	
	
	if(strstr(	(char*)Kitsune_SendMessage(obj, "c-string-value"), 
				(char*)Kitsune_SendMessage(argObj, "c-string-value")) != 0)
	{
		return Kitsune_MakeBoolean(true);
	}			
	
	return Kitsune_MakeBoolean(false);
}

Kitsune_Object* Kitsune_String_reverse(Kitsune_Object* obj, ...)
{
	int 	pos;
	int		idx;
	char*	curStr;
	char*	newStr;
	
	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	newStr = (char*)GC_MALLOC( sizeof(char) * (strlen(curStr) + 1));
	newStr[strlen(curStr)] = 0;
	idx = 0;
	
	for(pos = strlen(curStr) - 1; pos > -1; pos--)
	{
		newStr[idx] = curStr[pos];
		idx++;
	}
				
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_replace(Kitsune_Object* obj, ...)
{
	va_list			args;
	char*			searchStr;
	char*			replaceStr;
	char*			curStr;
	char*			newStr;
	int				i;
	int				count = 0;
	size_t			newLen;
	size_t			oldLen;
	
	va_start(args, obj);
	searchStr = (char*)Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "c-string-value");
	replaceStr = (char*)Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "c-string-value");
	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	
	/* calculate the size of the new string */
	newLen = strlen(replaceStr);
	oldLen = strlen(searchStr);
	
	for(i = 0; curStr[i] != 0; i++)
	{
		if( strstr(&curStr[i], searchStr) == &curStr[i] )
		{
			count++;
			i += oldLen - 1;
		}
	}
	
	/* make new string */		
	newStr = (char*)GC_MALLOC(sizeof(char) * ((count * (newLen - oldLen)) + strlen(curStr) + 1));

	i = 0;
	while (*curStr)
	{
		if(strstr(curStr, searchStr) == curStr)
		{
			strcpy(&newStr[i], replaceStr);
			i += newLen;
			curStr += oldLen;
		}
		else
		{
			newStr[i++] = *curStr++;
			newStr[i] = 0;
		}
	}
	
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_remove(Kitsune_Object* obj, ...)
{
	va_list args;
	va_start(args, obj);
	
	return Kitsune_String_replace(obj, va_arg(args, Kitsune_Object*), Kitsune_MakeString(""));
}

Kitsune_Object* Kitsune_String_toInteger(Kitsune_Object* obj, ...)
{
	char* string = (char*)Kitsune_SendMessage(obj, "c-string-value");
	
	return Kitsune_MakeInteger(atoi(string));
}

Kitsune_Object* Kitsune_String_toFloat(Kitsune_Object* obj, ...)
{
	char* string = (char*)Kitsune_SendMessage(obj, "c-string-value");
	
	return Kitsune_MakeFloat(atof(string));
}

Kitsune_Object* Kitsune_String_capitalize(Kitsune_Object* obj, ...)
{
	char*	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	int		i;
	char*	newStr;
	
	newStr = GC_MALLOC(sizeof(char) * (strlen(curStr) + 1));
	strcpy(newStr, curStr);

	for(i = 1; i < (strlen(curStr) - 1); i++)
	{
		if((curStr[i - 1] == ' ') || (i == 1))
		{
			if( (curStr[i] >= 'a') && (curStr[i] <= 'z') )
			{
				newStr[i] += 'A' - 'a';
			}
		}
	}
	
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_toUpper(Kitsune_Object* obj, ...)
{
	char*	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	int		i;
	char*	newStr;
	
	newStr = GC_MALLOC(sizeof(char) * (strlen(curStr) + 1));
	strcpy(newStr, curStr);

	for(i = 0; i < strlen(curStr); i++)
	{
		if( (curStr[i] >= 'a') && (curStr[i] <= 'z') )
		{
			newStr[i] += 'A' - 'a';
		}
	}
	
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_toLower(Kitsune_Object* obj, ...)
{
	char*	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	int		i;
	char*	newStr;
	
	newStr = GC_MALLOC(sizeof(char) * (strlen(curStr) + 1));
	strcpy(newStr, curStr);

	for(i = 0; i < strlen(curStr); i++)
	{
		if( (curStr[i] >= 'A') && (curStr[i] <= 'Z') )
		{
			newStr[i] += 'a' - 'A';
		}
	}
	
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_print(Kitsune_Object* obj, ...)
{
	char* string = (char*)Kitsune_SendMessage(obj, "c-string-value");

	printf("%s",string);
	
	return NULL;
}

Kitsune_Object* Kitsune_String_printLn(Kitsune_Object* obj, ...)
{
	char* string = (char*)Kitsune_SendMessage(obj, "c-string-value");

	printf("%s\n", string);
	
	return NULL;
}

Kitsune_Object* Kitsune_String_append(Kitsune_Object* obj, ...)
{
	va_list	args;
	char*	newStr;
	char*	appendingString;
	char*	curStr;
	
	va_start(args, obj);
	appendingString = (char*)Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "c-string-value");
	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	
	newStr = (char*)GC_MALLOC(sizeof(char) * (strlen(curStr) + strlen(appendingString) + 1));
	strcpy(newStr, curStr);
	newStr = strcat(newStr, appendingString);
	
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_subString(Kitsune_Object* obj, ...)
{
	va_list	args;
	char*	curStr;
	char*	newStr;
	int		fromIdx;
	int		toIdx;
	int		length;
	
	va_start(args, obj);
	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	fromIdx = (int)Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "c-int-value");
	toIdx = (int)Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "c-int-value");
	
	length = toIdx - fromIdx;
	newStr = (char*)GC_MALLOC(sizeof(char) * (length + 1));

	strncpy(newStr, &curStr[fromIdx], length + 1);
	
	return Kitsune_MakeString(newStr);
}

Kitsune_Object* Kitsune_String_split(Kitsune_Object* obj, ...)
{
	va_list				args;
	char*				curStr;
	char*				delimStr;
	char*				tmpStr;
	int					arraySize = 0;
	int					curArrayIdx = 0;
	int					i;
	int					prevFind = 0;
	Kitsune_Object**	array;

	
	va_start(args, obj);
	curStr = (char*)Kitsune_SendMessage(obj, "c-string-value");
	delimStr = (char*)Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "c-string-value");
	
	
	/* calculate the size of array */
	for(i = 0; curStr[i] != 0; i++)
	{
		if( strstr(&curStr[i], delimStr) == &curStr[i] )
		{
			arraySize++;
			i += strlen(delimStr) - 1;
		}
	}
	
	array = GC_MALLOC(sizeof(Kitsune_Object*) * arraySize);
	
	/* fill array */
	for(i = 0; curStr[i] != 0; i++)
	{
		if( strstr(&curStr[i], delimStr) == &curStr[i] )
		{
			tmpStr = (char*) GC_MALLOC(sizeof(char) * (i -  prevFind + 1));
			strncpy(tmpStr, &curStr[prevFind], i - prevFind);
			
			prevFind = i + strlen(delimStr);
			
			array[curArrayIdx] = Kitsune_MakeString(tmpStr);
			curArrayIdx++;

			i += strlen(delimStr) - 1;
		}
	}
	
	if(curArrayIdx < (arraySize - 1))
	{
		tmpStr = GC_MALLOC(sizeof(char) * (strlen(curStr) - prevFind + 1));
		strncpy(tmpStr, &curStr[prevFind], strlen(curStr) - prevFind);
	}
	
	return Kitsune_MakeArrayVec(arraySize, array);
}

Kitsune_Object* Kitsune_String_toString(Kitsune_Object* obj, ...)
{
	return obj;
}

