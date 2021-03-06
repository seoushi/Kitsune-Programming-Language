/*
 *  main.cpp
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

#include "expressions.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "generator.hpp"
#include <string.h>



void generate(char* filename, int argc, char** argv)
{
	std::string ccOpts;
	
	for(int i = 2; i < argc; i++)
	{
		ccOpts += std::string(argv[i]);
		ccOpts += " ";
	}

	kitc::Generator* gen = new kitc::Generator();	
	gen->Generate(filename, ccOpts);
}

int main(int argc, char** argv)
{
	if(argc >= 2)
	{
		if((strcmp("help", argv[1]) == 0) || (strcmp("HELP", argv[1]) == 0))
		{
			printf("Usage is: kitsunec <file.kit> <C Compile Options> \n");
		}
		else
		{
			printf("Processing %s \n", argv[1]);
			generate(argv[1], argc, argv);
		}
	}
	else
	{
		printf("invalid usage. The proper format is:  kitsunec <file.kit> <C Compile Options> \n");
	}
	
	return 0;
}
