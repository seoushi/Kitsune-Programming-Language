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
		
		return ObjPtr(intObj);
	}


	ObjPtr Integer::sendMsg(MsgPtr message)
	{
		switch(message->id)
		{
			case 5859493UL: // ==
				return equals( message->args.front() );
				break;
			case 177563UL: // >
				return greaterThan( message->args.front() );
				break;
			case 5859526UL: // >=
				return greaterThanOrEqual( message->args.front() );
				break;
			case 177561UL: // <
				return lessThan( message->args.front() );
				break;
			case 5859460UL: // <=
				return greaterThanOrEqual( message->args.front() );
				break;
			case 5858873UL: // !=
				return notEqual( message->args.front() );
				break;
			case 177550UL: // +
				return add( message->args.front() );
				break;
			case 177544UL: // -
				return sub( message->args.front() );
				break;
			case 177546UL: // /
				return div( message->args.front() );
				break;
			case 177551UL: // *
				return mul( message->args.front() );
				break;
			case 177659UL: // ^
				return power( message->args.front() );
				break;
			case 177536UL: // %
				return mod( message->args.front() );
				break;
			case 2088322081UL: // sqrt
				return squareRoot();
				break;
			case 1353689091UL: // to-float
				return toFloat();
				break;
			case 1756306272UL: // to-int
				return make(_value);
				break;
			case 1756282918UL: // to-str
				return toStr();
				break;
			case 2123293021UL: // to-bool
				return toBool();
				break;
			default:
				std::cerr << "Integer does not support method(" << message->id << ")" << std::endl;
				throw 100;
				break;
		}
		
		return ObjPtr();
	}


	ObjPtr Integer::equals(ObjPtr value)
	{
		return Boolean::make(_value == ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::greaterThan(ObjPtr value)
	{
		return Boolean::make(_value > ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::greaterThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value >= ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::lessThan(ObjPtr value)
	{
		return Boolean::make(_value < ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::lessThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value <= ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::notEqual(ObjPtr value)
	{
		return Boolean::make(_value != ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}


	ObjPtr Integer::add(ObjPtr value)
	{
		return Integer::make(_value + ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::sub(ObjPtr value)
	{
		return Integer::make(_value - ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::div(ObjPtr value)
	{
		return Integer::make(_value / ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::mul(ObjPtr value)
	{
		return Integer::make(_value * ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
	}
	
	
	ObjPtr Integer::power(ObjPtr value)
	{
		return Integer::make((int)pow(_value, ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value));
	}
	
	
	ObjPtr Integer::mod(ObjPtr value)
	{
		return Integer::make(_value % ((Integer*)value->sendMsg(MsgPtr( new Message(1756306272UL /* to-int*/))).get())->_value);
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

