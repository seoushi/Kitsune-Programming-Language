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
		Object* intObj = new Object();

		Integer* i = new Integer();
		i->_value = integer;

		intObj->target = (void*)i;
		intObj->script = &Integer::script;

		return intObj;
	}


	Object* Integer::script(Object* sender, MsgId message, ...)
	{
		va_list va;
		Object* result;
		
		va_start(va, message);

		switch(message)
		{
			case 1: // ==
				result = ((Integer*)sender)->equals( va_arg(va, Object*) );
				break;
			case 2: // >
				result = ((Integer*)sender)->greaterThan( va_arg(va, Object*) );
				break;
			case 3: // >=
				result = ((Integer*)sender)->greaterThanOrEqual( va_arg(va, Object*) );
				break;
			case 4: // <
				result = ((Integer*)sender)->lessThan( va_arg(va, Object*) );
				break;
			case 5: // <=
				result = ((Integer*)sender)->greaterThanOrEqual( va_arg(va, Object*) );
				break;
			case 6: // !=
				result = ((Integer*)sender)->notEqual( va_arg(va, Object*) );
				break;
			case 7: // +
				result = ((Integer*)sender)->add( va_arg(va, Object*) );
				break;
			case 8: // -
				result = ((Integer*)sender)->sub( va_arg(va, Object*) );
				break;
			case 9: // /
				result = ((Integer*)sender)->div( va_arg(va, Object*) );
				break;
			case 10: // *
				result = ((Integer*)sender)->mul( va_arg(va, Object*) );
				break;
			case 11: // ^
				result = ((Integer*)sender)->power( va_arg(va, Object*) );
				break;
			case 12: // %
				result = ((Integer*)sender)->mod( va_arg(va, Object*) );
				break;
			case 13: // sqrt
				result = ((Integer*)sender)->squareRoot();
				break;
			case 14: // to-float
				result = ((Integer*)sender)->toFloat();
				break;
			case 15: // to-int
				result = sender;
				break;
			case 16: // to-str
				result = ((Integer*)sender)->toStr();
				break;
			case 20: // to-bool
				result = ((Float*)sender)->toBool();
				break;
			default:
				throw "Does Not Support Operation";
				break;
		}
		
		va_end(va);
		return result;
	}


	Object* Integer::equals(Object* value)
	{
		return Boolean::make(_value == ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::greaterThan(Object* value)
	{
		return Boolean::make(_value > ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::greaterThanOrEqual(Object* value)
	{
		return Boolean::make(_value >= ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::lessThan(Object* value)
	{
		return Boolean::make(_value < ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::lessThanOrEqual(Object* value)
	{
		return Boolean::make(_value <= ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::notEqual(Object* value)
	{
		return Boolean::make(_value != ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}


	Object* Integer::add(Object* value)
	{
		return Integer::make(_value + ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::sub(Object* value)
	{
		return Integer::make(_value - ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::div(Object* value)
	{
		return Integer::make(_value / ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::mul(Object* value)
	{
		return Integer::make(_value * ((Integer*)(value->script(value, 15/* to-int */)))->_value);
	}
	
	
	Object* Integer::power(Object* value)
	{
		return Integer::make((int)pow(_value, ((Integer*)(value->script(value, 15/* to-int */)))->_value));
	}
	
	
	Object* Integer::mod(Object* value)
	{
		return Integer::make(_value % ((Integer*)(value->script(value, 15/* to-int */)))->_value);
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
