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


void Kitsune_PrintDebug(char* message)
{
	/*
	fprintf(stdout, "(DEBUG) %s\n", message);
	 */
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



/*
Kitsune_ResultTuple* Kitsune_Parse_Def(Kitsune_LexerData* lexer)
{
*/
/* format for a def expression is "def <IDENTIFIER> = <EXPRESSION>"
		Where expression is a funCall, function, or value*/
/*
	Kitsune_Token*			token = lexer->curToken;
	Kitsune_Token*			idToken;
	Kitsune_Expression*		expr;
	Kitsune_ResultTuple*	result;
	
	
	/* parse def */
/*	if(token->type != kitsune_tok_def)
	{
		Kitsune_ParseError("def", token, lexer, "def");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
		

	/* parse IDENTIFIER */
/*	token = Kitsune_Lex_parseNextToken(lexer);
	
	if(token->type != kitsune_tok_identifier)
	{
		Kitsune_ParseError("def", token, lexer, "identifier");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	idToken = token;
	
	
	/* parse = */
/*	token = Kitsune_Lex_parseNextToken(lexer);
	
	if(token->type != kitsune_tok_equal)
	{
		Kitsune_ParseError("def", token, lexer, "=");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	
	/* parse EXPRESSION */
	/*TODO: this should be fixed to parse funCalls, functions, or values */
/*	token = Kitsune_Lex_parseNextToken(lexer);
	result = Kitsune_Parse_Function(lexer);
	
	if(!result->succeeded)
	{
		return result;
	}
	
	/* alls well */
/*	expr = Kitsune_DefExpr_Make(idToken->data.identifier, result->expr);
	
	Kitsune_PrintDebug("Got def");
	
	return Kitsune_ResultTuple_make(expr, true);
}


Kitsune_ResultTuple* Kitsune_Parse_Function(Kitsune_LexerData* lexer)
{
	/* format for a function expression is "{ | <ARGUMENT>... | <EXPRESSION>... }" */
/*	Kitsune_ResultTuple*		result;
	Kitsune_Expression*			expr;
	Kitsune_FunctionExpr_Data*	funcData = (Kitsune_FunctionExpr_Data*)GC_MALLOC(sizeof(Kitsune_FunctionExpr_Data));
	Kitsune_Token* 				token = lexer->curToken;
	char* 						tmpText;
	bool 						running;
	
	/* parse { */
/*	if(token->type != kitsune_tok_openBrace)
	{
		Kitsune_ParseError("function", token, lexer, "{");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	
	token = Kitsune_Lex_parseNextToken(lexer);
	
	/* try parse pipe */
/*	if(token->type == kitsune_tok_pipe)
	{
		token = Kitsune_Lex_parseNextToken(lexer);
		running = true;
		
		while(running)
		{
			switch(token->type)
			{
				case kitsune_tok_identifier:
					funcData->numArgs++;
					if(funcData->numArgs == 1)
					{
						funcData->args = (char**)GC_MALLOC(sizeof(char*));
					}
					else
					{
						funcData->args = (char**)GC_REALLOC(funcData->args, funcData->numArgs * sizeof(char*));
					}
					funcData->args[funcData->numArgs - 1] = token->data.identifier;
					
					token = Kitsune_Lex_parseNextToken(lexer);
					break;
					
				case kitsune_tok_pipe:
					running = false;
					token = Kitsune_Lex_parseNextToken(lexer);
					break;
					
				default:
					Kitsune_ParseError("function", token, lexer, "identifier' or '|");
					return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
			}
		}
	}
	else
	{
		funcData->numArgs = 0;
		funcData->args = 0;
	}


	/* optionaly after the arguments (if there are any) you can have a new line */
/*	Kitsune_Parse_EatEolsAndComments(lexer);


	/* parse the body */
/*	funcData->numBodyExprs = 0;
	funcData->bodyExprs = 0;


	running = true;
	while(running)
	{
		switch(token->type)
		{
			case kitsune_tok_eof:
				Kitsune_ParseError("function", token, lexer, "expression");
				return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
				break;
			case kitsune_tok_closeBrace:
				token = Kitsune_Lex_parseNextToken(lexer);
				Kitsune_PrintDebug("Got function");
				running = false;
				break;
			case kitsune_tok_return: /* return statement */
/*				result = Kitsune_Parse_Return(lexer);
				if(!result->succeeded)
				{
					return result;
				}
				break;
			case kitsune_tok_def: /* def statement */
/*				result = Kitsune_Parse_Def(lexer);
				if(!result->succeeded)
				{
					return result;
				}
				break;
			default: /* function call statement */
/*				result = Kitsune_Parse_FuncCall(lexer);
				if(!result->succeeded)
				{
					return result;
				}
				break;
		}
		
		/* add expression to body */
/*		if(running)
		{
			funcData->numBodyExprs++;
			if(funcData->numBodyExprs == 1)
			{
				funcData->bodyExprs = (Kitsune_Expression**)GC_MALLOC(sizeof(Kitsune_Expression*));
			}
			else
			{
				funcData->bodyExprs = (Kitsune_Expression**)GC_REALLOC(funcData->bodyExprs, funcData->numBodyExprs * sizeof(Kitsune_Expression*));
			}
			funcData->bodyExprs[funcData->numBodyExprs - 1] = result->expr;		
			token = lexer->curToken;
		}
	}
	
	expr = Kitsune_Expression_Make(Kitsune_ExprType_Function);
	expr->data = funcData;
	return Kitsune_ResultTuple_make(expr, true);
}


Kitsune_ResultTuple* Kitsune_Parse_FuncCall(Kitsune_LexerData* lexer)
{
	/* format for a function call expression is "<LITERAL> <IDENTIFIER> <LITERAL>... <EOL>" */
/*	Kitsune_Token*				token = lexer->curToken;
	Kitsune_Expression*			expr;
	Kitsune_FunCallExpr_Data*	funCallData = (Kitsune_FunCallExpr_Data*)GC_MALLOC(sizeof(Kitsune_FunCallExpr_Data));
	Kitsune_LiteralExpr_Data*	litData = (Kitsune_LiteralExpr_Data*)GC_MALLOC(sizeof(Kitsune_LiteralExpr_Data));
	bool						running;
	
	/* check for literal (object) */
/*	funCallData->object = Kitsune_Expression_Make(Kitsune_ExprType_Literal);
	funCallData->object->data = litData;
	
	switch(token->type)
	{
		case kitsune_tok_identifier:		
			litData->type = Kitsune_Literal_Identifier;
			litData->data.identifier = token->data.identifier;
			break;
		case kitsune_tok_string:
			litData->type = Kitsune_Literal_String;
			litData->data.identifier = token->data.identifier;
			break;
		case kitsune_tok_int:
			litData->type = Kitsune_Literal_Int;
			litData->data.intValue = token->data.intValue;
			break;
		case kitsune_tok_float:
			litData->type = Kitsune_Literal_Float;
			litData->data.floatValue = token->data.floatValue;
			break;
		default:
			Kitsune_ParseError("function call", token, lexer, "literal");
			return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
			break;
	}
	
	
	/* check for function name */
/*	token = Kitsune_Lex_parseNextToken(lexer);
	
	if(token->type != kitsune_tok_identifier)
	{
		Kitsune_ParseError("function call", token, lexer, "identifier");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	funCallData->funName = token->data.identifier;
	
	
	/* get all arguments */
/*	funCallData->numArgs = 0;
	funCallData->args = 0;
	running = true;
	
	while(running)
	{
		litData = (Kitsune_LiteralExpr_Data*)GC_MALLOC(sizeof(Kitsune_LiteralExpr_Data));
		token = Kitsune_Lex_parseNextToken(lexer);
		
		switch(token->type)
		{
			case kitsune_tok_identifier:		
				litData->type = Kitsune_Literal_Identifier;
				litData->data.identifier = token->data.identifier;
				break;
			case kitsune_tok_string:
				litData->type = Kitsune_Literal_String;
				litData->data.identifier = token->data.identifier;
				break;
			case kitsune_tok_int:
				litData->type = Kitsune_Literal_Int;
				litData->data.intValue = token->data.intValue;
				break;
			case kitsune_tok_float:
				litData->type = Kitsune_Literal_Float;
				litData->data.floatValue = token->data.floatValue;
				break;
			case kitsune_tok_eol:
				Kitsune_Parse_EatEolsAndComments(lexer);
				running = false;
				break;
			default:
				Kitsune_ParseError("function call", token, lexer, "literal' or 'EOL");
				return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
				break;
		}
		
		if(running)
		{
			funCallData->numArgs++;
			
			if(funCallData->numArgs == 1)
			{
				funCallData->args = (Kitsune_Expression**)GC_MALLOC(sizeof(Kitsune_Expression*));
			}
			else
			{
				funCallData->args = (Kitsune_Expression**)GC_REALLOC(funCallData->args, sizeof(Kitsune_Expression*) * funCallData->numArgs);
			}
			funCallData->args[funCallData->numArgs - 1] = Kitsune_Expression_Make(Kitsune_ExprType_Literal);
			funCallData->args[funCallData->numArgs - 1]->data = litData;
		}
	}
	
	
	expr = Kitsune_Expression_Make(Kitsune_ExprType_FunCall);
	expr->data = funCallData;
	
	Kitsune_PrintDebug("Got funcCall");
	
	return Kitsune_ResultTuple_make(expr, true);
}


Kitsune_ResultTuple* Kitsune_Parse_Return(Kitsune_LexerData* lexer)
{
	/* format for a return expression is "return <LITERAL | FUNC_CALL><EOL>" */
/*	Kitsune_Token*				token = lexer->curToken;
	Kitsune_Expression*			expr = Kitsune_Expression_Make(Kitsune_ExprType_Return);
	Kitsune_ReturnExpr_Data*	returnData = (Kitsune_ReturnExpr_Data*)GC_MALLOC(sizeof(Kitsune_ReturnExpr_Data));
	Kitsune_LiteralExpr_Data*	litData = (Kitsune_LiteralExpr_Data*)GC_MALLOC(sizeof(Kitsune_LiteralExpr_Data));
	Kitsune_ResultTuple*		result;

	expr->data = returnData;


	/* check for return token */
/*	if(token->type != kitsune_tok_return)
	{
		Kitsune_ParseError("return", token, lexer, "return");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	

	/* check for literal or function call */
/*	token = Kitsune_Lex_parseNextToken(lexer);
	
	switch(token->type)
	{
		case kitsune_tok_identifier: /* this will always be a function call */
/*			result = Kitsune_Parse_FuncCall(lexer);
			if(!result->succeeded)
			{
				return result;
			}
			returnData->expr = result->expr;
			return Kitsune_ResultTuple_make(expr, true);
			break;
		case kitsune_tok_string: /* this can either be a function call or a straight literal, for now it is only a literal */
/*			litData->type = Kitsune_Literal_String;
			litData->data.identifier = token->data.identifier;
			returnData->expr = Kitsune_Expression_Make(Kitsune_ExprType_Literal);
			returnData->expr->data = litData;
			break;
		case kitsune_tok_int: /* this can either be a function call or a straight literal, for now it is only a literal */
/*			litData->type = Kitsune_Literal_Int;
			litData->data.intValue = token->data.intValue;
			returnData->expr = Kitsune_Expression_Make(Kitsune_ExprType_Literal);
			returnData->expr->data = litData;
			break;
		case kitsune_tok_float: /* this can either be a function call or a straight literal, for now it is only a literal */
/*			litData->type = Kitsune_Literal_Float;
			litData->data.floatValue = token->data.floatValue;
			returnData->expr = Kitsune_Expression_Make(Kitsune_ExprType_Literal);
			returnData->expr->data = litData;
		default:
			Kitsune_ParseError("return", token, lexer, "literal' or 'function call");
			return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
			break;
	}
	
	/* check for EOL */
/*	token = Kitsune_Lex_parseNextToken(lexer);
	
	if((token->type != kitsune_tok_eol) && (token->type != kitsune_tok_comment))
	{
		Kitsune_ParseError("return", token, lexer, "EOL' or 'COMMENT");
		return Kitsune_ResultTuple_make(Kitsune_Expression_Make(Kitsune_ExprType_Eof), false);
	}
	
	Kitsune_Parse_EatEolsAndComments(lexer);
	
	Kitsune_PrintDebug("Got return");
	
	return Kitsune_ResultTuple_make(expr, true);
}


*/