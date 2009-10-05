/*
 *  identifiers.cpp
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


#include "identifiers.hpp"

using namespace kitc;


Identifiers::Identifiers()
{
	nextId = 0;
	LoadRuntimeIds();
}


Identifiers::~Identifiers()
{
}


Identifiers* Identifiers::Instance()
{
	static Identifiers* sharedInstance = NULL;
	
	if(sharedInstance == NULL)
	{
		sharedInstance = new Identifiers();
	}
	
	return sharedInstance;
}


unsigned int Identifiers::GetId(std::string identString)
{
	std::map<std::string, unsigned int>::iterator itr = ids.find(identString);
	
	if(itr != ids.end())
	{
		return itr->second;
	}
	
	ids[identString] = nextId;
	nextId++;
	
	return nextId - 1;
}


void Identifiers::LoadRuntimeIds()
{
	ids["NULL"]			= 0;
	ids["=="]			= 1;
	ids[">"]			= 2;
	ids[">="]			= 3;
	ids["<"]			= 4;
	ids["<="]			= 5;
	ids["!="]			= 6;
	ids["+"]			= 7;
	ids["-"]			= 8;
	ids["/"]			= 9;
	ids["*"]			= 10;
	ids["^"]			= 11;
	ids["%"]			= 12;
	ids["sqrt"]			= 13;
	ids["to-float"]		= 14;
	ids["to-int"]		= 15;
	ids["to-str"]		= 16;
	ids["&&"]			= 17;
	ids["||"]			= 18;
	ids["!"]			= 19;
	ids["to-bool"]		= 20;
	ids["length"]		= 21;
	ids["includes?"]	= 22;
	ids["reverse"]		= 23;
	ids["replace"]		= 24;
	ids["remove"]		= 25;
	ids["capitalize"]	= 26;
	ids["to-upper"]		= 27;
	ids["to-lower"]		= 28;
	ids["print"]		= 29;
	ids["print-line"]	= 30;
	ids["append"]		= 31;
	ids["++"]			= 32;
	ids["sub-string"]	= 33;
	ids["split"]		= 34;
	ids["@"]			= 35;
	ids["at"]			= 36;
	ids["add!"]			= 37;
	ids["set!"]			= 38;
	ids["clear!"]		= 39;
	ids["map"]			= 40;
	ids["empty?"]		= 41;
	ids["count"]		= 42;
	ids["start"]		= 43;

	nextId = 44;
}


