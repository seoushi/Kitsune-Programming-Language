/*
 *  expressions.hpp
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

#ifndef KITC_EXPRESSIONS_HPP
#define	KITC_EXPRESSIONS_HPP

#include "lexer.hpp"

#include <string>
#include <vector>


namespace kitc
{


namespace ExprType
{
	typedef enum
	{		
		None,
		Eof,
		Def,
		Function,
		FuncCall,
		Literal,
		Return,
		Line,
		Assign,
	}Type;
}


namespace LiteralType
{
	typedef enum
	{
		None,
		Identifier,
		String,
		Int,
		Float,
	}Type;
}



class Expression
{
	public:
		
		Expression();
		virtual ~Expression();
	
		virtual std::string	toString();
	
		ExprType::Type Type();
};


class DefExpr : public Expression
{
	public:

		DefExpr(Expression* expr);
		virtual ~DefExpr();
			
		virtual std::string	toString();
			
		ExprType::Type Type();


		Expression* expr;
};


class AssignExpr : public Expression
{
	public:
		
		AssignExpr(Expression* expr);
		virtual ~AssignExpr();
		
		virtual std::string	toString();
		
		ExprType::Type Type();
	
		Expression* expr;
		std::string identifer;
};


class FuncExpr : public Expression
{
	public:
			
		FuncExpr();
		virtual ~FuncExpr();
				
		virtual std::string	toString();
				
		ExprType::Type Type();


		std::vector< Expression* > bodyExprs;
		std::vector< std::string > args;
		std::string identifer;
};


class LitExpr : public Expression
{
	public:
		
		LitExpr();
		virtual ~LitExpr();
		
		virtual std::string	toString();
		
		ExprType::Type Type();
		
	
		LiteralType::Type literalType;
	
	

		std::string	stringValue;

		union
		{
			int			intValue;
			float		floatValue;
		};
};


class FuncCallExpr : public Expression
{
	public:

		FuncCallExpr();
		virtual ~FuncCallExpr();

		virtual std::string	toString();

		ExprType::Type Type();

		std::string					funcName;
		Expression*					sender;
		std::vector<Expression*>	args;
};


class ReturnExpr : public Expression
{
	public:
		
		ReturnExpr();
		virtual ~ReturnExpr();
		
		virtual std::string	toString();
		
		ExprType::Type Type();
		
		Expression* expr;
};


class LineExpr : public Expression
{
	public:
		
		LineExpr();
		virtual ~LineExpr();
		
		virtual std::string	toString();
		
		ExprType::Type Type();
		
		Expression*	expr;
};


}
	
#endif
