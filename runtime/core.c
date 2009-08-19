/*
 *  core.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"


bool Kitsune_InternString(Kitsune_Object* obj, char* string, unsigned int* result)
{
	/* TODO: caching the lookup, would make things faster */
	unsigned int i;
	
	for(i = 0; i < obj->numSlots; i++)
	{
		if( strcmp(string, obj->slotKeys[i]) == 0 )
		{
			if(result)
			{
				(*result) = i;
				return true;
			}
		}
	}
	
	/*
	This isn't always an error or warning, I'll keep it here incase I change my mind
	fprintf(stderr, "WARNING: Could not find slot '%s in object %p", string, obj);
	*/
	
	return false;
}

Kitsune_Object* Kitsune_SendMessage(Kitsune_Object* obj, char* slotName, ... )
{
	unsigned int	messageId;
	Kitsune_Slot*		slot;	
	Kitsune_Object*	curObj = obj;
	va_list			args;
	
	while(curObj)
	{
		if(Kitsune_InternString(curObj, slotName, &messageId))
		{
			slot = curObj->slots[messageId];
			
			switch(slot->type)
			{
				case KITSUNE_SLOT_TYPE_METHOD:
					va_start(args, slotName);
					return ((Kitsune_FunctionPtr)slot->data)(obj, args);
					break;
				case KITSUNE_SLOT_TYPE_VALUE:
					return (Kitsune_Object*)slot;
					break;
				case KITSUNE_SLOT_TYPE_NO_OP:
					curObj = NULL; /* break from the main while loop so the parent isn't searched */
					break;
			}
		}
		else
		{
			curObj = obj->parent;
		}
	}

	/* throw warning message on slot not found */
	fprintf(stderr, "WARNING: could not find slot '%s in object %p", slotName, obj);
	
	return NULL;
}


