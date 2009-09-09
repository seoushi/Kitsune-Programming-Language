/*
 *  core.cpp
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

#include "core.hpp"

using namespace kitsune;


Object::Object()
{
	parent = NULL;
}

bool Object::internString(std::string string, unsigned int* result)
{
	SlotMapItr itr = slots.find(string);
	
	if(itr == slots.end())
	{
		(*result) = 0;
		return false;
	}
	
	(*result) = (unsigned int)&itr->second;
	return true;
}


Object* Object::bind(std::string slotName, SlotType::Type* resultCode)
{
	unsigned int	messageId;
	Slot*			slot;
	Object*			curObj = this;
	
	while(curObj)
	{
		if(curObj->internString(slotName, &messageId))
		{
			slot = (Slot*)messageId;
			
			(*resultCode) = slot->type;
			
			if(slot->type == SlotType::noOp)
			{
				return NULL;
			}
			
			return (Object*)slot->data;
		}
		else
		{
			curObj = curObj->parent;
		}
	}

	// throw warning message on slot not found
	fprintf(stderr, "WARNING: could not find slot '%s in object %p \n", slotName.c_str(), this);
	
	(*resultCode) = SlotType::noOp;

	return NULL;
}


