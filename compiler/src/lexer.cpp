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

#include <ctype.h>
#include <stdlib.h>


/*
Kitsune_LexerData* Kitsune_Lex_make()
{
	Kitsune_LexerData* result = (Kitsune_LexerData*) GC_MALLOC(sizeof(Kitsune_LexerData));
	result->lastChar = ' ';
	
	return result;
}


bool Kitsune_Lex_openFile(Kitsune_LexerData* lexer, char* filename)
{
	lexer->fileHandle = fopen(filename, "r");
	
	return (lexer->fileHandle != 0);
}


Kitsune_Token* Kitsune_Token_make(Kitsune_TokenType type)
{
	Kitsune_Token* tok = GC_MALLOC(sizeof(Kitsune_Token));
	tok->type = type;
	
	return tok;
}


Kitsune_Token*	Kitsune_Lex_parseNextToken(Kitsune_LexerData* lexer)
{
	static char 	buffer[1024];
	int				i;
	char			tmpChar;
	bool			hasPeriod;
	bool			isValid;
	
	
	while( (lexer->lastChar == ' ') || (lexer->lastChar == '\t') || (lexer->lastChar == '\r') || (lexer->lastChar == '\n') )
	{
		if(lexer->lastChar == '\n')
		{
			lexer->curColumn = 0;
			lexer->curLine++;
		}

		Kitsune_Lex_getNextCharacter(lexer);
	}


	/* check for comment */
/*	if(lexer->lastChar == '#' )
	{
		/* perhaps the comment should be saved? */
/*        while((lexer->lastChar != '\n') && (lexer->lastChar != EOF))
        {
			Kitsune_Lex_getNextCharacter(lexer);
        }

		/* we don't care about the comment, return the next thing */
/*		return Kitsune_Lex_parseNextToken(lexer);
    }


	/* open paren */
/*	if(lexer->lastChar == '(' )
	{
		Kitsune_Lex_getNextCharacter(lexer);
		
		lexer->curToken = Kitsune_Token_make(kitsune_tok_openParen);
		return lexer->curToken;
	}
	

	/* close paren */
/*	if(lexer->lastChar == ')')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		lexer->curToken = Kitsune_Token_make(kitsune_tok_closeParen);
		return lexer->curToken;
	}


	/* check for string literal */
/*	if(lexer->lastChar == '\"')
	{
		i = 0;
		buffer[0] = '\0';

		Kitsune_Lex_getNextCharacter(lexer);

		while(true)
		{
			if(lexer->lastChar == '\"')
			{
				if(i == 0)
				{
					buffer[i] = '\0';
					break;
				}

				tmpChar = buffer[i - 1];

				if(tmpChar != '\\')
				{
					i++;
					buffer[i] = '\0';
					break;
				}
			}

			buffer[i] = lexer->lastChar;
			i++;
			Kitsune_Lex_getNextCharacter(lexer);
		}
		buffer[i] = '\0';
		i--;
		
		/* eat last " */
/*		Kitsune_Lex_getNextCharacter(lexer);

		lexer->curToken = Kitsune_Token_make(kitsune_tok_string);
		lexer->curToken->data.identifier = (char*)GC_MALLOC(sizeof(char) * (i + 1));
		strncpy(lexer->curToken->data.identifier, buffer, i);

        return lexer->curToken;
	}

	
	/* check for ints and floats */
/*	if(isdigit(lexer->lastChar))
	{
		i = 0;
		hasPeriod = false;
		isValid = true;

		while( !Kitsune_Lex_isReservedCharacter(lexer->lastChar) )
		{
			if(lexer->lastChar == '.')
			{
				if(hasPeriod)
				{
					isValid = false;
				}
				else
				{
					hasPeriod = true;
				}

				if( !isdigit(lexer->lastChar) && (lexer->lastChar != '.'))
				{
					isValid = false;
				}
			}

			if(!isValid)
			{
				lexer->curToken = Kitsune_Token_make(kitsune_tok_invalid);
				i++;
				buffer[i] = '\0';
				lexer->curToken->data.identifier = GC_MALLOC( sizeof(char) * i);
				strncpy(lexer->curToken->data.identifier, buffer, i);
				
				return lexer->curToken;
			}

			buffer[i] = lexer->lastChar;
			buffer[i + 1] = '\0';
			i++;

			Kitsune_Lex_getNextCharacter(lexer);
		}

		if(hasPeriod)
		{
			i++;
			buffer[i] = '\0';
			lexer->curToken = Kitsune_Token_make(kitsune_tok_float);
			lexer->curToken->data.floatValue = atof(buffer);
			return lexer->curToken;
		}

		lexer->curToken = Kitsune_Token_make(kitsune_tok_int);
		i++;
		buffer[i] = '\0';
		lexer->curToken->data.intValue = atoi(buffer);

		return lexer->curToken;
	}

	/* open brace */
/*	if(lexer->lastChar == '{')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		lexer->curToken = Kitsune_Token_make(kitsune_tok_openBrace);
		return lexer->curToken;
	}

	/* close brace */
/*	if(lexer->lastChar == '}')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		lexer->curToken = Kitsune_Token_make(kitsune_tok_closeBrace);
		return lexer->curToken;
	}

    /* check for the end of the file */
/*	if(lexer->lastChar == EOF)
	{
		lexer->curToken = Kitsune_Token_make(kitsune_tok_eof);
		return lexer->curToken;
	}



	/* Check for identifiers and reserved words */
/*	{
		i = 0;
		buffer[0] = '\0';

		while( !Kitsune_Lex_isReservedCharacter(lexer->lastChar) )
		{
			buffer[i] = lexer->lastChar;
			i++;
			Kitsune_Lex_getNextCharacter(lexer);
		}
		buffer[i] = '\0';

		if(strcmp(buffer,"def") == 0)
		{
			lexer->curToken = Kitsune_Token_make(kitsune_tok_def);
			return lexer->curToken;
		}
		else if(strcmp(buffer,"=") == 0)
		{
			lexer->curToken = Kitsune_Token_make(kitsune_tok_equal);
			return lexer->curToken;
		}
		else if(strcmp(buffer,".") == 0)
		{
			lexer->curToken = Kitsune_Token_make(kitsune_tok_dot);
			return lexer->curToken;
		}

		lexer->curToken = Kitsune_Token_make(kitsune_tok_identifier);
		lexer->curToken->data.identifier = GC_MALLOC(sizeof(char) * i + 1);
		strncpy(lexer->curToken->data.identifier, buffer, i);

		return lexer->curToken;
    }


	/* seems we found something we don't know about just return it */
/*	lexer->curToken = Kitsune_Token_make(lexer->lastChar);
	Kitsune_Lex_getNextCharacter(lexer);

	return lexer->curToken;
}


char* Kitsune_Lex_positionStr(Kitsune_LexerData* lexer)
{
	char* buffer = GC_MALLOC(sizeof(char) * 32);
	
	sprintf(buffer, "Line: %d Col: %d", lexer->curLine + 1, lexer->curColumn);
	
	return buffer;
}


bool Kitsune_Lex_getNextCharacter(Kitsune_LexerData* lexer)
{
	if(!lexer->fileHandle)
	{
		lexer->lastChar = EOF;
		return false;
	}
	
	lexer->curColumn++;
	lexer->lastChar = fgetc(lexer->fileHandle);
	
	return true;
}


bool Kitsune_Lex_isReservedCharacter(char character)
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


char* Kitsune_Token_toString(Kitsune_Token* token)
{
	char* result;
	
	if(!token)
	{
		return "NULL_TOKEN";
	}
	
	switch(token->type)
	{
		case kitsune_tok_eof:
			return "EOF";
			break;
		case kitsune_tok_def:
			return "DEF";
			break;
		case kitsune_tok_identifier:
			result = GC_MALLOC(sizeof(char) * (strlen(token->data.identifier) + 14) );
			sprintf(result, "IDENTIFIER(%s)", token->data.identifier);
			return result;
			break;
		case kitsune_tok_int:
			result = GC_MALLOC(sizeof(char) * 24);
			sprintf(result, "INT(%d)", token->data.intValue);
			return result;
			break;
		case kitsune_tok_float:
			result = GC_MALLOC(sizeof(char) * 24);
			sprintf(result, "FLOAT(%f)", token->data.floatValue);
			return result;
		case kitsune_tok_equal:
			return "EQUALS";
			break;
		case kitsune_tok_string:
			result = GC_MALLOC(sizeof(char) * (strlen(token->data.identifier) + 14) );
			sprintf(result, "STRING(%s)", token->data.identifier);
			return result;
			break;
		case kitsune_tok_openBrace:
			return "OPEN_BRACE";
			break;
		case kitsune_tok_closeBrace:
			return "CLOSE_BRACE";
			break;
/*		case kitsune_tok_pipe:
			return "PIPE";
			break;*/
/*		case kitsune_tok_comma:
			return "COMMA";
			break;
		case kitsune_tok_comment:
			return "COMMENT";
			break;
		case kitsune_tok_dot:
			return "DOT";
			break;
		case kitsune_tok_openParen:
			return "OPEN_PAREN";
			break;
		case kitsune_tok_closeParen:
			return "CLOSE_PAREN";
			break;
		case kitsune_tok_invalid:
			result = GC_MALLOC(sizeof(char) * (strlen(token->data.identifier) + 14) );
			sprintf(result, "INVALID(%s)", token->data.identifier);
			return result;
			break;
		default:
			result = GC_MALLOC(sizeof(char) *  12);
			sprintf(result, "UNKNOWN(%i)", token->type);
			return result;
			break;
	}
}
*/
