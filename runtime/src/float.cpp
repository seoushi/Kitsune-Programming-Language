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
#include <iostream>

#include <kitpl/float.hpp>
#include <kitpl/integer.hpp>
#include <kitpl/boolean.hpp>
#include <kitpl/string.hpp>

namespace kit
{
	Float::Float()
	{
	}

	ObjPtr Float::make(float value)
	{
		Float* floatObj = new Float();
		floatObj->_value = value;
		return floatObj;
	}


	ObjPtr Float::script(MsgId message, ...)
	{
		va_list va;
		ObjPtr result;
		
		va_start(va, message);

		switch(message)
		{
			case 5859493UL: // ==
				result = equals( va_arg(va, ObjPtr) );
				break;
			case 177563UL: // >
				result = greaterThan( va_arg(va, ObjPtr) );
				break;
			case 5859526UL: // >=
				result = greaterThanOrEqual( va_arg(va, ObjPtr) );
				break;
			case 177561UL: // <
				result = lessThan( va_arg(va, ObjPtr) );
				break;
			case 5859460UL: // <=
				result = greaterThanOrEqual( va_arg(va, ObjPtr) );
				break;
			case 5858873UL: // !=
				result = notEqual( va_arg(va, ObjPtr) );
				break;
			case 177550UL: // +
				result = add( va_arg(va, ObjPtr) );
				break;
			case 177544UL: // -
				result = sub( va_arg(va, ObjPtr) );
				break;
			case 177546UL: // /
				result = div( va_arg(va, ObjPtr) );
				break;
			case 177551UL: // *
				result = mul( va_arg(va, ObjPtr) );
				break;
			case 177659UL: // ^
				result = power( va_arg(va, ObjPtr) );
				break;
			case 177536: // %
				result = mod( va_arg(va, ObjPtr) );
				break;
			case 2088322081UL: // sqrt
				result = squareRoot();
				break;
			case 1353689091UL: // to-float
				result = this;
				break;
			case 1756306272UL: // to-int
				result = toInt();
				break;
			case 1756282918UL: // to-str
				result = toStr();
				break;
			case 2123293021UL: // to-bool
				result = toBool();
				break;
			default:
				std::cerr << "Float does not support method(" << message << ")" << std::endl;
				throw 100;
		}
		
		va_end(va);
		return result;
	}


	ObjPtr Float::equals(ObjPtr value)
	{
		return Boolean::make(_value == ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::greaterThan(ObjPtr value)
	{
		return Boolean::make(_value > ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::greaterThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value >= ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::lessThan(ObjPtr value)
	{
		return Boolean::make(_value < ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::lessThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value <= ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::notEqual(ObjPtr value)
	{
		return Boolean::make(_value != ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}


	ObjPtr Float::add(ObjPtr value)
	{
		return Float::make(_value + ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::sub(ObjPtr value)
	{
		return Float::make(_value - ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::div(ObjPtr value)
	{
		return Float::make(_value / ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::mul(ObjPtr value)
	{
		return Float::make(_value * ((Float*)value->script(1353689091UL /* to-float */))->_value);
	}
	
	
	ObjPtr Float::power(ObjPtr value)
	{
		return Float::make((float)pow(_value, ((Float*)value->script(1353689091UL /* to-float */))->_value));
	}
	
	
	ObjPtr Float::mod(ObjPtr value)
	{
		return Float::make(fmod(_value, ((Float*)value->script(1353689091UL /* to-float */))->_value));
	}
	
	
	ObjPtr Float::squareRoot()
	{
		return Float::make(sqrt(_value));
	}
	
	
	ObjPtr Float::toInt()
	{
		return Integer::make((int)(_value + 0.5f));
	}
	
	ObjPtr Float::toStr()
	{
		std::stringstream ss;
		ss << _value;
		
		return String::make(ss.str());
	}

	ObjPtr Float::toBool()
	{
		if(_value == 0.0f)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
}

