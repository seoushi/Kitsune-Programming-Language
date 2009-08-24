/*
 *  integer.c
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

#include "integer.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>

#include "object.h"
#include "boolean.h"
#include "string.h"
#include "float.h"


/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_Integer_isEqual(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_isGreaterThan(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_isLessThan(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_isGreaterThanOrEqual(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_isLessThanOrEqual(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_isNotEqual(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_add(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_sub(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_div(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_mul(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_pow(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_mod(Kitsune_Object* self, Kitsune_Object* intValue);
Kitsune_Object* Kitsune_Integer_sqrt(Kitsune_Object* self);
Kitsune_Object* Kitsune_Integer_toFloat(Kitsune_Object* self);	
Kitsune_Object* Kitsune_Integer_toInteger(Kitsune_Object* self);
Kitsune_Object* Kitsune_Integer_toString(Kitsune_Object* self);

/****************************
 * end prototypes
 ****************************/


Kitsune_Object* Kitsune_InitInteger()
{
	static Kitsune_Object* obj = NULL;
		
	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = Kitsune_SendMessage( Kitsune_InitObject(), "clone");

		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "==",		&Kitsune_Integer_isEqual);
		Kitsune_SendMessage(obj, "set-method", "equal?",	&Kitsune_Integer_isEqual);
		
		Kitsune_SendMessage(obj, "set-method", "!=",			&Kitsune_Integer_isNotEqual);
		Kitsune_SendMessage(obj, "set-method", "not-equal?",	&Kitsune_Integer_isNotEqual);
		
		Kitsune_SendMessage(obj, "set-method", ">",		&Kitsune_Integer_isGreaterThan);
		Kitsune_SendMessage(obj, "set-method", "<",		&Kitsune_Integer_isLessThan);
		Kitsune_SendMessage(obj, "set-method", ">=",	&Kitsune_Integer_isGreaterThanOrEqual);
		Kitsune_SendMessage(obj, "set-method", "<=",	&Kitsune_Integer_isLessThanOrEqual);
		
		Kitsune_SendMessage(obj, "set-method", "+",		&Kitsune_Integer_add);
		Kitsune_SendMessage(obj, "set-method", "-",		&Kitsune_Integer_sub);
		Kitsune_SendMessage(obj, "set-method", "/",		&Kitsune_Integer_div);
		Kitsune_SendMessage(obj, "set-method", "*",		&Kitsune_Integer_mul);	
		Kitsune_SendMessage(obj, "set-method", "^",		&Kitsune_Integer_pow);
		Kitsune_SendMessage(obj, "set-method", "%",		&Kitsune_Integer_mod);
		Kitsune_SendMessage(obj, "set-method", "sqrt",	&Kitsune_Integer_sqrt);

		Kitsune_SendMessage(obj, "set-method", "to-i",			&Kitsune_Integer_toInteger);
		Kitsune_SendMessage(obj, "set-method", "to-integer",	&Kitsune_Integer_toInteger);

		Kitsune_SendMessage(obj, "set-method", "to-f",			&Kitsune_Integer_toFloat);
		Kitsune_SendMessage(obj, "set-method", "to-float",		&Kitsune_Integer_toFloat);
		
		Kitsune_SendMessage(obj, "set-method", "to-s",		&Kitsune_Integer_toString);
		Kitsune_SendMessage(obj, "set-method", "to-string",	&Kitsune_Integer_toString);

		/* add integer value */
		Kitsune_SendMessage(obj, "set-value", "c-integer-value", 0);
	}
		
	return obj;
}


Kitsune_Object* Kitsune_MakeInteger(int integer)
{
	Kitsune_Object* obj = Kitsune_SendMessage( Kitsune_InitInteger(), "clone");
	
	Kitsune_SendMessage(obj, "set-value", "c-integer-value", integer);
	
	return obj;
}


Kitsune_Object* Kitsune_Integer_isEqual(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	if(selfValue == argValue)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Integer_isGreaterThan(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	if(selfValue > argValue)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Integer_isLessThan(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	if(selfValue < argValue)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Integer_isGreaterThanOrEqual(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	if(selfValue >= argValue)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Integer_isLessThanOrEqual(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	if(selfValue <= argValue)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Integer_isNotEqual(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	if(selfValue != argValue)
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Integer_add(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	return Kitsune_MakeInteger(selfValue + argValue);
}


Kitsune_Object* Kitsune_Integer_sub(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	return Kitsune_MakeInteger(selfValue - argValue);
}


Kitsune_Object* Kitsune_Integer_div(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	return Kitsune_MakeInteger(selfValue / argValue);
}


Kitsune_Object* Kitsune_Integer_mul(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	return Kitsune_MakeInteger(selfValue * argValue);
}


Kitsune_Object* Kitsune_Integer_pow(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	return Kitsune_MakeInteger( pow(selfValue, argValue) );
}


Kitsune_Object* Kitsune_Integer_mod(Kitsune_Object* self, Kitsune_Object* intValue)
{
	int argValue = (int)Kitsune_SendMessage(Kitsune_SendMessage(intValue, "to-i"), "c-integer-value");
	int selfValue = (int)Kitsune_SendMessage(self, "c-integer-value");
	
	return Kitsune_MakeInteger(selfValue % argValue);
}


Kitsune_Object* Kitsune_Integer_sqrt(Kitsune_Object* self)
{	
	return Kitsune_MakeFloat(sqrtf((int)Kitsune_SendMessage(self, "c-integer-value")));
}


Kitsune_Object* Kitsune_Integer_toFloat(Kitsune_Object* self)
{
	return Kitsune_MakeFloat((int)Kitsune_SendMessage(self, "c-integer-value"));
}


Kitsune_Object* Kitsune_Integer_toInteger(Kitsune_Object* self)
{
	return self;
}


Kitsune_Object* Kitsune_Integer_toString(Kitsune_Object* self)
{
	char 	buffer[128];
	int		strSize;
	char*	finalStr;
	
	strSize = sprintf(buffer, "%d", (int)Kitsune_SendMessage(self, "c-integer-value"));
	
	finalStr = GC_MALLOC(sizeof(char) * (strSize + 1) );
	strncpy(finalStr, buffer, strSize + 1);
	
	return Kitsune_MakeString(finalStr);
}

