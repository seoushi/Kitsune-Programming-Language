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
#include <boost/shared_ptr.hpp>


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
		Cond,
		Then,
		End,
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
		Quoted,
	}Type;
}

namespace CondType
{
	typedef enum
	{
		None,
		If,
		Elif,
		Else,
	}Type;
}


class Expression
{
	public:
		
		Expression();
		virtual ~Expression();
	
		virtual std::string ToString();
	
		virtual ExprType::Type Type() = 0;
};
typedef boost::shared_ptr<Expression> ExprPtr;

	
class EndExpr : public Expression
{
	public:
		
		EndExpr();
		virtual ~EndExpr();
		
		virtual std::string ToString();
		
		virtual ExprType::Type Type();
};
typedef boost::shared_ptr<EndExpr> EndExprPtr;
	
	
class CondExpr : public Expression
{
	public:
		
		CondExpr();
		virtual ~CondExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
	

		CondType::Type condType;
	

		ExprPtr conditional; // the expression to evaluate
	
		std::vector<ExprPtr> body;
	
		ExprPtr child; // a child is an else or elif block
	
		bool blockIsTerminated;
};
typedef boost::shared_ptr<CondExpr> CondExprPtr;


class DefExpr : public Expression
{
	public:

		DefExpr(ExprPtr expr);
		virtual ~DefExpr();
			
		virtual std::string	ToString();
			
		ExprType::Type Type();

		ExprPtr expr;
};
typedef boost::shared_ptr<DefExpr> DefExprPtr;


class AssignExpr : public Expression
{
	public:
		
		AssignExpr(std::string ident, ExprPtr expr);
		virtual ~AssignExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
	
		ExprPtr expr;
		std::string identifier;
};
typedef boost::shared_ptr<AssignExpr> AssignExprPtr;


class FuncExpr : public Expression
{
	public:

		FuncExpr();
		virtual ~FuncExpr();
				
		virtual std::string	ToString();
				
		ExprType::Type Type();


		std::vector< ExprPtr > bodyExprs;
		std::vector< std::string > args;
		std::string identifier;
};
typedef boost::shared_ptr<FuncExpr> FuncExprPtr;


class LitExpr : public Expression
{
	public:
		
		LitExpr();
		virtual ~LitExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
		
		LiteralType::Type literalType;
	
		std::string	stringValue;

		union
		{
			int			intValue;
			float		floatValue;
		};
};
typedef boost::shared_ptr<LitExpr> LitExprPtr;


class FuncCallExpr : public Expression
{
	public:

		FuncCallExpr();
		virtual ~FuncCallExpr();

		virtual std::string	ToString();

		ExprType::Type Type();

		std::string				funcName;
		ExprPtr					sender;
		std::vector<ExprPtr>	args;
};
typedef boost::shared_ptr<FuncExpr> FuncExprPtr;


class ReturnExpr : public Expression
{
	public:
		
		ReturnExpr();
		virtual ~ReturnExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
		
		ExprPtr expr;
};
typedef boost::shared_ptr<ReturnExpr> ReturnExprPtr;


class LineExpr : public Expression
{
	public:
		
		LineExpr();
		virtual ~LineExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
		
		ExprPtr expr;
};
typedef boost::shared_ptr<LineExpr> LineExprPtr;


class ThenExpr : public Expression
{
	public:
		
		ThenExpr();
		virtual ~ThenExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
		
		ExprPtr	expr;
};
typedef boost::shared_ptr<ThenExpr> ThenExprPtr;
	

class EofExpr : public Expression
{
	public:
		
		EofExpr();
		virtual ~EofExpr();
		
		virtual std::string	ToString();
		
		ExprType::Type Type();
		
		ExprPtr	expr;
};
typedef boost::shared_ptr<EofExpr> EofExprPtr;


}
	
#endif

