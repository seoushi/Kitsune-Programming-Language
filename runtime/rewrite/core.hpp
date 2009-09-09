/*
 *  core.hpp
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
       
       
#ifndef _KITSUNE_CORE_HPP_
#define _KITSUNE_CORE_HPP_

#include <string>
#include <map>

namespace kitsune
{
	namespace SlotType
	{
		enum Type
		{
			noOp,
			method,
			value,
		};
	}
		
	class Slot
	{
		public:
		SlotType::Type type;
		void* data;
	};
		

	typedef std::map<std::string, Slot> SlotMap;
	typedef SlotMap::iterator			SlotMapItr;
	
	class Object
	{
		public:
			Object();
			
			unsigned int	numSlots;
			SlotMap			slots;
			Object*			parent;
		
		
			bool internString(std::string string, unsigned int* result);

			Object* bind(std::string slotName, SlotType::Type* resultCode);
	};


	// function pointer
	typedef Object* (*FunctionPtr)(Object*, ... );
}


#define Kitsune_SendMessage(CLOSURE, RCV, MSG, ARGS...) ({			\
	kitsune::Object*	r = (kitsune::Object*)(RCV);				\
	kitsune::Object*	result;										\
	SlotType::Type		resultCode;									\
																	\
	result = r->bind((MSG), &resultCode);							\
																	\
	(resultCode == SlotType::noOp) ? NULL :							\
		((resultCode == SlotType::value) ? result :					\
			((kitsune::FunctionPtr)result)((CLOSURE), r, ##ARGS));	\
}) 

#endif
 																																			