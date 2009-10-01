/*
 *  lexer.hpp
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


#ifndef KITC_LEXER_HPP
#define	KITC_LEXER_HPP

#include <string>

namespace kitc
{

namespace TokenType
{
	typedef enum
	{
		Eof = -1,
		Dot = -2,
		Identifier = -3,
		Int = -4,
		Float = -5,
		Equal = -6,
		String = -7,
		OpenBrace = -8,
		CloseBrace = -9,
		Comment = -10,
		OpenParen = -11,
		CloseParen = -12,
		Def = -13,
		Invalid = -14,
	}Type;
}


class Token
{
	public:
		Token();
		Token(int type);
		~Token();
	
		std::string ToString();
	
		int type;
	
		union
		{
			int		intValue;
			float	floatValue;
		};
	
		std::string identifier;
};


class Lexer
{
	public:

		Lexer();
		~Lexer();

		bool OpenFile(std::string filename);
	
		void ParseNextToken();
	
		std::string PositionString();
		
		bool GetNextChar();
	
		static bool IsReservedChar(char character);
	
		Token* CurToken();
	
	private:

		FILE*	fileHandle;
		Token*	curToken;
		int		lastChar;
		int		curLine;
		int		curColumn;

};


}


#endif

