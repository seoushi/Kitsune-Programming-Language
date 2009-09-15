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


/*
Kitsune_ResultTuple* Kitsune_Parse_Def(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_Function(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_FuncCall(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_Return(Kitsune_LexerData* lexer);
*/

void Kitsune_ParseError(Kitsune_Token* token, Kitsune_LexerData* lexer, char* expected, char* additional)
{
	fprintf(stderr, "(%s) Error Got '%s', Expected: '%s' \n", 
				Kitsune_Lex_positionStr(lexer),
				Kitsune_Token_toString(token),
				expected);

	if(additional)
	{
		fprintf(stderr, "\t%s\n", additional);
	}
}


Kitsune_ResultTuple* Kitsune_ResultTuple_make(Kitsune_Expression* expr, bool succeeded)
{
	Kitsune_ResultTuple* result = (Kitsune_ResultTuple*)GC_MALLOC(sizeof(Kitsune_ResultTuple));
	result->expr = expr;
	result->succeeded = succeeded;
	
	return result;
}


void Kitsune_Parse_stackPush(Kitsune_Expression*** stack, int* numItems, Kitsune_Expression* expr)
{
	(*numItems)++;

	if((*numItems) == 1)
	{
		(*stack) = (Kitsune_Expression**)GC_MALLOC(sizeof(Kitsune_Expression*));
	}
	else
	{
		(*stack) = (Kitsune_Expression**)GC_REALLOC((*stack), sizeof(Kitsune_Expression*) * (*numItems));
	}
	
	(*stack)[(*numItems) - 1] = expr;
}


void Kitsune_Parse_stackReduce(Kitsune_Expression*** stack, int* numItems)
{
	Kitsune_Expression*			tmpExpr;
	Kitsune_FunCallExpr_Data*	funCallData;

	if((*numItems) < 2)
	{
		return;
	}

	/* <lit> <lit-id> */
	if( ((*stack)[0]->type == Kitsune_ExprType_Literal) &&
		((*stack)[1]->type == Kitsune_ExprType_Literal) &&
		((((Kitsune_LiteralExpr_Data*)(*stack)[1]->data))->type == Kitsune_Literal_Identifier))
	{
		tmpExpr = Kitsune_Expression_Make(Kitsune_ExprType_FunCall);
		funCallData = (Kitsune_FunCallExpr_Data*)GC_MALLOC(sizeof(Kitsune_FunCallExpr_Data));
		funCallData->args = 0;
		funCallData->numArgs = 0;
		funCallData->object = (*stack)[0];
		funCallData->funName = (((Kitsune_LiteralExpr_Data*)(*stack)[1]->data))->data.identifier;
		tmpExpr->data = funCallData;

		(*stack)[0] = tmpExpr;
		(*stack)[1] = NULL;
		(*numItems)--;
		return;
	}
	/* <funCall> <lit> */
	else if(	((*stack)[0]->type == Kitsune_ExprType_FunCall) &&
				((*stack)[1]->type == Kitsune_ExprType_Literal))
	{
		/* <funCall> <lit-id> */
		if((((Kitsune_LiteralExpr_Data*)(*stack)[1]->data))->type == Kitsune_Literal_Identifier)
		{
			tmpExpr = Kitsune_Expression_Make(Kitsune_ExprType_FunCall);
			funCallData = (Kitsune_FunCallExpr_Data*)GC_MALLOC(sizeof(Kitsune_FunCallExpr_Data));
			funCallData->args = 0;
			funCallData->numArgs = 0;
			funCallData->object = (*stack)[0];
			funCallData->funName = (((Kitsune_LiteralExpr_Data*)(*stack)[1]->data))->data.identifier;
			tmpExpr->data = funCallData;

			(*stack)[0] = tmpExpr;
			(*stack)[1] = NULL;
			(*numItems)--;
			return;
		}
		/* <funCall> <lit-value> */
		else if((((Kitsune_LiteralExpr_Data*)(*stack)[1]->data))->type != Kitsune_Literal_Identifier)
		{
			funCallData = (*stack)[0]->data;
			
			funCallData->numArgs++;
			
			if(funCallData->numArgs == 1)
			{
				funCallData->args = (Kitsune_Expression**)GC_MALLOC(sizeof(Kitsune_Expression*));
			}
			else
			{
				funCallData->args = (Kitsune_Expression**)GC_REALLOC(funCallData->args, sizeof(Kitsune_Expression*) * funCallData->numArgs);
			}
			funCallData->args[funCallData->numArgs - 1] = (*stack)[1];

			(*numItems)--;
			return;
		}
	}
}

Kitsune_ResultTuple* Kitsune_Parse_topLevel(Kitsune_LexerData* lexer)
{
	Kitsune_Token*				token = Kitsune_Lex_parseNextToken(lexer);
	Kitsune_ResultTuple*		result;
	Kitsune_Expression**		exprStack;
	int							exprStackSize = 0;
	Kitsune_Expression*			tmpExpr;
	Kitsune_LiteralExpr_Data*	tmpLitData;
	Kitsune_LineExpr_Data*		tmpLineData;

	token = lexer->curToken;
	
parse_next:
	
	switch(token->type)
	{
		/* end of file */
		case kitsune_tok_eof:
			if(exprStackSize > 0)
			{
				Kitsune_ParseError(token, lexer, "dot", "missing dot after expression.");
				return Kitsune_ResultTuple_make(NULL, false);
			}
			return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), true);

		/* end line marker */
		case kitsune_tok_dot:
			switch(exprStackSize)
			{
				case 0:
					Kitsune_ParseError(token, lexer, "one expression", "No expression before the dot.");
					return Kitsune_ResultTuple_make(NULL, false);
				case 1:
					tmpExpr = Kitsune_Expression_Make(Kitsune_ExprType_Line);
					tmpLineData = (Kitsune_LineExpr_Data*)GC_MALLOC(sizeof(Kitsune_LineExpr_Data));
					tmpLineData->expr = exprStack[0];
					tmpExpr->data = tmpLineData;
					return Kitsune_ResultTuple_make(tmpExpr, true);
				default:
					Kitsune_ParseError(token, lexer, "one expression", "too many expressions before the dot.");
					return Kitsune_ResultTuple_make(NULL, false);
			}
			break;
		/* assignment */
		case kitsune_tok_equal:
			if(exprStackSize == 0)
			{
				Kitsune_ParseError(token, lexer, "an identifer", "assignment statement without something to assign it to.");
				return Kitsune_ResultTuple_make(NULL, false);
			}
			if(exprStackSize > 1)
			{
				Kitsune_ParseError(token, lexer, "an identifer", "assignment statement should only have one identifier to assign to.");
				return Kitsune_ResultTuple_make(NULL, false);
			}
			else
			{
				/* we can only assign to an identifier */
				if(	(exprStack[0]->type == Kitsune_ExprType_Literal) &&
					((((Kitsune_LiteralExpr_Data*)exprStack[0]->data))->type == Kitsune_Literal_Identifier))
				{

					tmpLitData = (Kitsune_LiteralExpr_Data*)exprStack[0]->data;

					result = Kitsune_Parse_topLevel(lexer);
					if(!result->succeeded)
					{
						return result;
					}

					if(((Kitsune_LineExpr_Data*)result->expr->data)->expr->type == Kitsune_ExprType_Def)
					{
						Kitsune_ParseError(token, lexer, "not def", "def can only be used to start a statement.");
						return Kitsune_ResultTuple_make(NULL, false);
					}

					tmpExpr = Kitsune_Expression_Make(Kitsune_ExprType_Line);
					tmpLineData = (Kitsune_LineExpr_Data*)GC_MALLOC(sizeof(Kitsune_LineExpr_Data));
					tmpLineData->expr = Kitsune_AssignExpr_Make(tmpLitData->data.identifier, ((Kitsune_LineExpr_Data*)result->expr->data)->expr );
					tmpExpr->data = tmpLineData;
					return Kitsune_ResultTuple_make(tmpExpr, true);
				}
				else
				{
					Kitsune_ParseError(token, lexer, "an identifer", "assignment statement should have an identifier on the left side of the equals.");
					return Kitsune_ResultTuple_make(NULL, false);
				}
			}
			
			break;
		/* defining */
		case kitsune_tok_def:
			if(exprStackSize == 0)
			{
				result = Kitsune_Parse_topLevel(lexer);
				if(!result->succeeded)
				{
					return result;
				}

				tmpExpr = Kitsune_Expression_Make(Kitsune_ExprType_Line);
				tmpLineData = (Kitsune_LineExpr_Data*)GC_MALLOC(sizeof(Kitsune_LineExpr_Data));
				tmpLineData->expr = Kitsune_DefExpr_Make(((Kitsune_LineExpr_Data*)result->expr->data)->expr );
				tmpExpr->data = tmpLineData;
				return Kitsune_ResultTuple_make(tmpExpr, true);
			}
			else
			{
				Kitsune_ParseError(token, lexer, "not def", "def can only be used to start a statement.");
				return Kitsune_ResultTuple_make(NULL, false);
			}
		/* add literals to the stack */
		case kitsune_tok_identifier:
		case kitsune_tok_string:
		case kitsune_tok_int:
		case kitsune_tok_float:
			tmpExpr = Kitsune_Expression_Make(Kitsune_ExprType_Literal);
			tmpLitData = (Kitsune_LiteralExpr_Data*)GC_MALLOC(sizeof(Kitsune_LiteralExpr_Data));
			tmpExpr->data = tmpLitData;
			
			switch(token->type)
			{
				case kitsune_tok_identifier:
					tmpLitData->type = Kitsune_Literal_Identifier;
					tmpLitData->data.identifier = token->data.identifier;
					break;
				case kitsune_tok_string:
					tmpLitData->type = Kitsune_Literal_String;
					tmpLitData->data.identifier = token->data.identifier;
					break;
				case kitsune_tok_int:
					tmpLitData->type = Kitsune_Literal_Int;
					tmpLitData->data.intValue = token->data.intValue;
					break;
				case kitsune_tok_float:
					tmpLitData->type = Kitsune_Literal_Float;
					tmpLitData->data.floatValue = token->data.floatValue;
					break;
			}

			Kitsune_Parse_stackPush(&exprStack, &exprStackSize, tmpExpr);
			break;

		default:
			Kitsune_ParseError(token, lexer, "?", "unknown or used token type.");
			return Kitsune_ResultTuple_make(NULL, false);
			break;
	}


	/* try and reduce statement */
	Kitsune_Parse_stackReduce(&exprStack, &exprStackSize);

	token = Kitsune_Lex_parseNextToken(lexer);

	goto parse_next;


	/* this will never happen but I want to make the compiler happy anyways */
	return NULL;
}
