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
#include <iostream>
#include <math.h>

#include <kitpl/integer.hpp>
#include <kitpl/float.hpp>
#include <kitpl/boolean.hpp>
#include <kitpl/string.hpp>

namespace kit
{
	Integer::Integer()
	{
	}

	Object* Integer::make(int integer)
	{
		Integer* intObj = new Integer();
		intObj->_value = integer;
		
		return intObj;
	}


	Object* Integer::script(MsgId message, ...)
	{
		va_list va;
		Object* result;
		
		va_start(va, message);

		switch(message)
		{
			case 5859493: // ==
				result = equals( va_arg(va, Object*) );
				break;
			case 177563: // >
				result = greaterThan( va_arg(va, Object*) );
				break;
			case 5859526: // >=
				result = greaterThanOrEqual( va_arg(va, Object*) );
				break;
			case 177561: // <
				result = lessThan( va_arg(va, Object*) );
				break;
			case 5859460: // <=
				result = greaterThanOrEqual( va_arg(va, Object*) );
				break;
			case 5858873: // !=
				result = notEqual( va_arg(va, Object*) );
				break;
			case 177550: // +
				result = add( va_arg(va, Object*) );
				break;
			case 177544: // -
				result = sub( va_arg(va, Object*) );
				break;
			case 177546: // /
				result = div( va_arg(va, Object*) );
				break;
			case 177551: // *
				result = mul( va_arg(va, Object*) );
				break;
			case 177659 : // ^
				result = power( va_arg(va, Object*) );
				break;
			case 177536: // %
				result = mod( va_arg(va, Object*) );
				break;
			case 6383289377: // sqrt
				result = squareRoot();
				break;
			case 7569653579622403: // to-float
				result = toFloat();
				break;
			case 6951013391200: // to-int
				result = this;
				break;
			case 6951013367846: // to-str
				result = toStr();
				break;
			case 229383441670493: // to-bool
				result = toBool();
				break;
			default:
				std::cerr << "Integer does not support method(" << message << ")" << std::endl;
				throw 100;
				break;
		}
		
		va_end(va);
		return result;
	}


	Object* Integer::equals(Object* value)
	{
		return Boolean::make(_value == ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::greaterThan(Object* value)
	{
		return Boolean::make(_value > ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::greaterThanOrEqual(Object* value)
	{
		return Boolean::make(_value >= ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::lessThan(Object* value)
	{
		return Boolean::make(_value < ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::lessThanOrEqual(Object* value)
	{
		return Boolean::make(_value <= ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::notEqual(Object* value)
	{
		return Boolean::make(_value != ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}


	Object* Integer::add(Object* value)
	{
		return Integer::make(_value + ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::sub(Object* value)
	{
		return Integer::make(_value - ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::div(Object* value)
	{
		return Integer::make(_value / ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::mul(Object* value)
	{
		return Integer::make(_value * ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::power(Object* value)
	{
		return Integer::make((int)pow(_value, ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value));
	}
	
	
	Object* Integer::mod(Object* value)
	{
		return Integer::make(_value % ((Integer*)(value->script(6951013391200 /* to-int*/)))->_value);
	}
	
	
	Object* Integer::squareRoot()
	{
		return Float::make(sqrt(_value));
	}
	
	
	Object* Integer::toFloat()
	{
		return Float::make((float)_value);
	}
	
	Object* Integer::toStr()
	{
		std::stringstream ss;
		ss << _value;
		
		return String::make(ss.str());
	}

	Object* Integer::toBool()
	{
		if(_value == 0.0f)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
}

