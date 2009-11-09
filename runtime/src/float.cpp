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
		return ObjPtr(floatObj);
	}


	ObjPtr Float::sendMsg(MsgPtr message)
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
			case 177536: // %
				return mod( message->args.front() );
				break;
			case 2088322081UL: // sqrt
				return squareRoot();
				break;
			case 1353689091UL: // to-float
				return make(_value);
				break;
			case 1756306272UL: // to-int
				return toInt();
				break;
			case 1756282918UL: // to-str
				return toStr();
				break;
			case 2123293021UL: // to-bool
				return toBool();
				break;
			default:
				std::cerr << "Float does not support method(" << message->id << ")" << std::endl;
				throw 100;
		}
		
		return ObjPtr();
	}


	ObjPtr Float::equals(ObjPtr value)
	{
		return Boolean::make(_value == ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::greaterThan(ObjPtr value)
	{
		return Boolean::make(_value > ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::greaterThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value >= ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::lessThan(ObjPtr value)
	{
		return Boolean::make(_value < ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::lessThanOrEqual(ObjPtr value)
	{
		return Boolean::make(_value <= ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::notEqual(ObjPtr value)
	{
		return Boolean::make(_value != ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}


	ObjPtr Float::add(ObjPtr value)
	{
		return Float::make(_value + ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::sub(ObjPtr value)
	{
		return Float::make(_value - ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::div(ObjPtr value)
	{
		return Float::make(_value / ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::mul(ObjPtr value)
	{
		return Float::make(_value * ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value);
	}
	
	
	ObjPtr Float::power(ObjPtr value)
	{
		return Float::make((float)pow(_value, ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value));
	}
	
	
	ObjPtr Float::mod(ObjPtr value)
	{
		return Float::make((float)fmod(_value, ((Float*)value->sendMsg(MsgPtr(new Message(1353689091UL /* to-float */))).get())->_value));
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

