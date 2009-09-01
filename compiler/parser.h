/*
 *  parser.h
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


#ifndef _PARSER_H
#define	_PARSER_H

#include "lexer.h"
#include "expressions.h"
#include <stdbool.h>


void Kitsune_ParseError(char* exprType, Kitsune_Token* token, Kitsune_LexerData* lexer, char* expected);
void Kitsune_PrintDebug(char* message);

typedef struct
{
    Kitsune_Expression* expr;
    bool succeeded;
}Kitsune_ResultTuple;

Kitsune_ResultTuple* Kitsune_ResultTuple_make(Kitsune_Expression* expr, bool succeeded);



Kitsune_ResultTuple* Kitsune_Parse_TopLevel(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_Def(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_Function(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_FuncCall(Kitsune_LexerData* lexer);
Kitsune_ResultTuple* Kitsune_Parse_Return(Kitsune_LexerData* lexer);

#endif

