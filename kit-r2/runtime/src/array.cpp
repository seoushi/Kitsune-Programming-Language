/*
 *  array.cpp
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

#include <kitpl/array.hpp>

#include <kitpl/string.hpp>
#include <kitpl/integer.hpp>
#include <kitpl/boolean.hpp>

#include <sstream>

namespace kit
{
	Array::Array()
	{
	}
	
			
	Object* Array::make()
	{
		Object* arrayObj = new Object();
		
		Array* a = new Array();
		
		arrayObj->target = (void*)a;
		arrayObj->script = &Array::script;
		
		return arrayObj;
	}

	
	Object* Array::script(Object* sender, MsgId message, ...)
	{
		va_list va;
		Object* result;
		
		va_start(va, message);
		
		switch(message)
		{
			case 16: // to-str
				result = ((Array*)sender)->toString();
				break;
			case 42: // "count"
			case 21: // "length"
				result = ((Array*)sender)->length();
				break;
			case 22: // "include?"
				result = ((Array*)sender)->includes(va_arg(va, Object*));
				break;
			case 35: // "@"
			case 36: // "at"
				result = ((Array*)sender)->at(va_arg(va, Object*));
				break;
			case 37: // "add!"
				result = ((Array*)sender)->add(va_arg(va, Object*));
				break;
			case 38: // "set!"
				result = ((Array*)sender)->set(va_arg(va, Object*), va_arg(va, Object*));
				break;
			case 39: // "clear!"
				result = ((Array*)sender)->clear();
				break;
			case 40: // "map"
				result = ((Array*)sender)->map(va_arg(va, Object*));
				break;
			case 41: // "empty?"
				result = ((Array*)sender)->isEmpty();
				break;
			default:
				throw "Does Not Support Operation";
				break;
		}
		
		va_end(va);
		return result;
	}
	
	
	Object* Array::length()
	{
		return Integer::make(_value.size());
	}
	
	
	Object* Array::includes(Object* obj)
	{
		for(std::vector<Object*>::iterator itr = _value.begin(); itr != _value.end(); itr++)
		{
			if( (*itr) == obj )
			{
				return Boolean::make(true);
			}
		}
		
		return Boolean::make(false);
	}
	
	
	Object* Array::at(Object* obj)
	{
		return _value[((Integer*)(obj->script(obj, 15 /* to-int */)))->_value];
	}
	
	
	Object* Array::set(Object* index, Object* obj)
	{
		_value[((Integer*)(obj->script(obj, 15 /* to-int */)))->_value] = obj;
		
		return NULL;
	}
	
	
	Object* Array::add(Object* obj)
	{
		_value.push_back(obj);
		
		return NULL;
	}
	
	
	Object* Array::clear()
	{
		_value.clear();
		
		return NULL;
	}
	
	
	Object* Array::isEmpty()
	{
		if(_value.size() == 0)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	
	Object* Array::map(Object* obj)
	{
		for(unsigned int i = 0; i < _value.size(); i++)
		{
			obj->script(obj, 0, _value[i], Integer::make(i));
		}
		
		return NULL;
	}
	
	
	Object* Array::toString()
	{
		std::stringstream ss;
		ss << "Array [ ";
		
		for(unsigned int i = 0; i < _value.size(); i++)
		{
			ss << ((String*)(_value[i]->script(_value[i], 16 /* to-str */)))->_value;
			
			if(i < (_value.size() - 1))
			{
				ss << ", ";
			}
		}
		
		ss << " ]";
		
		return String::make(ss.str());
	}
}