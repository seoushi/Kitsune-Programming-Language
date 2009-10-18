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

#include <stdarg.h>
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


	Object* Boolean::make(bool boolean)
	{
		Boolean* boolObj = new Boolean();
		boolObj->_value = boolean;
		
		return boolObj;
	}


	Object* Boolean::script(MsgId message, ...)
	{
		va_list va;
		Object* result;
		
		va_start(va, message);
		
		switch(message)
		{
			case 5859493: // ==
				result = equal(va_arg(va, Object*) );
				break;
			case 5858873: // !=
				result = notEqual(va_arg(va, Object*) );
				break;
			case 1756282918: // to-str
				result = toStr();
				break;
			case 177539: // &
				result = andOp(va_arg(va, Object*));
				break;
			case 177625: // |
				result = orOp(va_arg(va, Object*));
				break;
			case 177540: // !
				result = notOp();
				break;
			case 2123293021: // to-bool
				result = this;
				break;
			default:
				std::cerr << "Boolean does not support method(" << message << ")" << std::endl;
				throw 100;
				break;
		}
		
		va_end(va);
		return result;
	}
			
	Object* Boolean::equal(Object* value)
	{
		return Boolean::make(_value == ((Boolean*)value->script(2123293021/* to-bool */))->_value);
	}
	
	Object* Boolean::notEqual(Object* value)
	{
		return Boolean::make(_value != ((Boolean*)value->script(2123293021/* to-bool */))->_value);
	}
	
	Object* Boolean::andOp(Object* value)
	{
		return Boolean::make(_value && ((Boolean*)value->script(2123293021/* to-bool */))->_value);
	}
	
	Object* Boolean::orOp(Object* value)
	{
		return Boolean::make(_value || ((Boolean*)value->script(2123293021/* to-bool */))->_value);
	}
	
	Object* Boolean::notOp()
	{
		return Boolean::make(!_value);
	}
	
	Object* Boolean::toStr()
	{
		if(_value)
		{
			return String::make("True");
		}
		
		return String::make("False");
	}

}

