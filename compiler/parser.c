/*
 *  parser.c
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

#include "parser.h"

#include <gc/gc.h>

void Kitsune_ParseError(char* exprType, Kitsune_Token* token, Kitsune_LexerData* lexer, char* expected)
{
	fprintf(stderr, "(%s) Error while parsing %s expression Got '%s', Expected: '%s' \n", 
				Kitsune_Lex_positionStr(lexer),
				exprType,
				Kitsune_Token_toString(token),
				expected);
}


void Kitsune_PrintDebug(char* message)
{
	fprintf(stdout, "(DEBUG) %s\n", message);
}


Kitsune_ResultTuple* Kitsune_ResultTuple_make(Kitsune_Expression* expr, bool succeeded)
{
	Kitsune_ResultTuple* result = (Kitsune_ResultTuple*)GC_MALLOC(sizeof(Kitsune_ResultTuple));
	result->expr = expr;
	result->succeeded = succeeded;
	
	return result;
}


void Kitsune_Parse_EatEolsAndComments(Kitsune_LexerData* lexer)
{
	Kitsune_Token* token = lexer->curToken;

	while( (token->type == kitsune_tok_comment) || (token->type == kitsune_tok_eol))
	{
		if(token->type == kitsune_tok_eof)
		{
			return;
		}

		token = Kitsune_Lex_parseNextToken(lexer);
	}
}


Kitsune_ResultTuple* Kitsune_Parse_TopLevel(Kitsune_LexerData* lexer)
{
	Kitsune_Token* token = Kitsune_Lex_parseNextToken(lexer);

	Kitsune_Parse_EatEolsAndComments(lexer);

	token = lexer->curToken;
	
	switch(token->type)
	{
		case kitsune_tok_def:
			return Kitsune_Parse_Def(lexer);
	        break;
	
		case kitsune_tok_eof:
			return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), true);
			break;

		default:
			break;
	}

	Kitsune_ParseError("top-level", token, lexer, "def");
	
	return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
}


Kitsune_ResultTuple* Kitsune_Parse_Def(Kitsune_LexerData* lexer)
{
	/* format for a def expression is "def <IDENTIFIER> = <EXPRESSION>"
		Where expression is a funCall, function, or value*/

	Kitsune_Token*			token = lexer->curToken;
	Kitsune_Token*			idToken;
	Kitsune_Expression*		expr;
	Kitsune_ResultTuple*	result;
	
	
	/* parse def */
	if(token->type != kitsune_tok_def)
	{
		Kitsune_ParseError("def", token, lexer, "def");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
		

	/* parse IDENTIFIER */
	token = Kitsune_Lex_parseNextToken(lexer);
	
	if(token->type != kitsune_tok_identifier)
	{
		Kitsune_ParseError("def", token, lexer, "identifier");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	idToken = token;
	
	
	/* parse = */
	token = Kitsune_Lex_parseNextToken(lexer);
	
	if(token->type != kitsune_tok_equal)
	{
		Kitsune_ParseError("def", token, lexer, "=");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	
	/* parse EXPRESSION */
	/*TODO: this should be fixed to parse funCalls, functions, or values */
	token = Kitsune_Lex_parseNextToken(lexer);
	result = Kitsune_Parse_Function(lexer);
	
	if(!result->succeeded)
	{
		return result;
	}
	
	/* alls well */
	expr = Kitsune_DefExpr_Make(idToken->data.identifier, result->expr);
	
	Kitsune_PrintDebug("Got def");
	
	return Kitsune_ResultTuple_make(expr, true);
}


Kitsune_ResultTuple* Kitsune_Parse_Function(Kitsune_LexerData* lexer)
{
	/* format for a function expression is "{ | <ARGUMENT>... | <EXPRESSION>... }" */
	
	/* for now just eat everything */
	Kitsune_Token* token = lexer->curToken;
	
	while(true)
	{
		switch(token->type)
		{
			case kitsune_tok_eof:
				return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
				break;
			case kitsune_tok_closeBrace:
				token = Kitsune_Lex_parseNextToken(lexer);
				Kitsune_PrintDebug("Got function");
				return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), true);
			default:
				break;
		}
		
		token = Kitsune_Lex_parseNextToken(lexer);
	}
}
