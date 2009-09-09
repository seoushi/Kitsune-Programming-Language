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


#include "object.hpp"
#include "boolean.hpp"

#include <string>
#include <sstream>

using namespace kitsune;

/****************************
 * function prototypes
 ****************************/

Object* Object_hasSlot		(Object* __CLOSURE__, Object* self,	std::string slotName);
Object* Object_setMethod	(Object* __CLOSURE__, Object* self,	std::string methodName,	FunctionPtr method);
Object* Object_setValue		(Object* __CLOSURE__, Object* self,	std::string valueName,	Object* value);
Object* Object_removeSlot	(Object* __CLOSURE__, Object* self,	std::string slotName);
Object* Object_clone		(Object* __CLOSURE__, Object* self);
Object* Object_toString		(Object* __CLOSURE__, Object* self);

/****************************
 * end prototypes
 ****************************/


Object* InitObject()
{
	static Object* obj = NULL;
		
	if(obj == NULL)
	{		
		// initialize object
		obj = new Object();
		obj->parent = NULL;
		
		/* add set method directly so we can use it */
		Object_setMethod(NULL, obj, "set-method", (FunctionPtr)&Object_setMethod);
		
		/*	add methods	*/
		Kitsune_SendMessage(NULL, obj, "set-method", "has-slot",	&Object_hasSlot);
		
		Kitsune_SendMessage(NULL, obj, "set-method", "set-value",	&Object_setValue);
		
		Kitsune_SendMessage(NULL, obj, "set-method", "remove-slot",	&Object_removeSlot);
		Kitsune_SendMessage(NULL, obj, "set-method", "clone",		&Object_clone);
		
		Kitsune_SendMessage(NULL, obj, "set-method", "to-s",		&Object_toString);
		Kitsune_SendMessage(NULL, obj, "set-method", "to-string",	&Object_toString);
	}
		
	return obj;
}


Object* Object_hasSlot(Object* __CLOSURE__, Object* self, std::string slotName)
{
	Object*	curObj = self;
	unsigned int tmp;

	while(curObj)
	{
		if(curObj->internString(slotName, &tmp))
		{
			return MakeBoolean(true);
		}
		else
		{
			curObj = curObj->parent;
		}
	}	
	
	return MakeBoolean(false);
}
	

Object* Object_setMethod(Object* __CLOSURE__, Object* self, std::string methodName, FunctionPtr method)
{
	// try and replace current slots first
	SlotMapItr itr = self->slots.find(methodName);
	
	if(itr != self->slots.end())
	{
		itr->second.type = SlotType::method;
		itr->second.data = (void*)method;
		
		return NULL;
	}

	// add new slot
	Slot newSlot;
	newSlot.type = SlotType::method;
	newSlot.data = (void*)method;
	
	self->slots[methodName] = newSlot;
	
	return NULL;
}


Object* Object_setValue(Object* __CLOSURE__, Object* self, std::string valueName, Object* value)
{
	// try and replace current slots first
	SlotMapItr itr = self->slots.find(valueName);
	
	if(itr != self->slots.end())
	{
		itr->second.type = SlotType::value;
		itr->second.data = value;
		
		return NULL;
	}
	
	// add new slot
	Slot newSlot;
	newSlot.type = SlotType::value;
	newSlot.data = value;
	
	self->slots[valueName] = newSlot;
	
	return NULL;
}
	
	
Object* Object_removeSlot(Object* __CLOSURE__, Object* self, std::string slotName)
{
	// try and remove from self first
	SlotMapItr itr = self->slots.find(slotName);
	
	if(itr != self->slots.end())
	{
		itr->second.type = SlotType::noOp;
		itr->second.data = NULL;
		
		return NULL;
	}
	
	
	// see if parent has the slot
	Object* curObj = self->parent;
	
	while(curObj)
	{
		itr = curObj->slots.find(slotName);
		
		if(itr != self->slots.end())
		{
			Slot newSlot;
			newSlot.type = SlotType::noOp;
			newSlot.data = NULL;
			
			self->slots[slotName] = newSlot;
			
			return NULL;
		}

		curObj = curObj->parent;
	}
	
	return NULL;
}
	
	
Object* Object_clone(Object* __CLOSURE__, Object* self)
{
	Object* newObj = new Object();
	newObj->parent = self;
	
	return newObj;
}

Object* Object_toString(Object* __CLOSURE__, Object* self)
{
	std::stringstream ss;
	ss << "object @ " << (int)self;
	
	return MakeString(ss.str());
}

