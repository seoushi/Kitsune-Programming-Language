/*
 *  lexer.c
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

#include "lexer.hpp"
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace kitc;

////////////////////////////////////////////
// Token
////////////////////////////////////////////

Token::Token()
{
}


Token::Token(int type)
{
	this->type = type;
}


Token::~Token()
{
}


std::string Token::ToString()
{
	std::stringstream ss;
	
	switch (type)
	{
		case TokenType::Eof:
			ss << "EOF";
			break;
		case TokenType::Dot:
			ss << "DOT";
			break;
		case TokenType::Identifier:
			ss << "IDENTIFIER(" << identifier << ")";
			break;
		case TokenType::Int:
			ss << "INT(" << intValue << ")";
			break;
		case TokenType::Float:
			ss << "FLOAT(" << floatValue << ")";
			break;
		case TokenType::Equal:
			ss << "EQUALS";
			break;
		case TokenType::String:
			ss << "STRING(" << identifier << ")";
			break;
		case TokenType::OpenBrace:
			ss << "OPEN_BRACE";
			break;
		case TokenType::CloseBrace:
			ss << "CLOSE_BRACE";
			break;
		case TokenType::Comment:
			ss << "COMMENT";
			break;
		case TokenType::OpenParen:
			ss << "OPEN_PAREN";
			break;
		case TokenType::CloseParen:
			ss << "CLOSE_PAREN";
			break;
		case TokenType::If:
			ss << "IF";
			break;
		case TokenType::Elif:
			ss << "ELIF";
			break;
		case TokenType::Else:
			ss << "ELSE";
			break;
		case TokenType::Invalid:
			ss << "INVALID(" << identifier << ")";
			break;
		default:
			ss << "UNKNOWN(" << identifier << ")";
			break;
	}
	
	return ss.str();
}


////////////////////////////////////////////
// Lexer
////////////////////////////////////////////

Lexer::Lexer()
{
	lastChar = ' ';
	fileHandle = NULL;
}


Lexer::~Lexer()
{
	if(fileHandle)
	{
		fclose(fileHandle);
	}
}


bool Lexer::OpenFile(std::string filename)
{
	fileHandle = fopen(filename.c_str(), "r");
	
	return (fileHandle != 0);
}

	
std::string Lexer::PositionString()
{
	std::stringstream ss;
	
	ss << "[ " << (curLine + 1) << " : " << curColumn << " ]";
	
	return ss.str();
}


bool Lexer::GetNextChar()
{
	if(!fileHandle)
	{
		lastChar = EOF;
		return false;
	}
	
	curColumn++;
	lastChar = fgetc(fileHandle);
	
	return true;
}


bool Lexer::IsReservedChar(char character)
{
	switch(character)
	{
		case '{':
		case '}':
		case '(':
		case ')':
		case ' ':
		case '\r':
		case '\t':
		case '\n':
		case '#':
		case ',':
		case '\\':
		case '|':
		case '\"':
		case EOF:
			return true;
			break;
	}
	
	return false;
}


void Lexer::ParseNextToken()
{
	static char 	buffer[512];
	int				i;
	char			tmpChar;
	bool			hasPeriod;
	bool			isValid;
	
	
	while( (lastChar == ' ') || (lastChar == '\t') || (lastChar == '\r') || (lastChar == '\n') )
	{
		if(lastChar == '\n')
		{
			curColumn = 0;
			curLine++;
		}

		GetNextChar();
	}


	// check for comment
	if(lastChar == '#')
	{
		// perhaps the comment should be saved?
        while((lastChar != '\n') && (lastChar != EOF))
        {
			GetNextChar();
        }

		// we don't care about the comment, return the next thing
		ParseNextToken();
		return;
    }

	// dot
	if(lastChar == '.')
	{
		GetNextChar();
		curToken = TokenPtr(new Token(TokenType::Dot));
		return;
	}

	// open paren
	if(lastChar == '(')
	{
		GetNextChar();
		curToken = TokenPtr(new Token(TokenType::OpenParen));
		return;
	}
	

	// close paren
	if(lastChar == ')')
	{
		GetNextChar();
		curToken = TokenPtr(new Token(TokenType::CloseParen));
		return;
	}

	// check for string literal
	if(lastChar == '\"')
	{
		i = 0;
		buffer[0] = '\0';

		GetNextChar();

		while(true)
		{
			if(lastChar == '\"')
			{
				if(i == 0)
				{
					buffer[i] = '\0';
					break;
				}

				tmpChar = buffer[i - 1];

				if(tmpChar != '\\')
				{
					buffer[i] = '\0';
					break;
				}
			}

			buffer[i] = lastChar;
			i++;
			GetNextChar();
		}
		buffer[i] = '\0';
		i--;
		
		// eat last "
		GetNextChar();

		curToken = TokenPtr(new Token(TokenType::String));
		curToken->identifier = std::string(buffer);

        return;
	}
	
	// check for ints and floats
	if(isdigit(lastChar))
	{
		i = 0;
		hasPeriod = false;
		isValid = true;

		while( !IsReservedChar(lastChar) )
		{
			if(lastChar == '.')
			{
				if(hasPeriod)
				{
					isValid = false;
				}
				else
				{
					hasPeriod = true;
				}

				if( !isdigit(lastChar) && (lastChar != '.'))
				{
					isValid = false;
				}
			}

			if(!isValid)
			{
				curToken = TokenPtr(new Token(TokenType::Invalid));
				i++;
				buffer[i] = '\0';
				curToken->identifier = std::string(buffer);
				
				return;
			}

			buffer[i] = lastChar;
			buffer[i + 1] = '\0';
			i++;

			GetNextChar();
		}

		if(hasPeriod)
		{
			i++;
			buffer[i] = '\0';
			curToken = TokenPtr(new Token(TokenType::Float));
			curToken->floatValue = atof(buffer);
			
			return;
		}

		curToken = TokenPtr(new Token(TokenType::Int));
		i++;
		buffer[i] = '\0';
		curToken->intValue = atoi(buffer);

		return;
	}

	// open brace
	if(lastChar == '{')
	{
		GetNextChar();
		curToken = TokenPtr(new Token(TokenType::OpenBrace));
		return;
	}

	// close brace
	if(lastChar == '}')
	{
		GetNextChar();
		curToken = TokenPtr(new Token(TokenType::CloseBrace));
		return;
	}

    // check for the end of the file
	if(lastChar == EOF)
	{
		GetNextChar();
		curToken = TokenPtr(new Token(TokenType::Eof));
		return;
	}

	// Check for identifiers and reserved words
	{
		i = 0;
		buffer[0] = '\0';

		while( !IsReservedChar(lastChar) )
		{
			buffer[i] = lastChar;
			i++;
			GetNextChar();
		}
		buffer[i] = '\0';

		if(strcmp(buffer,"=") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::Equal));
			return;
		}
		if (strcmp(buffer, "def") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::Def));
			return;
		}
		if (strcmp(buffer, "if") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::If));
			return;
		}
		if (strcmp(buffer, "elif") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::Elif));
			return;
		}
		if (strcmp(buffer, "else") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::Else));
			return;
		}
		if (strcmp(buffer, "then") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::Then));
			return;
		}
		if (strcmp(buffer, "end") == 0)
		{
			curToken = TokenPtr(new Token(TokenType::End));
			return;
		}

		curToken = TokenPtr(new Token(TokenType::Identifier));
		curToken->identifier = std::string(buffer);

		return;
    }


	// seems we found something we don't know about just return it
	curToken = TokenPtr(new Token(TokenType::Invalid));
	curToken->identifier = " ";
	curToken->identifier[0] = lastChar;
	GetNextChar();

	return;
}


TokenPtr Lexer::CurToken()
{
	return curToken;
}
