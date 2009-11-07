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

	ObjPtr Integer::make(int integer)
	{
		Integer* intObj = new Integer();
		intObj->_value = integer;
		
		return intObj;
	}


	ObjPtr Integer::script(MsgId message, ...)
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
			case 177536UL: // %
				result = mod( va_arg(va, ObjPtr) );
				break;
			case 2088322081UL: // sqrt
				result = squareRoot();
				break;
			case 1353689091UL: // to-float
				result = toFloat();
				break;
			case 1756306272UL: // to-int
				result = this;
				break;
			case 1756282918UL: // to-str
				result = toStr();
				break;
			case 2123293021UL: // to-bool
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


	ObjPtr Integer::equals(ObjPtr value)
	{
		return Boolean::make(_value == ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::greaterThan(ObjPtr value)
	{
		return Boolean::make(_value > ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::greaterThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value >= ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::lessThan(ObjPtr value)
	{
		return Boolean::make(_value < ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::lessThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value <= ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::notEqual(ObjPtr value)
	{
		return Boolean::make(_value != ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}


	ObjPtr Integer::add(ObjPtr value)
	{
		return Integer::make(_value + ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::sub(ObjPtr value)
	{
		return Integer::make(_value - ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::div(ObjPtr value)
	{
		return Integer::make(_value / ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::mul(ObjPtr value)
	{
		return Integer::make(_value * ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::power(ObjPtr value)
	{
		return Integer::make((int)pow(_value, ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value));
	}
	
	
	ObjPtr Integer::mod(ObjPtr value)
	{
		return Integer::make(_value % ((Integer*)(value->script(1756306272UL /* to-int*/)))->_value);
	}
	
	
	ObjPtr Integer::squareRoot()
	{
		return Float::make(sqrt(_value));
	}
	
	
	ObjPtr Integer::toFloat()
	{
		return Float::make((float)_value);
	}
	
	ObjPtr Integer::toStr()
	{
		std::stringstream ss;
		ss << _value;
		
		return String::make(ss.str());
	}

	ObjPtr Integer::toBool()
	{
		if(_value == 0.0f)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
}

