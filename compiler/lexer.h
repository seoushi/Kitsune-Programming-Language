/*
 *  lexer.h
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


#ifndef _KITSUNE_LEXER_HPP
#define	_KITSUNE_LEXER_HPP

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    kitsune_tok_eof = -1,
    kitsune_tok_def = -2,
    kitsune_tok_identifier = -3,
    kitsune_tok_int = -4,
    kitsune_tok_float = -5,
	kitsune_tok_equal = -6,
	kitsune_tok_string = -7,
 	kitsune_tok_openBrace = -8,
    kitsune_tok_closeBrace = -9,
	kitsune_tok_pipe = -10,
	kitsune_tok_comma = -11,
	kitsune_tok_comment = -12,
	kitsune_tok_eol = -13,
	kitsune_tok_openParen = -8,
    kitsune_tok_closeParen = -9,
} Kitsune_TokenType;


typedef struct
{
    int type;
    
    union
    {
		char*	identifier;
        int		intValue;
        float	floatValue;
    }data;
} Kitsune_Token;


typedef struct
{
	FILE*			fileHandle;
    Kitsune_Token	curToken;
    int          	lastChar;
    int          	curLine;
    int          	curColumn;

}Kitsune_LexerData;



Kitsune_LexerData* Kitsune_Lex_make();

bool Kitsune_Lex_openFile(Kitsune_LexerData* lexer, char* filename);

Kitsune_Token* Kitsune_Lex_parseNextToken(Kitsune_LexerData* lexer);

char* Kitsune_Lex_positionStr(Kitsune_LexerData* lexer);

bool Kitsune_Lex_getNextCharacter(Kitsune_LexerData* lexer);

bool Kitsune_Lex_isReservedCharacter(char character);


#endif

