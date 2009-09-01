/*
 *  expressions.h
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

#ifndef _EXPRESSIONS_H
#define	_EXPRESSIONS_H

#include "lexer.h"


typedef enum
{
	Kitsune_ExprType_None,
	Kitsune_ExprType_Eof,
	Kitsune_ExprType_Def,
	Kitsune_ExprType_Function,
	Kitsune_ExprType_FunCall,
	Kitsune_ExprType_Literal,
	Kitsune_ExprType_Return,
}Kitsune_ExpressionType;


typedef struct
{
	Kitsune_ExpressionType type;
	void* data;
		
} Kitsune_Expression;

Kitsune_Expression* Kitsune_Expression_Make(Kitsune_ExpressionType type);


typedef struct
{
	char* identifer;
	Kitsune_Expression*	expr;
}Kitsune_DefExpr_Data;

Kitsune_Expression* Kitsune_DefExpr_Make(char* identifier, Kitsune_Expression*	expr);

typedef struct
{
	int		numArgs;
	char**	args;
	
	int						numBodyExprs;
	Kitsune_Expression**	bodyExprs;

}Kitsune_FunctionExpr_Data;



typedef enum
{
	Kitsune_Literal_Identifier,
	Kitsune_Literal_String,
	Kitsune_Literal_Int,
	Kitsune_Literal_Float,
}Kitsune_LiteralType;

typedef struct
{
	Kitsune_LiteralType type;
	
	union
	{
		char*	identifier;
		char*	stringData;
		int		intValue;
		float	floatValue;
	}data;

}Kitsune_LiteralExpr_Data;


typedef struct
{
	Kitsune_Expression*	object;
	char*				funName;
	
	int						numArgs;
	Kitsune_Expression**	args;

}Kitsune_FunCallExpr_Data;


typedef struct
{
	Kitsune_Expression*	expr;
}Kitsune_ReturnExpr_Data;


typedef struct
{
	float value;
}Kitsune_FloatExpr_Data;


typedef struct
{
	char* value;
}Kitsune_StringExpr_Data;

#endif

