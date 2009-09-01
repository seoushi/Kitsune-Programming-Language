/*
 *  main.c
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

#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include <gc/gc.h>


void lexTest(char* filename)
{
	Kitsune_LexerData*	lexer = Kitsune_Lex_make();
	Kitsune_Token*		token;
	
	if(Kitsune_Lex_openFile(lexer, filename))
	{
		while(true)
		{
			token = Kitsune_Lex_parseNextToken(lexer);
		
			printf("%s\n", Kitsune_Token_toString(token));
			
			if(token->type == kitsune_tok_eof)
			{
				break;
			}
		}
	}
}

void parseTest(char* filename)
{
	Kitsune_LexerData*		lexer = Kitsune_Lex_make();
	Kitsune_ResultTuple*	result;
	
	if(Kitsune_Lex_openFile(lexer, filename))
	{
		while(true)
		{
			result = Kitsune_Parse_TopLevel(lexer);
			
			if((!result->succeeded) || (result->expr->type == Kitsune_ExprType_Eof))
			{
				break;
			}
		}
	}
}

void generateTest(char* filename)
{
	Kitsune_Generate(filename);
}

int main(int argc, char** argv)
{
	int i;
	
	GC_INIT();
	
	for(i = 1; i < argc; i++)
	{
		printf("Processing %s \n", argv[i]);
		/* lexTest(); */
		/* parseTest(argv[i]); */
		generateTest(argv[i]);
	}
	
	return 0;
}