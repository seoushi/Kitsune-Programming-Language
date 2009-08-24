/*
 * object.c
 * kitsune runtime
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


#include "object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>

#include "boolean.h"
#include "string.h"

/****************************
 * function prototypes
 ****************************/

Kitsune_Object* Kitsune_Object_hasSlot(Kitsune_Object* self, char* slotName);
Kitsune_Object* Kitsune_Object_setMethod(Kitsune_Object* self, char* methodName, Kitsune_FunctionPtr method);
Kitsune_Object* Kitsune_Object_setValue(Kitsune_Object* self, char* valueName, Kitsune_Object* value);
Kitsune_Object* Kitsune_Object_removeSlot(Kitsune_Object* self, char* slotName);
Kitsune_Object* Kitsune_Object_clone(Kitsune_Object* self);
Kitsune_Object* Kitsune_Object_toString(Kitsune_Object* self);
	
/****************************
 * end prototypes
 ****************************/

void Kitsune_Object_Helper_ResizeSlots(Kitsune_Object* obj)
{
	/* do slots even exist? */
	if(obj->numSlots == 1)
	{
		obj->slots = (Kitsune_Slot**) GC_MALLOC( sizeof(Kitsune_Slot*) );
		obj->slotKeys = (char**) GC_MALLOC( sizeof(char*) );
	}
	else
	{
		obj->slots = (Kitsune_Slot**) GC_REALLOC( obj->slots, sizeof(Kitsune_Slot*) * obj->numSlots );
		obj->slotKeys = (char**) GC_REALLOC( obj->slotKeys, sizeof(char*) * obj->numSlots );
	}
}


Kitsune_Object* Kitsune_InitObject()
{
	static Kitsune_Object*	obj = NULL;
		
	if(obj == NULL)
	{
		/*	initialize object	*/
		obj = (Kitsune_Object*)GC_MALLOC( sizeof(Kitsune_Object) );
		obj->parent = NULL;
		obj->numSlots = 0;
		
		/* add set method directly so we can use it */
		Kitsune_Object_setMethod(obj, "set-method", (Kitsune_FunctionPtr)&Kitsune_Object_setMethod);
			
		/*	add methods	*/
		Kitsune_SendMessage(obj, "set-method", "has-slot",	&Kitsune_Object_hasSlot);
		
		Kitsune_SendMessage(obj, "set-method", "set-value",	&Kitsune_Object_setValue);
		
		Kitsune_SendMessage(obj, "set-method", "remove-slot",	&Kitsune_Object_removeSlot);
		Kitsune_SendMessage(obj, "set-method", "clone",			&Kitsune_Object_clone);
		
		Kitsune_SendMessage(obj, "set-method", "to-s",		&Kitsune_Object_toString);
		Kitsune_SendMessage(obj, "set-method", "to-string",	&Kitsune_Object_toString);
	}
		
	return obj;
}


Kitsune_Object* Kitsune_Object_hasSlot(Kitsune_Object* self, char* slotName)
{
	Kitsune_Object*	curObj = self;

	while(curObj)
	{
		if(Kitsune_InternString(curObj, slotName, NULL))
		{
			return Kitsune_MakeBoolean(true);
		}
		else
		{
			curObj = curObj->parent;
		}
	}	
	
	return Kitsune_MakeBoolean(false);
}
	

Kitsune_Object* Kitsune_Object_setMethod(Kitsune_Object* self, char* methodName, Kitsune_FunctionPtr method)
{
	int					i;
	Kitsune_Slot*		newSlot;
	

	/* try and replace current slots first */
	for(i = 0; i < self->numSlots; i++)
	{
		if( strcmp(methodName, self->slotKeys[i]) == 0 )
		{
			self->slots[i]->type = KITSUNE_SLOT_TYPE_METHOD;
			self->slots[i]->data = method;
			
			return NULL;
		}
	}
	

	/* add new slot */
	self->numSlots++;
	Kitsune_Object_Helper_ResizeSlots(self);
	
	/* set the new data */
	newSlot = (Kitsune_Slot*)GC_MALLOC( sizeof(Kitsune_Slot) );
	newSlot->type = KITSUNE_SLOT_TYPE_METHOD;
	newSlot->data = method;
	
	self->slots[self->numSlots - 1] = newSlot;
	self->slotKeys[self->numSlots -1] = methodName;
	
	return NULL;
}
	

Kitsune_Object* Kitsune_Object_setValue(Kitsune_Object* self, char* valueName, Kitsune_Object* value)
{
	int					i;
	Kitsune_Slot*		newSlot;

	/* try and replace current slots first */
	for(i = 0; i < self->numSlots; i++)
	{
		if( strcmp(valueName, self->slotKeys[i]) == 0 )
		{
			self->slots[i]->type = KITSUNE_SLOT_TYPE_VALUE;
			self->slots[i]->data = value;
			
			return NULL;
		}
	}
	

	/* add new slot */
	self->numSlots++;
	Kitsune_Object_Helper_ResizeSlots(self);
	
	/* set the new data */
	newSlot = (Kitsune_Slot*)GC_MALLOC( sizeof(Kitsune_Slot) );
	newSlot->type = KITSUNE_SLOT_TYPE_VALUE;
	newSlot->data = value;
	
	self->slots[self->numSlots - 1] = newSlot;
	self->slotKeys[self->numSlots -1] = valueName;
	
	return NULL;
}
	
	
Kitsune_Object* Kitsune_Object_removeSlot(Kitsune_Object* self, char* slotName)
{
	int					i;
	Kitsune_Object*		curObj;
	Kitsune_Slot*		newSlot;
	
	
	/* see if slot exits to remove */
	for(i = 0; i < self->numSlots; i++)
	{
		if( strcmp(slotName, self->slotKeys[i]) == 0 )
		{
			self->slots[i]->type = KITSUNE_SLOT_TYPE_NO_OP;
			self->slots[i]->data = NULL;
			
			return NULL;
		}
	}
	
	/* see if parent has the slot */
	curObj = self->parent;
	
	while(curObj)
	{
		if(Kitsune_InternString(curObj, slotName, NULL))
		{
			/* parent has the slot that needs to be removed, we will cover it up by adding
				a no op in the top most object */
			self->numSlots++;
			Kitsune_Object_Helper_ResizeSlots(self);
			
			/* set the new data */
			newSlot = (Kitsune_Slot*)GC_MALLOC( sizeof(Kitsune_Slot) );
			newSlot->type = KITSUNE_SLOT_TYPE_NO_OP;
			newSlot->data = NULL;
	
			self->slots[self->numSlots - 1] = newSlot;
			self->slotKeys[self->numSlots -1] = slotName;
	
			return NULL;
		}
		else
		{
			curObj = curObj->parent;
		}
	}
	
	return NULL;
}
	
	
Kitsune_Object* Kitsune_Object_clone(Kitsune_Object* self)
{
	Kitsune_Object* newObj = (Kitsune_Object*) GC_MALLOC( sizeof(Kitsune_Object) );
	
	newObj->parent = self;
	newObj->numSlots = 0;
	
	return newObj;
}

Kitsune_Object* Kitsune_Object_toString(Kitsune_Object* self)
{
	char string[24];
	
	sprintf(string, "object @ %p", self);
	
	return Kitsune_MakeString(string);
}

