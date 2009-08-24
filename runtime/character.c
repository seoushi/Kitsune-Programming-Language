/*
 *  character.c
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

#include "character.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>

#include "object.h"
#include "boolean.h"
#include "integer.h"
#include "float.h"
#include "string.h"

/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_Character_isEqual(Kitsune_Object* self, Kitsune_Object* character);
Kitsune_Object* Kitsune_Character_isGreaterThan(Kitsune_Object* self, Kitsune_Object* character);
Kitsune_Object* Kitsune_Character_isLessThan(Kitsune_Object* self, Kitsune_Object* character);
Kitsune_Object* Kitsune_Character_isGreaterThanOrEqual(Kitsune_Object* self, Kitsune_Object* character);
Kitsune_Object* Kitsune_Character_isLessThanOrEqual(Kitsune_Object* self, Kitsune_Object* character);
Kitsune_Object* Kitsune_Character_isNotEqual(Kitsune_Object* self, Kitsune_Object* character);
Kitsune_Object* Kitsune_Character_toUpper(Kitsune_Object* self);
Kitsune_Object* Kitsune_Character_toLower(Kitsune_Object* self);
Kitsune_Object* Kitsune_Character_toInteger(Kitsune_Object* self);
Kitsune_Object* Kitsune_Character_toFloat(Kitsune_Object* self);
Kitsune_Object* Kitsune_Character_toString(Kitsune_Object* self);

/****************************
 * end prototypes
 ****************************/


Kitsune_Object* Kitsune_InitCharacter()
{
	static Kitsune_Object* obj = NULL;
		
	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = Kitsune_SendMessage( Kitsune_InitObject(), "clone");

		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "==",		&Kitsune_Character_isEqual);
		Kitsune_SendMessage(obj, "set-method", "equal?",	&Kitsune_Character_isEqual);
		
		Kitsune_SendMessage(obj, "set-method", "!=",			&Kitsune_Character_isNotEqual);
		Kitsune_SendMessage(obj, "set-method", "not-equal?",	&Kitsune_Character_isNotEqual);
		
		Kitsune_SendMessage(obj, "set-method", ">",		&Kitsune_Character_isGreaterThan);
		Kitsune_SendMessage(obj, "set-method", "<",		&Kitsune_Character_isLessThan);
		Kitsune_SendMessage(obj, "set-method", ">=",	&Kitsune_Character_isGreaterThanOrEqual);
		Kitsune_SendMessage(obj, "set-method", "<=",	&Kitsune_Character_isLessThanOrEqual);
		
		Kitsune_SendMessage(obj, "set-method", "to-upper",		&Kitsune_Character_toUpper);
		Kitsune_SendMessage(obj, "set-method", "to-lower",		&Kitsune_Character_toLower);
		
		Kitsune_SendMessage(obj, "set-method", "to-i",	&Kitsune_Character_toInteger);
		Kitsune_SendMessage(obj, "set-method", "to-integer",	&Kitsune_Character_toInteger);

		Kitsune_SendMessage(obj, "set-method", "to-f",		&Kitsune_Character_toFloat);
		Kitsune_SendMessage(obj, "set-method", "to-float",	&Kitsune_Character_toFloat);
		
		Kitsune_SendMessage(obj, "set-method", "to-s",		&Kitsune_Character_toString);
		Kitsune_SendMessage(obj, "set-method", "to-string",	&Kitsune_Character_toString);

		/* add character value */
		Kitsune_SendMessage(obj, "set-value", "c-char-value", 0);
	}
		
	return obj;
}


Kitsune_Object* Kitsune_MakeCharacter(char character)
{
	Kitsune_Object* obj = Kitsune_SendMessage( Kitsune_InitCharacter(), "clone");
	
	Kitsune_SendMessage(obj, "set-value", "c-char-value", (int)character);
	
	return obj;
}


Kitsune_Object* Kitsune_Character_isEqual(Kitsune_Object* self, Kitsune_Object* character)
{
	if( ((int)Kitsune_SendMessage(self, "c-char-value")) == ((int)Kitsune_SendMessage(character, "c-char-value")))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Character_isGreaterThan(Kitsune_Object* self, Kitsune_Object* character)
{
	if( ((int)Kitsune_SendMessage(self, "c-char-value")) > ((int)Kitsune_SendMessage(character, "c-char-value")))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Character_isLessThan(Kitsune_Object* self, Kitsune_Object* character)
{
	if( ((int)Kitsune_SendMessage(self, "c-char-value")) < ((int)Kitsune_SendMessage(character, "c-char-value")))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Character_isGreaterThanOrEqual(Kitsune_Object* self, Kitsune_Object* character)
{
	if( ((int)Kitsune_SendMessage(self, "c-char-value")) >= ((int)Kitsune_SendMessage(character, "c-char-value")))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Character_isLessThanOrEqual(Kitsune_Object* self, Kitsune_Object* character)
{
	if( ((int)Kitsune_SendMessage(self, "c-char-value")) <= ((int)Kitsune_SendMessage(character, "c-char-value")))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Character_isNotEqual(Kitsune_Object* self, Kitsune_Object* character)
{
	if( ((int)Kitsune_SendMessage(self, "c-char-value")) != ((int)Kitsune_SendMessage(character, "c-char-value")))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Character_toUpper(Kitsune_Object* self)
{
	int	curChar = (int)Kitsune_SendMessage(self, "c-char-value");
	
	if( (curChar >= 'a') && (curChar <= 'z') )
	{
		curChar += 'A' - 'a';
	}
	
	return Kitsune_MakeCharacter(curChar);
}


Kitsune_Object* Kitsune_Character_toLower(Kitsune_Object* self)
{
	int	curChar = (int)Kitsune_SendMessage(self, "c-char-value");
	
	if( (curChar >= 'A') && (curChar <= 'Z') )
	{
		curChar += 'a' - 'A';
	}
	
	return Kitsune_MakeCharacter(curChar);
}


Kitsune_Object* Kitsune_Character_toInteger(Kitsune_Object* self)
{
	return Kitsune_MakeInteger((int)Kitsune_SendMessage(self, "c-char-value"));
}


Kitsune_Object* Kitsune_Character_toFloat(Kitsune_Object* self)
{
	return Kitsune_MakeFloat((int)Kitsune_SendMessage(self, "c-char-value"));
}


Kitsune_Object* Kitsune_Character_toString(Kitsune_Object* self)
{
	int		curChar = (int)Kitsune_SendMessage(self, "c-char-value");
	char*	newStr = (char*) GC_MALLOC(sizeof(char) * 2);
	
	newStr[0] = (char)curChar;
	newStr[1] = 0;
	
	return Kitsune_MakeString(newStr);
}
