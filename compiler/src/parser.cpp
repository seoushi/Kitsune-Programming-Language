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

#include "parser.hpp"

#include <sstream>
#include <iostream>

using namespace kitc;

Parser::Parser(LexerPtr lexer)
{
	encounteredError = false;
	this->lexer = lexer;
}

Parser::~Parser()
{
}

ExprPtr Parser::Parse()
{
	std::vector<ExprPtr> stack;
	
	lexer->ParseNextToken();
	TokenPtr token = lexer->CurToken();

parse_next:
	
	switch(token->type)
	{
		// end of file
		case TokenType::Eof:
			if(stack.size() > 0)
			{
				ParseError(token, "Dot", "Unexpected end of file");
				return ExprPtr();
			}

			return ExprPtr(new EofExpr());
			
		// end line marker
		case TokenType::Dot:
			switch(stack.size())
			{
				case 0:
					ParseError(token, "one expression", "No expression before the dot");
					return ExprPtr();
					break;
				case 1:
					{
						LineExpr* tmpExpr = new LineExpr();
						tmpExpr->expr = stack[0];
						return ExprPtr(tmpExpr);
					}
					break;
				default:
					ParseError(token, "one expression", "too many expressions before the dot");
					return ExprPtr();
					break;
			}
			break;
		// then
		case TokenType::Then:
			switch(stack.size())
			{
				case 0:
					ParseError(token, "one expression", "No expression before 'then'");
					return ExprPtr();
					break;
				case 1:
					{
						ThenExpr* tmpExpr = new ThenExpr();
						tmpExpr->expr = stack[0];
						return ExprPtr(tmpExpr);
					}
				break;
			default:
				ParseError(token, "one expression", "too many expressions before 'then'");
				return ExprPtr();
				break;
			}
			break;
		// assignment
		case TokenType::Equal:
			if(stack.size() == 0)
			{
				ParseError(token, "an identifer", "assignment statement without something to assign it to");
				return ExprPtr();
			}
			if(stack.size() > 1)
			{
				ParseError(token, "an identifer", "assignment statement should only have one identifier to assign to");
				return ExprPtr();
			}
			else
			{
				// we can only assign to an identifier
				if(	(stack[0]->Type() == ExprType::Literal) &&
				   (((LitExpr*)stack[0].get())->literalType == LiteralType::Identifier))
				{
					LitExpr* lit = (LitExpr*)stack[0].get();
					
					ExprPtr result = Parse();
					if(encounteredError || (result->Type() != ExprType::Line))
					{
						return ExprPtr();
					}
					
					LineExpr* line = (LineExpr*)result.get();
					
					if(line->expr->Type() == ExprType::Def)
					{
						ParseError(token, "not def", "def can only be used to start a statement");
						return ExprPtr();
					}
					
					LineExpr* newLine = new LineExpr();
					newLine->expr = ExprPtr(new AssignExpr(lit->stringValue, line->expr));

					return ExprPtr(newLine);
				}
				else
				{
					ParseError(token, "an identifer", "assignment statement should have an identifier on the left side of the equals");
					return ExprPtr();
				}
			}
			
			break;
		// defining
		case TokenType::Def:
			if(stack.size() == 0)
			{
				ExprPtr result = Parse();

				if(encounteredError || (result->Type() != ExprType::Line))
				{
					return ExprPtr();
				}
				
				LineExpr* line = (LineExpr*)result.get();

				
				LineExpr* newLine = new LineExpr();
				newLine->expr = ExprPtr(new DefExpr(line->expr));

				return ExprPtr(newLine);
			}
			else
			{
				ParseError(token, "not def", "def can only be used to start a statement");
				return ExprPtr();
			}
			// add literals to the stack
		case TokenType::Identifier:
		case TokenType::String:
		case TokenType::Int:
		case TokenType::Float:
			{
				LitExpr* tmpExpr = new LitExpr();
			
				switch(token->type)
				{
					case TokenType::Identifier:
						if(token->identifier[0] == '\'')
						{
							tmpExpr->literalType = LiteralType::Quoted;
							tmpExpr->stringValue = token->identifier.substr(1); //drop the quote
						}
						else
						{
							tmpExpr->literalType = LiteralType::Identifier;
							tmpExpr->stringValue = token->identifier;
						}
						break;
					case TokenType::String:
						tmpExpr->literalType = LiteralType::String;
						tmpExpr->stringValue = token->identifier;
						break;
					case TokenType::Int:
						tmpExpr->literalType = LiteralType::Int;
						tmpExpr->intValue = token->intValue;
						break;
					case TokenType::Float:
						tmpExpr->literalType = LiteralType::Float;
						tmpExpr->floatValue = token->floatValue;
						break;
				}
				
				stack.push_back(ExprPtr(tmpExpr));
			}
			break;
		case TokenType::End:
			if(stack.size() > 0)
			{
				ParseError(token, "not end", "end can only be used to start a statement");
				return ExprPtr();
			}
			return ExprPtr(new EndExpr());
			break;
		case TokenType::Else:
		case TokenType::Elif:
			{
				if(stack.size() == 0)
				{
					CondExpr* condExpr = new CondExpr();
					
					if(token->type == TokenType::Else)
					{
						condExpr->condType = CondType::Else;
					}
					else if(token->type == TokenType::Elif)
					{
						condExpr->condType = CondType::Elif;
					}
					
					
					return ExprPtr(condExpr);
				}
				if(stack.size() > 1)
				{
					ParseError(token, "one cond expression", "too many expressions before else statement, expected right after elif or if");
					return ExprPtr();
				}
				
				//get the last child
				ExprPtr tmpExpr = stack[0];
				
				if(tmpExpr->Type() != ExprType::Cond)
				{
					ParseError(token, "cond expression", "expected previous statement to be if or elif");
					return ExprPtr();
				}
				
				CondExpr* cond = (CondExpr*)tmpExpr.get();
				
				while(true)
				{
					if(cond->child)
					{
						cond = (CondExpr*)cond->child.get();
					}
					else
					{
						break;
					}
				}
				
				//make sure that the previous statement is an elif or if
				if(cond->condType == CondType::Else)
				{
					ParseError(token, "cond expression", "expected previous statement to be if or elif");
					return ExprPtr();
				}
				
				// make the cond else expr
				CondExpr* elseCond = new CondExpr();
				
				if(token->type == TokenType::Else)
				{
					elseCond->condType = CondType::Else;
				}
				else if(token->type == TokenType::Elif)
				{
					elseCond->condType = CondType::Elif;
					
					//get conditional, this should return a then expression
					ExprPtr thenExpr = Parse();

				
					if(thenExpr.get() == NULL)
					{
						ParseError(token, "boolean expression", "missing conditional in if statement");
						return ExprPtr();
					}
					else if(thenExpr->Type() != ExprType::Then)
					{
						ParseError(token, "then", "missing 'then' in if statement");
						return ExprPtr();
					}

					elseCond->conditional = ExprPtr(((ThenExpr*)thenExpr.get())->expr);
				}
				
				
				// parse the body of the else statement
				tmpExpr = Parse();
				
				while (tmpExpr)
				{
					if(tmpExpr->Type() == ExprType::End)
					{
						cond->child = ExprPtr(elseCond);
						return stack[0];
					}
					if(tmpExpr->Type() == ExprType::Cond)
					{
						cond->child = ExprPtr(elseCond);
						token = lexer->CurToken();
						
						goto parse_next;
					}
					
					elseCond->body.push_back(tmpExpr);
					
					tmpExpr = Parse();
				}
				
			}
			break;
		case TokenType::If:
			{
				if(stack.size() > 0)
				{
					ParseError(token, "not if", "if can only be used to start a statement");
					return ExprPtr();
				}
				
				CondExpr* tmpExpr = new CondExpr();
				tmpExpr->condType = CondType::If;
				
				//get conditional, this should return a then expression
				ExprPtr thenExpr = Parse();

				
				if(thenExpr.get() == NULL)
				{
					ParseError(token, "boolean expression", "missing conditional in if statement");
					return ExprPtr();
				}
				else if(thenExpr->Type() != ExprType::Then)
				{
					ParseError(token, "then", "missing 'then' in if statement");
					return ExprPtr();
				}

				tmpExpr->conditional = ((ThenExpr*)thenExpr.get())->expr;
				
				
				// parse the body till end
				ExprPtr bodyExpr = Parse();
				
				while (bodyExpr)
				{
					if(bodyExpr->Type() == ExprType::End)
					{
						return ExprPtr(tmpExpr);
					}
					if(bodyExpr->Type() == ExprType::Cond)
					{
						stack.push_back(ExprPtr(tmpExpr));
						token = lexer->CurToken();
						
						goto parse_next;
					}
					
					tmpExpr->body.push_back(bodyExpr);

					bodyExpr = Parse();
				}
				
			}
			break;

		default:
			ParseError(token, "?", "unknown or used token type");
			return ExprPtr();
			break;
	}

	// try and reduce statement
	StackReduce(&stack);
	
	lexer->ParseNextToken();
	token = lexer->CurToken();
	
	goto parse_next;
	
	
	// this will never happen but I want to make the compiler happy anyways
	return ExprPtr();
}


bool Parser::EncounteredError()
{
	return encounteredError;
}


void Parser::ParseError(TokenPtr token, std::string expected, std::string additional)
{
	encounteredError = true;
	
	std::stringstream ss;
	
	ss << lexer->PositionString() << " Error got '" << token->ToString() << "' expected: " << expected << ".";
	
	if(additional.length() > 0)
	{
		ss << " " << additional << ".";
	}
	
	std::cerr << ss.str() << std::endl;;
}



void Parser::StackReduce(std::vector<ExprPtr>* stack)
{
	if(stack->size() < 2)
	{
		return;
	}
	
	// <lit> <lit-id>
	if(((*stack)[0]->Type() == ExprType::Literal) &&
	   ((*stack)[1]->Type() == ExprType::Literal) &&
	   (((LitExpr*)((*stack)[1].get()))->literalType == LiteralType::Identifier))
	{
		FuncCallExpr* tmpExpr = new FuncCallExpr();
		tmpExpr->sender = (*stack)[0];
		tmpExpr->funcName = ((LitExpr*)((*stack)[1]).get())->stringValue;
		
		// remove both items on the stack
		stack->pop_back();
		stack->pop_back();
		
		stack->push_back(ExprPtr(tmpExpr));
		return;
	}
	// <funCall> <lit>
	else if(((*stack)[0]->Type() == ExprType::FuncCall) &&
			((*stack)[1]->Type() == ExprType::Literal))
	{
		FuncCallExpr* funcCall = (FuncCallExpr*)((*stack)[0].get());
		LitExpr* literal = (LitExpr*)((*stack)[1].get());
		
		// <funCall> <lit-id>
		if(literal->literalType == LiteralType::Identifier)
		{
			FuncCallExpr* tmpExpr = new FuncCallExpr();
			tmpExpr->sender = (*stack)[0];
			tmpExpr->funcName = ((LitExpr*)((*stack)[1].get()))->stringValue;

			// remove both items on the stack
			stack->pop_back();
			stack->pop_back();
			
			stack->push_back(ExprPtr(tmpExpr));
			
			return;
		}
		// <funCall> <lit-value>
		else
		{
			funcCall->args.push_back((*stack)[1]);
			
			// remove lit item from the stack
			stack->pop_back();
			return;
		}
	}
}


