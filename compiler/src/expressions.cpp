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


///////////////////////////////////////////
/// CLASS Expression
///////////////////////////////////////////
Expression::Expression(){}


Expression::~Expression(){}


std::string Expression::ToString()
{
	return "Base Class Expression";
}


/*ExprType::Type Expression::Type()
{
	return ExprType::None;
}*/
	

///////////////////////////////////////////
/// End Expression
///////////////////////////////////////////

EndExpr::EndExpr(){}
	
EndExpr::~EndExpr(){}


std::string EndExpr::ToString()
{
	return "<EndExpr>";
}


ExprType::Type EndExpr::Type()
{
	return ExprType::End;
}


///////////////////////////////////////////
/// Conditional Expression
///////////////////////////////////////////

CondExpr::CondExpr()
{
	condType = CondType::None;
	child = NULL;
	conditional = NULL;
	blockIsTerminated = false;
}

CondExpr::~CondExpr(){}
		
std::string CondExpr::ToString()
{
	return "<Conditional Expression>";
}
		
ExprType::Type CondExpr::Type()
{
	return ExprType::Cond;
}


///////////////////////////////////////////
/// Def Expression
///////////////////////////////////////////
DefExpr::DefExpr(Expression* expr)
{
	this->expr = expr;
}


DefExpr::~DefExpr(){}


std::string DefExpr::ToString()
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
AssignExpr::AssignExpr(std::string ident, Expression* expr)
{
	this->identifier = ident;
	this->expr = expr;
}


AssignExpr::~AssignExpr(){}


std::string AssignExpr::ToString()
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
	identifier = "default";
}


FuncExpr::~FuncExpr(){}


std::string FuncExpr::ToString()
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


std::string LitExpr::ToString()
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


std::string FuncCallExpr::ToString()
{
	return "<Function Call Expression>";
}


ExprType::Type FuncCallExpr::Type()
{
	return ExprType::FuncCall;
}


///////////////////////////////////////////
/// Return Expression
///////////////////////////////////////////
ReturnExpr::ReturnExpr(){}


ReturnExpr::~ReturnExpr(){}


std::string ReturnExpr::ToString()
{
	return "<Return Expression>";
}


ExprType::Type ReturnExpr::Type()
{
	return ExprType::Return;
}


///////////////////////////////////////////
/// Line Expression
///////////////////////////////////////////

LineExpr::LineExpr(){}


LineExpr::~LineExpr(){}


std::string LineExpr::ToString()
{
	return "<Line Expression>";
}


ExprType::Type LineExpr::Type()
{
	return ExprType::Line;
}

	
///////////////////////////////////////////
/// Line Expression
///////////////////////////////////////////

ThenExpr::ThenExpr(){}


ThenExpr::~ThenExpr(){}


std::string ThenExpr::ToString()
{
	return "<Then Expression>";
}


ExprType::Type ThenExpr::Type()
{
	return ExprType::Then;
}


///////////////////////////////////////////
/// Eof Expression
///////////////////////////////////////////
	
EofExpr::EofExpr(){}


EofExpr::~EofExpr(){}
	

std::string	EofExpr::ToString()
{
	return "<Eof Expression>";
}


ExprType::Type EofExpr::Type()
{
	return ExprType::Eof;
}
	

}
