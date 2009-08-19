/*
 *  boolean.c
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
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "boolean.h"

#include <string.h>
#include <stdlib.h>
#include <gc/gc.h>

#include "object.h"
#include "string.h"


/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_Boolean_isEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Boolean_isNotEqual(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Boolean_and(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Boolean_or(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Boolean_not(Kitsune_Object* obj, ...);
Kitsune_Object* Kitsune_Boolean_toString(Kitsune_Object* obj, ...);

/****************************
 * end prototypes
 ****************************/

Kitsune_Object* Kitsune_InitBoolean()
{
	static Kitsune_Object* obj = NULL;
		
	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = Kitsune_SendMessage( Kitsune_InitObject(), "clone");

		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "==",		&Kitsune_Boolean_isEqual);
		Kitsune_SendMessage(obj, "set-method", "equal?",	&Kitsune_Boolean_isEqual);
		
		Kitsune_SendMessage(obj, "set-method", "!=",			&Kitsune_Boolean_isNotEqual);
		Kitsune_SendMessage(obj, "set-method", "not-equal?",	&Kitsune_Boolean_isNotEqual);
		
		Kitsune_SendMessage(obj, "set-method", "&&",	&Kitsune_Boolean_and);
		Kitsune_SendMessage(obj, "set-method", "and",	&Kitsune_Boolean_and);
		
		Kitsune_SendMessage(obj, "set-method", "||",	&Kitsune_Boolean_or);
		Kitsune_SendMessage(obj, "set-method", "or",	&Kitsune_Boolean_or);
		
		Kitsune_SendMessage(obj, "set-method", "!",		&Kitsune_Boolean_not);
		Kitsune_SendMessage(obj, "set-method", "not",	&Kitsune_Boolean_not);
		
		Kitsune_SendMessage(obj, "set-method", "to-string",	&Kitsune_Boolean_toString);
		Kitsune_SendMessage(obj, "set-method", "to-s",		&Kitsune_Boolean_toString);
		
		/* add bool value */
		Kitsune_SendMessage(obj, "set-value", "c-bool-value", false);
	}
		
	return obj;
}

Kitsune_Object* Kitsune_MakeBoolean(bool value)
{
	Kitsune_Object* obj = Kitsune_SendMessage( Kitsune_InitBoolean(), "clone");
	
	Kitsune_SendMessage(obj, "set-value", "c-bool-value", value);
	
	return obj;
}

	
Kitsune_Object* Kitsune_Boolean_isEqual(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		boolArg;
	
	
	va_start(args, obj);
	boolArg = va_arg(args, Kitsune_Object*);
	
	
	if(Kitsune_SendMessage(obj, "c-bool-value") == Kitsune_SendMessage(boolArg, "c-bool-value"))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}

Kitsune_Object* Kitsune_Boolean_isNotEqual(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		boolArg;
	
	
	va_start(args, obj);
	boolArg = va_arg(args, Kitsune_Object*);
	
	
	if(Kitsune_SendMessage(obj, "c-bool-value") != Kitsune_SendMessage(boolArg, "c-bool-value"))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}

Kitsune_Object* Kitsune_Boolean_and(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		boolArg;
	
	
	va_start(args, obj);
	boolArg = va_arg(args, Kitsune_Object*);
	
	
	if(Kitsune_SendMessage(obj, "c-bool-value") && Kitsune_SendMessage(boolArg, "c-bool-value"))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}

Kitsune_Object* Kitsune_Boolean_or(Kitsune_Object* obj, ...)
{
	va_list				args;
	Kitsune_Object*		boolArg;
	
	
	va_start(args, obj);
	boolArg = va_arg(args, Kitsune_Object*);
	
	
	if(Kitsune_SendMessage(obj, "c-bool-value") || Kitsune_SendMessage(boolArg, "c-bool-value"))
	{
		return Kitsune_MakeBoolean(true);
	}
	
	return Kitsune_MakeBoolean(false);
}

Kitsune_Object* Kitsune_Boolean_not(Kitsune_Object* obj, ...)
{
	return Kitsune_MakeBoolean(!Kitsune_SendMessage(obj, "c-bool-value"));
}

Kitsune_Object* Kitsune_Boolean_toString(Kitsune_Object* obj, ...)
{
	if(Kitsune_SendMessage(obj, "c-bool-value"))
	{
		return Kitsune_MakeString("true");
	}

	return Kitsune_MakeString("false");
}

