/*
 *  core.h
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
       
       
#ifndef _KITSUNE_CORE_H_
#define _KITSUNE_CORE_H_

#include <stdbool.h>
#include <stdarg.h>

/*			slot types 			*/
typedef enum
{
	KITSUNE_SLOT_TYPE_NO_OP,
	KITSUNE_SLOT_TYPE_METHOD,
	KITSUNE_SLOT_TYPE_VALUE,
}Kitsune_SlotType;


typedef struct
{
	Kitsune_SlotType type;
	void* data;
}Kitsune_Slot;


typedef struct
{
	unsigned int	numSlots;
	Kitsune_Slot**	slots;
	char**			slotKeys;

	void* parent;
}Kitsune_Object;


/*	function pointer		*/
typedef Kitsune_Object* (*Kitsune_FunctionPtr)(Kitsune_Object*, ... );


/*	core runtime functions	*/
bool Kitsune_InternString(Kitsune_Object* obj, char* string, unsigned int* result);


/* for reference bind can result three result codes
* 0 = no method found
* 1 = method
* 2 = value
*/
Kitsune_Object* Kitsune_Bind(Kitsune_Object* obj, char* slotName, int* resultCode);


#define Kitsune_SendMessage(RCV, MSG, ARGS...) ({ 									\
	Kitsune_Object* r = (Kitsune_Object*)(RCV);										\
	Kitsune_Object*	result;															\
	int				resultCode;														\
																					\
	result = Kitsune_Bind(r, (MSG), &resultCode);									\
																					\
	(resultCode == 0) ? NULL : 														\
		((resultCode == 2) ? result : ((Kitsune_FunctionPtr)result)(r, ##ARGS));	\
}) 

#endif
 																																			
