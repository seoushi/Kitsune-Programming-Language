/*
 *  float.cpp
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

#include <kitpl/float.hpp>
#include <kitpl/integer.hpp>
#include <kitpl/boolean.hpp>
#include <kitpl/string.hpp>

namespace kit
{
	Float::Float()
	{
	}

	Object* Float::make(float value)
	{
		Object* floatObj = new Object();

		Float* f = new Float();
		f->_value = value;

		floatObj->target = (void*)f;
		floatObj->script = &Float::script;

		return floatObj;
	}


	Object* Float::script(Object* sender, MsgId message, ...)
	{
		va_list va;
		Object* result;
		
		va_start(va, message);

		switch(message)
		{
			case 1: // ==
				result = ((Float*)sender)->equals( va_arg(va, Object*) );
				break;
			case 2: // >
				result = ((Float*)sender)->greaterThan( va_arg(va, Object*) );
				break;
			case 3: // >=
				result = ((Float*)sender)->greaterThanOrEqual( va_arg(va, Object*) );
				break;
			case 4: // <
				result = ((Float*)sender)->lessThan( va_arg(va, Object*) );
				break;
			case 5: // <=
				result = ((Float*)sender)->greaterThanOrEqual( va_arg(va, Object*) );
				break;
			case 6: // !=
				result = ((Float*)sender)->notEqual( va_arg(va, Object*) );
				break;
			case 7: // +
				result = ((Float*)sender)->add( va_arg(va, Object*) );
				break;
			case 8: // -
				result = ((Float*)sender)->sub( va_arg(va, Object*) );
				break;
			case 9: // /
				result = ((Float*)sender)->div( va_arg(va, Object*) );
				break;
			case 10: // *
				result = ((Float*)sender)->mul( va_arg(va, Object*) );
				break;
			case 11: // ^
				result = ((Float*)sender)->power( va_arg(va, Object*) );
				break;
			case 12: // %
				result = ((Float*)sender)->mod( va_arg(va, Object*) );
				break;
			case 13: // sqrt
				result = ((Float*)sender)->squareRoot();
				break;
			case 14: // to-float
				result = sender;
				break;
			case 15: // to-int
				result = ((Float*)sender)->toInt();
				break;
			case 16: // to-str
				result = ((Float*)sender)->toStr();
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


	Object* Float::equals(Object* value)
	{
		return Boolean::make(_value == ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::greaterThan(Object* value)
	{
		return Boolean::make(_value > ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::greaterThanOrEqual(Object* value)
	{
		return Boolean::make(_value >= ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::lessThan(Object* value)
	{
		return Boolean::make(_value < ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::lessThanOrEqual(Object* value)
	{
		return Boolean::make(_value <= ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::notEqual(Object* value)
	{
		return Boolean::make(_value != ((Float*)value->script(value, 14/* to-float */))->_value);
	}


	Object* Float::add(Object* value)
	{
		return Float::make(_value + ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::sub(Object* value)
	{
		return Float::make(_value - ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::div(Object* value)
	{
		return Float::make(_value / ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::mul(Object* value)
	{
		return Float::make(_value * ((Float*)value->script(value, 14/* to-float */))->_value);
	}
	
	
	Object* Float::power(Object* value)
	{
		return Float::make((float)pow(_value, ((Float*)value->script(value, 14/* to-float */))->_value));
	}
	
	
	Object* Float::mod(Object* value)
	{
		return Float::make(fmod(_value, ((Float*)value->script(value, 14/* to-float */))->_value));
	}
	
	
	Object* Float::squareRoot()
	{
		return Float::make(sqrt(_value));
	}
	
	
	Object* Float::toInt()
	{
		return Integer::make((int)(_value + 0.5f));
	}
	
	Object* Float::toStr()
	{
		std::stringstream ss;
		ss << _value;
		
		return String::make(ss.str());
	}

	Object* Float::toBool()
	{
		if(_value == 0.0f)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
}

