/*
 *  core.hpp
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

#ifndef _KIT_CORE_HPP_
#define _KIT_CORE_HPP_

#include <boost/shared_ptr.hpp>
#include <list>

namespace kit
{
	// message id
	#ifdef __i386__
		typedef unsigned long MsgId;
	#else
		typedef unsigned int MsgId;
	#endif


	class Object;
	typedef boost::shared_ptr<Object> ObjPtr;
	
	
	//message structure
	class Message
	{
		public:
		
			Message(MsgId id){ this->id = id; };
			~Message(){};
		
			Message* add(ObjPtr obj)
			{
				args.push_back(obj);
				return this;
			}
		
			MsgId id;
			std::list<ObjPtr> args;
	};
	typedef boost::shared_ptr<Message> MsgPtr;


	// base object type
	class Object
	{
		public:

			Object(){};

			virtual ObjPtr sendMsg(MsgPtr msg) = 0;
	};
	

}

#endif
