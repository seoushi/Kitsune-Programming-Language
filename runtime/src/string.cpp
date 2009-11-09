/*
 *  string.cpp
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

#include <kitpl/string.hpp>
#include <kitpl/integer.hpp>
#include <kitpl/float.hpp>
#include <kitpl/boolean.hpp>

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

namespace kit
{
	String::String()
	{
	}
			
	ObjPtr String::make(std::string str)
	{
		String* strObj = new String();
		strObj->_value = str;

		return ObjPtr(strObj);
	}
	
	ObjPtr String::sendMsg(MsgPtr message)
	{		
		switch(message->id)
		{
			case 5859493UL: // ==
				return equals(message->args.front());
				break;
			case 177563UL: // >
				return greaterThan(message->args.front());
				break;
			case 5859526UL: // >=
				return greaterThanOrEqual(message->args.front());
				break;
			case 177561UL: // <
				return lessThan(message->args.front());
				break;
			case 5859460UL: // <=
				return greaterThanOrEqual(message->args.front());
				break;
			case 5858873UL: // !=
				return notEqual(message->args.front());
				break;
			case 1353689091UL: // to-float
				return toFloat();
				break;
			case 1756306272UL: // to-int
				return toInteger();
				break;
			case 1756282918UL: // to-str
				return make(_value);
				break;
			case 1433765721UL: // length
				return length();
				break;
			case 608820149UL: // includes?
				return includes(message->args.front());
				break;
			case 1362176549UL: // reverse
				return reverse();
				break;
			case 1364993353UL: // replace
			{
				ObjPtr tmpObj = message->args.front();
				message->args.pop_front();
				
				return replace(tmpObj, message->args.front());
			}
				break;
			case 2124712547UL: // remove
				return remove(message->args.front());
				break;
			case 898217777UL: // capitalize
				return capitalize();
				break;
			case 1334017297UL: // to-upper
				return toUpper();
				break;
			case 1369389104UL: // to-lower
				return toLower();
				break;
			case 187024980UL: // print
				return print();
				break;
			case 1853128887UL: // print-line
				return printLine();
				break;
			case 1326815083UL: // append
				return append(message->args.front());
				break;
			case 5859173UL: // ++
				return append(message->args.front());
				break;
			case 3597576089UL: // sub-string
			{
				ObjPtr tmpObj = message->args.front();
				message->args.pop_front();
				
				return subString(tmpObj, message->args.front());
			}
				break;
			case 195179447UL: // split
				return split(message->args.front());
				break;	
			default:
				std::cerr << "String does not support method(" << message->id << ")" << std::endl;
				throw 100;
				break;
		}
		
		return ObjPtr();
	}
			
	ObjPtr String::equals(ObjPtr value)
	{
		if(_value == ((String*)value->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	ObjPtr String::greaterThan(ObjPtr value)
	{
		if(_value > ((String*)value->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	ObjPtr String::greaterThanOrEqual(ObjPtr value)
	{
		if(_value >= ((String*)value->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	ObjPtr String::lessThan(ObjPtr value)
	{
		if(_value < ((String*)value->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	ObjPtr String::lessThanOrEqual(ObjPtr value)
	{
		if(_value <= ((String*)value->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	
	ObjPtr String::notEqual(ObjPtr value)
	{
		if(_value != ((String*)value->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
			
	ObjPtr String::length()
	{
		return Integer::make(_value.length());
	}
	
	
	ObjPtr String::includes(ObjPtr str)
	{
		if(strstr(_value.c_str(), ((String*)str->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value.c_str()))
		{
			return Boolean::make(false);
		}
		
		return Boolean::make(true);
	}
	
	
	ObjPtr String::reverse()
	{
		std::string newValue = _value;
		unsigned int i = 0;
	
		for(int j = _value.length() - 1; j >= 0; j--, i++)
		{
			newValue[i] = _value[j];
		}
		
		return String::make(newValue);
	}
	
	
	ObjPtr String::replace(ObjPtr search, ObjPtr replace)
	{
		std::string newString = _value;
		
		std::string* searchStr = &((String*)search->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value;

		std::string::size_type pos = newString.find((*searchStr));
		while( pos != std::string::npos )
		{
			newString.replace(pos, searchStr->length(), ((String*)replace->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value);
			
			pos = newString.find((*searchStr));
		}
		
		return make(newString);
	}
	
	
	ObjPtr String::remove(ObjPtr str)
	{
		return replace(str, make(""));
	}
	
	
	ObjPtr String::toInteger()
	{
		return Integer::make(atoi(_value.c_str()));
	}
	
	
	ObjPtr String::toFloat()
	{
		return Float::make(atof(_value.c_str()));
	}
	
	
	ObjPtr String::capitalize()
	{
		std::string newString = _value;
		
		for(unsigned int i = 0; i < newString.length(); i++)
		{
			if((i == 0) || ((i > 0) && (newString[i - 1] == ' ')))
			{
				if((newString[i] >= 'a') && (newString[i] <= 'z') )
				{
					newString[i] += 'A' - 'a';
				}
			}
		}
		
		return make(newString);
	}
	
	
	ObjPtr String::toUpper()
	{
		std::string newString = _value;
		
		for(unsigned int i = 0; i < newString.length(); i++)
		{
			if((newString[i] >= 'a') && (newString[i] <= 'z') )
			{
				newString[i] += 'A' - 'a';
			}
		}
		
		return make(newString);
	}
	
	
	ObjPtr String::toLower()
	{
		std::string newString = _value;
		
		for(unsigned int i = 0; i < newString.length(); i++)
		{
			if((newString[i] >= 'A') && (newString[i] <= 'Z') )
			{
				newString[i] += 'a' - 'A';
			}
		}
		
		return make(newString);
	}
	
	
	ObjPtr String::print()
	{
		std::cout << _value;
		return ObjPtr();
	}
	
	
	ObjPtr String::printLine()
	{
		std::cout << _value << std::endl;
		return ObjPtr();
	}
	
	
	ObjPtr String::append(ObjPtr str)
	{
		std::stringstream ss;
		
		ss << _value;
		ss << ((String*)str->sendMsg(MsgPtr(new Message(1756282918UL /* to-str */))).get())->_value;
		
		return String::make(ss.str());
	}
	
	
	ObjPtr String::subString(ObjPtr startIndex, ObjPtr endIndex)
	{
		std::string newString = _value.substr(((Integer*)startIndex->sendMsg(MsgPtr(new Message(1756306272UL /* to-int */))).get())->_value,
											  ((Integer*)endIndex->sendMsg(MsgPtr(new Message(1756306272UL /* to-int */))).get())->_value);
		
		return make(newString);
	}
	
	
	ObjPtr String::split(ObjPtr delimiter)
	{
		return ObjPtr();
	}
}
