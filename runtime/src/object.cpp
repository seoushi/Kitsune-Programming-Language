/*
 *  core.cpp
 *  kitsune runtime
 *
 * Copyright (c) 2010, Seoushi Games
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
 
#include <kitpl/object.hpp>


namespace kitpl
{


objPtr objCreate(objPtr parent)
{
	objPtr object = objPtr(new obj());
	object->parent = parent;
	object->script = NULL;
  
	return object;
}


objPtr objLookup(std::string name, objPtr curObj)
{
	objPtr object = curObj;
	objPtr params;

	//search the current object and it's parents
	while(object)
	{
		// look at parameters first
		if(object->data.find("_KITSUNE_PL_PARAMETERS_") != object->data.end())
		{
			params = object->data["_KITSUNE_PL_PARAMETERS_"];
			
			if (params->data.find(name) != params->data.end())
			{
				return params->data[name];
			}
		}
		
		//look at the object
		if(object->data.find(name) != object->data.end())
		{
			return object->data[name];
		}
		
		// it wasn't found so time to search it's parent
		object = object->parent;
	}

	//TODO: create exception class and throw a value not found
	throw 100;
}


objPtr objExec(objPtr object, objPtr parameters)
{
	if(object->script)
	{
		return object->script(parameters);
	}
	
	//TODO: create exception class and throw object not executable
	throw 200;
}


void objAssign(objPtr object, std::string field, objPtr data)
{
	object->data[field] = data;
}


}

