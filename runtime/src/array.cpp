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
#include <kitpl/core.hpp>

#include <sstream>
#include <iostream>


namespace kit
{
	Array::Array()
	{
	}
	
	ObjPtr Array::sendMsg(MsgPtr msg)
	{		
		switch(msg->id)
		{
			case 2087696263UL: //make
				return ObjPtr(new Array());
				break;
			case 1756282918UL: // to-str
				return toString();
				break;
			case 176967078UL: // count
			case 1433765721UL: // length
				return length();
				break;
			case 4183265798UL: // "include?"
				return includes(msg->args.front());
				break;
			case 177637UL: // "@"
			case 5860912UL: // "at"
				return at(msg->args.front());
				break;
			case 2087549669UL: // "add!"
				return add(msg->args.front());
				break;
			case 2088334374UL: // "set!"
			{
				ObjPtr arg1 = msg->args.front();
				msg->args.pop_front();
				
				return set(arg1, msg->args.front());
			}
				break;
			case 1543483357UL: // "clear!"
				return clear();
				break;
			case 2087697144UL: // "map!"
				return map(msg->args.front());
				break;
			case 1165546895UL: // "empty?"
				return isEmpty();
				break;
			default:
				std::cerr << "Array does not support method(" << msg->id << ")" << std::endl;
				throw 100;
				break;
		}
		
		return ObjPtr();
	}
	
	
	ObjPtr Array::length()
	{
		return Integer::make(_value.size());
	}
	
	
	ObjPtr Array::includes(ObjPtr obj)
	{
		for(std::vector<ObjPtr>::iterator itr = _value.begin(); itr != _value.end(); itr++)
		{
			if( (*itr) == obj )
			{
				return Boolean::make(true);
			}
		}
		
		return Boolean::make(false);
	}
	
	
	ObjPtr Array::at(ObjPtr obj)
	{
		ObjPtr tmpObj = obj->sendMsg(MsgPtr(new Message(1756306272UL /* to-int */)));
		
		return _value[((Integer*)tmpObj.get())->_value];
	}
	
	
	ObjPtr Array::set(ObjPtr index, ObjPtr obj)
	{
		_value[((Integer*)(obj->sendMsg(MsgPtr(new Message(1756306272UL /* to-int */)))).get())->_value] = obj;
		
		return ObjPtr();
	}
	
	
	ObjPtr Array::add(ObjPtr obj)
	{
		_value.push_back(obj);
		
		return ObjPtr();
	}
	
	
	ObjPtr Array::clear()
	{
		_value.clear();
		
		return ObjPtr();
	}
	
	
	ObjPtr Array::isEmpty()
	{
		if(_value.size() == 0)
		{
			return Boolean::make(true);
		}
		
		return Boolean::make(false);
	}
	
	
	ObjPtr Array::map(ObjPtr obj)
	{
		for(unsigned int i = 0; i < _value.size(); i++)
		{
			_value[i] = obj->sendMsg( MsgPtr((new Message(0))->add(_value[i])));
		}
		
		return ObjPtr();
	}
	
	
	ObjPtr Array::toString()
	{
		std::stringstream ss;
		ss << "Array [ ";
		
		for(unsigned int i = 0; i < _value.size(); i++)
		{
			ss << ((String*)_value[i]->sendMsg(MsgPtr(new Message(1756282918UL/*to-str*/))).get())->_value;
			
			if(i < (_value.size() - 1))
			{
				ss << ", ";
			}
		}
		
		ss << " ]";
		
		return String::make(ss.str());
	}
}
