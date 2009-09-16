/*
 *  expressions.cpp
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

namespace kitc
{
	
/*	
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
*/

///////////////////////////////////////////
/// CLASS Expression
///////////////////////////////////////////
Expression::Expression(){}


Expression::~Expression(){}


std::string Expression::toString()
{
	return "Base Class Expression";
}


ExprType::Type Expression::Type()
{
	return ExprType::None;
}


///////////////////////////////////////////
/// Def Expression
///////////////////////////////////////////
DefExpr::DefExpr(Expression* expr)
{
	this->expr = expr;
}


DefExpr::~DefExpr(){}


std::string DefExpr::toString()
{
	return "<Def Expression>";
}


ExprType::Type DefExpr::Type()
{
	return ExprType::Def;
}


///////////////////////////////////////////
/// Assign Expression
///////////////////////////////////////////
AssignExpr::AssignExpr(Expression* expr)
{
	this->expr = expr;
	identifer = "default";
}


AssignExpr::~AssignExpr(){}


std::string AssignExpr::toString()
{
	return "<Assign Expression>";
}


ExprType::Type AssignExpr::Type()
{
	return ExprType::Assign;
}


///////////////////////////////////////////
/// Function Expression
///////////////////////////////////////////
FuncExpr::FuncExpr()
{
	identifer = "default";
}


FuncExpr::~FuncExpr(){}


std::string FuncExpr::toString()
{
	return "<Function Expression>";
}


ExprType::Type FuncExpr::Type()
{
	return ExprType::Function;
}


///////////////////////////////////////////
/// Literal Expression
///////////////////////////////////////////
LitExpr::LitExpr()
{
	literalType = LiteralType::None;
}


LitExpr::~LitExpr(){}


std::string LitExpr::toString()
{
	return "<Literal Expression>";
}


ExprType::Type LitExpr::Type()
{
	return ExprType::Literal;
}


///////////////////////////////////////////
/// Function Call Expression
///////////////////////////////////////////
FuncCallExpr::FuncCallExpr(){}


FuncCallExpr::~FuncCallExpr(){}


std::string FuncCallExpr::toString()
{
	return "<Function Call Expression>";
}


ExprType::Type FuncCallExpr::Type()
{
	return ExprType::FuncCall;
}


///////////////////////////////////////////
/// Returnl Expression
///////////////////////////////////////////
ReturnExpr::ReturnExpr(){}


ReturnExpr::~ReturnExpr(){}


std::string ReturnExpr::toString()
{
	return "<Return Expression>";
}


ExprType::Type ReturnExpr::Type()
{
	return ExprType::Return;
}


///////////////////////////////////////////
/// Returnl Expression
///////////////////////////////////////////
LineExpr::LineExpr(){}


LineExpr::~LineExpr(){}


std::string LineExpr::toString()
{
	return "<Line Expression>";
}


ExprType::Type LineExpr::Type()
{
	return ExprType::Line;
}


}
