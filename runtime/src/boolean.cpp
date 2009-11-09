/*
 *  integer.cpp
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

#include <sstream>
#include <math.h>
#include <iostream>

#include <kitpl/boolean.hpp>
#include <kitpl/string.hpp>

namespace kit
{
	
	Boolean::Boolean()
	{
	}

	
	ObjPtr Boolean::make(bool boolean)
	{
		Boolean* boolObj = new Boolean();
		boolObj->_value = boolean;
		
		return ObjPtr(boolObj);
	}


	ObjPtr Boolean::sendMsg(MsgPtr message)
	{
		
		switch(message->id)
		{
			case 5859493UL: // ==
				return equal(message->args.front());
				break;
			case 5858873UL: // !=
				return notEqual(message->args.front());
				break;
			case 1756282918UL: // to-str
				return toStr();
				break;
			case 177539UL: // &
				return andOp(message->args.front());
				break;
			case 177625UL: // |
				return orOp(message->args.front());
				break;
			case 177540UL: // !
				return notOp();
				break;
			case 2123293021UL: // to-bool
				return make(_value);
				break;
			default:
				std::cerr << "Boolean does not support method(" << message << ")" << std::endl;
				throw 100;
				break;
		}
		
		return ObjPtr();
	}
			
	ObjPtr Boolean::equal(ObjPtr value)
	{
		return Boolean::make(_value == ((Boolean*)(value->sendMsg(MsgPtr( new Message(2123293021UL /* to-bool */))).get()))->_value);
	}
	
	ObjPtr Boolean::notEqual(ObjPtr value)
	{
		return Boolean::make(_value != ((Boolean*)(value->sendMsg(MsgPtr( new Message(2123293021UL /* to-bool */))).get()))->_value);
	}
	
	ObjPtr Boolean::andOp(ObjPtr value)
	{
		return Boolean::make(_value && ((Boolean*)(value->sendMsg(MsgPtr( new Message(2123293021UL /* to-bool */))).get()))->_value);
	}
	
	ObjPtr Boolean::orOp(ObjPtr value)
	{
		return Boolean::make(_value || ((Boolean*)(value->sendMsg(MsgPtr( new Message(2123293021UL /* to-bool */))).get()))->_value);
	}
	
	ObjPtr Boolean::notOp()
	{
		return Boolean::make(!_value);
	}
	
	ObjPtr Boolean::toStr()
	{
		if(_value)
		{
			return String::make("True");
		}
		
		return String::make("False");
	}

}

