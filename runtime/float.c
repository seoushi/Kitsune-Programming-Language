/*
 *  float.c
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

#include "float.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>

#include "object.h"
#include "boolean.h"
#include "string.h"
#include "integer.h"


/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_Float_isEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_isGreaterThan(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_isLessThan(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_isGreaterThanOrEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_isLessThanOrEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_isNotEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_add(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_sub(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_div(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_mul(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_pow(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_sqrt(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_toFloat(Kitsune_Object* obj, ...);	
Kitsune_Object* Kitsune_Float_toInteger(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Float_toString(Kitsune_Object* obj, ...);

/****************************
 * end prototypes
 ****************************/


Kitsune_Object* Kitsune_InitFloat()
{
	static Kitsune_Object* obj = NULL;
	float*	tmpFloat;	
		
	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = Kitsune_SendMessage( Kitsune_InitObject(), "clone");

		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "==",		&Kitsune_Float_isEqual);
		Kitsune_SendMessage(obj, "set-method", "equal?",	&Kitsune_Float_isEqual);
		
		Kitsune_SendMessage(obj, "set-method", "!=",			&Kitsune_Float_isNotEqual);
		Kitsune_SendMessage(obj, "set-method", "not-equal?",	&Kitsune_Float_isNotEqual);
		
		Kitsune_SendMessage(obj, "set-method", ">",		&Kitsune_Float_isGreaterThan);
		Kitsune_SendMessage(obj, "set-method", "<",		&Kitsune_Float_isLessThan);
		Kitsune_SendMessage(obj, "set-method", ">=",	&Kitsune_Float_isGreaterThanOrEqual);
		Kitsune_SendMessage(obj, "set-method", "<=",	&Kitsune_Float_isLessThanOrEqual);
		
		Kitsune_SendMessage(obj, "set-method", "+",		&Kitsune_Float_add);
		Kitsune_SendMessage(obj, "set-method", "-",		&Kitsune_Float_sub);
		Kitsune_SendMessage(obj, "set-method", "/",		&Kitsune_Float_div);
		Kitsune_SendMessage(obj, "set-method", "*",		&Kitsune_Float_mul);	
		Kitsune_SendMessage(obj, "set-method", "^",		&Kitsune_Float_pow);
		Kitsune_SendMessage(obj, "set-method", "sqrt",	&Kitsune_Float_sqrt);

		Kitsune_SendMessage(obj, "set-method", "to-i",			&Kitsune_Float_toInteger);
		Kitsune_SendMessage(obj, "set-method", "to-integer",	&Kitsune_Float_toInteger);

		Kitsune_SendMessage(obj, "set-method", "to-f",			&Kitsune_Float_toFloat);
		Kitsune_SendMessage(obj, "set-method", "to-float",		&Kitsune_Float_toFloat);
		
		Kitsune_SendMessage(obj, "set-method", "to-s",		&Kitsune_Float_toString);
		Kitsune_SendMessage(obj, "set-method", "to-string",	&Kitsune_Float_toString);

		/* add float value */
		tmpFloat = (float*)GC_MALLOC( sizeof(float) );
		(*tmpFloat) = 0;
		Kitsune_SendMessage(obj, "set-value", "c-float-value", tmpFloat);
	}
		
	return obj;
}


Kitsune_Object* Kitsune_MakeFloat(float floatingPoint)
{
	Kitsune_Object* obj = Kitsune_SendMessage( Kitsune_InitFloat(), "clone");
	float* tmpFloat = (float*)GC_MALLOC( sizeof(float) );
	(*tmpFloat) = floatingPoint;
	
	Kitsune_SendMessage(obj, "set-value", "c-float-value", tmpFloat);

	return obj;
}


Kitsune_Object* Kitsune_Float_isEqual(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	if((*selfValue) == (*argValue))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Float_isGreaterThan(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	if((*selfValue) > (*argValue))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Float_isLessThan(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	if((*selfValue) < (*argValue))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Float_isGreaterThanOrEqual(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	if((*selfValue) >= (*argValue))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Float_isLessThanOrEqual(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	if((*selfValue) <= (*argValue))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Float_isNotEqual(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	if((*selfValue) != (*argValue))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}


Kitsune_Object* Kitsune_Float_add(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	return Kitsune_MakeFloat((*selfValue) + (*argValue));
}


Kitsune_Object* Kitsune_Float_sub(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	return Kitsune_MakeFloat((*selfValue) - (*argValue));
}


Kitsune_Object* Kitsune_Float_div(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	return Kitsune_MakeFloat((*selfValue) / (*argValue));
}


Kitsune_Object* Kitsune_Float_mul(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	return Kitsune_MakeFloat((*selfValue) * (*argValue));
}


Kitsune_Object* Kitsune_Float_pow(Kitsune_Object* obj, ...)
{
	va_list	args;
	float*	argValue;
	float*	selfValue;
	
	va_start(args, obj);
	argValue = (float*)Kitsune_SendMessage(Kitsune_SendMessage(va_arg(args, Kitsune_Object*), "to-f"), "c-float-value");
	selfValue = (float*)Kitsune_SendMessage(obj, "c-float-value");
	
	return Kitsune_MakeFloat( pow((*selfValue), (*argValue)) );
}


Kitsune_Object* Kitsune_Float_sqrt(Kitsune_Object* obj, ...)
{	
	return Kitsune_MakeFloat(sqrtf( (*(float*)Kitsune_SendMessage(obj, "c-float-value"))));
}


Kitsune_Object* Kitsune_Float_toFloat(Kitsune_Object* obj, ...)
{
	return obj;
}


Kitsune_Object* Kitsune_Float_toInteger(Kitsune_Object* obj, ...)
{
	return Kitsune_MakeInteger((*(float*)Kitsune_SendMessage(obj, "c-float-value")));
}


Kitsune_Object* Kitsune_Float_toString(Kitsune_Object* obj, ...)
{
	char 	buffer[128];
	int		strSize;
	char*	finalStr;
	
	strSize = sprintf(buffer, "%f", (*(float*)Kitsune_SendMessage(obj, "c-float-value")));
	
	finalStr = GC_MALLOC(sizeof(char) * (strSize + 1) );
	strncpy(finalStr, buffer, strSize + 1);
	
	return Kitsune_MakeString(finalStr);
}

