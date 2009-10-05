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

namespace kit
{
	String::String()
	{
	}
			
	Object* String::make(std::string str)
	{
		String* strObj = new String();
		strObj->_value = str;

		return strObj;
	}
	
	Object* String::script(MsgId message, ...)
	{
		va_list va;
		Object* result;
		
		va_start(va, message);
		
		switch(message)
		{
			case 1: // ==
				result = equals(va_arg(va, Object*));
				break;
			case 2: // >
				result = greaterThan(va_arg(va, Object*));
				break;
			case 3: // >=
				result = greaterThanOrEqual(va_arg(va, Object*));
				break;
			case 4: // <
				result = lessThan(va_arg(va, Object*));
				break;
			case 5: // <=
				result = greaterThanOrEqual(va_arg(va, Object*));
				break;
			case 6: // !=
				result = notEqual(va_arg(va, Object*));
				break;
			case 14: // to-float
				result = toFloat();
				break;
			case 15: // to-int
				result = toInteger();
				break;
			case 16: // to-str
				result = this;
				break;
			case 21: // length
				result = length();
				break;
			case 22: // includes?
				result = includes(va_arg(va, Object*));
				break;
			case 23: // reverse
				result = reverse();
				break;
			case 24: // replace
				result = replace(va_arg(va, Object*), va_arg(va, Object*));
				break;
			case 25: // remove
				result = remove(va_arg(va, Object*));
				break;
			case 26: // capitalize
				result = capitalize();
				break;
			case 27: // to-upper
				result = toUpper();
				break;
			case 28: // to-lower
				result = toLower();
				break;
			case 29: // print
				result = print();
				break;
			case 30: // print-line
				result = printLine();
				break;
			case 31: // append
				result = append(va_arg(va, Object*));
				break;
			case 32: // ++
				result = append(va_arg(va, Object*));
				break;
			case 33: // sub-string
				result = subString(va_arg(va, Object*), va_arg(va, Object*));
				break;
			case 34: // split
				result = split(va_arg(va, Object*));
				break;	
			default:
				std::cerr << "String does not support method(" << message << ")" << std::endl;
				throw 100;
				break;
		}
		
		va_end(va);
		return result;
	}
			
	Object* String::equals(Object* value)
	{
		if(_value == ((String*)(value->script(16/* to-str */)))->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	Object* String::greaterThan(Object* value)
	{
		if(_value > ((String*)(value->script(16/* to-str */)))->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	Object* String::greaterThanOrEqual(Object* value)
	{
		if(_value >= ((String*)(value->script(16/* to-str */)))->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	Object* String::lessThan(Object* value)
	{
		if(_value < ((String*)(value->script(16/* to-str */)))->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	Object* String::lessThanOrEqual(Object* value)
	{
		if(_value <= ((String*)(value->script(16/* to-str */)))->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	
	Object* String::notEqual(Object* value)
	{
		if(_value != ((String*)(value->script(16/* to-str */)))->_value)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
			
	Object* String::length()
	{
		return Integer::make(_value.length());
	}
	
	
	Object* String::includes(Object* str)
	{
		if(strstr(_value.c_str(), (((String*)(str->script(16/* to-str */)))->_value).c_str()))
		{
			return Boolean::make(false);
		}
		
		return Boolean::make(true);
	}
	
	
	Object* String::reverse()
	{
		std::string newValue = _value;
		unsigned int i = 0;
	
		for(int j = _value.length() - 1; j >= 0; j--, i++)
		{
			newValue[i] = _value[j];
		}
		
		return String::make(newValue);
	}
	
	
	Object* String::replace(Object* search, Object* replace)
	{
		std::string newString = _value;
		std::string* searchStr = &((String*)(search->script(16/* to-str */)))->_value;

		std::string::size_type pos = newString.find((*searchStr));
		while( pos != std::string::npos )
		{
			newString.replace(pos, searchStr->length(), ((String*)(replace->script(16/* to-str */)))->_value);
			
			pos = newString.find((*searchStr));
		}
		
		return make(newString);
	}
	
	
	Object* String::remove(Object* str)
	{
		return replace(str, make(""));
	}
	
	
	Object* String::toInteger()
	{
		return Integer::make(atoi(_value.c_str()));
	}
	
	
	Object* String::toFloat()
	{
		return Float::make(atof(_value.c_str()));
	}
	
	
	Object* String::capitalize()
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
	
	
	Object* String::toUpper()
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
	
	
	Object* String::toLower()
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
	
	
	Object* String::print()
	{
		std::cout << _value;
		return NULL;
	}
	
	
	Object* String::printLine()
	{
		std::cout << _value << std::endl;
		return NULL;
	}
	
	
	Object* String::append(Object* str)
	{
		std::stringstream ss;
		
		ss << _value;
		ss << ((String*)(str->script(16/* to-str */)))->_value;
		
		return String::make(ss.str());
	}
	
	
	Object* String::subString(Object* startIndex, Object* endIndex)
	{
		std::string newString = _value.substr(((Integer*)(startIndex->script(15/* to-int */)))->_value,
											  ((Integer*)(endIndex->script(15/* to-int */)))->_value);
		
		return make(newString);
	}
	
	
	Object* String::split(Object* delimiter)
	{
		return NULL;
	}
}