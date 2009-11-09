/*
 *  string.hpp
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


#ifndef _KIT_STRING_H_
#define _KIT_STRING_H_

#include <kitpl/core.hpp>

#include <string>


namespace kit
{
	
	class String : public Object
	{
		public:
			
			String();
			
			static ObjPtr make(std::string str);
			ObjPtr sendMsg(MsgPtr message);
			
			ObjPtr equals(ObjPtr value);
			ObjPtr greaterThan(ObjPtr value);
			ObjPtr greaterThanOrEqual(ObjPtr value);
			ObjPtr lessThan(ObjPtr value);
			ObjPtr lessThanOrEqual(ObjPtr value);
			ObjPtr notEqual(ObjPtr value);
			
			ObjPtr length();
			ObjPtr includes(ObjPtr str);
			ObjPtr reverse();
			ObjPtr replace(ObjPtr search, ObjPtr replace);
			ObjPtr remove(ObjPtr str);
			ObjPtr toInteger();
			ObjPtr toFloat();
			ObjPtr capitalize();
			ObjPtr toUpper();
			ObjPtr toLower();
			ObjPtr print();
			ObjPtr printLine();
			ObjPtr append(ObjPtr str);
			ObjPtr subString(ObjPtr startIndex, ObjPtr endIndex);
			ObjPtr split(ObjPtr delimiter);
			
			
			std::string _value;
	};
}

#endif

