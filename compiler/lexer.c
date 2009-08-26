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

#include "lexer.h"

#include <gc/gc.h>



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
	Kitsune_Token* 	curToken;
	static char 	buffer[1024];
	int				i;
	char			tmpChar;
	
	
	while( (lexer->lastChar == ' ') || (lexer->lastChar == '\t') || (lexer->lastChar == '\r') )
	{
		Kitsune_Lex_getNextCharacter(lexer);
	}

	/* check for line extension */
	if(lexer->lastChar == '\\')
	{
		while( (lexer->lastChar != EOF) && (lexer->lastChar != '\n'))
		{
			Kitsune_Lex_getNextCharacter(lexer);
		}
	}

	/* eat EOL */
	if(lexer->lastChar == '\n')
	{
		Kitsune_Lex_getNextCharacter(lexer);
	}

	/* check for end of line */
	if((lexer->lastChar == '\n') || (lexer->lastChar == ','))
	{
		if(lexer->lastChar == '\n')
		{
			lexer->curLine++;
			lexer->curColumn = 0;
		}
			
		curToken = Kitsune_Token_make(kitsune_tok_eol);
		Kitsune_Lex_getNextCharacter(lexer);

		return curToken;
    }


	/* check for comment */
	if(lexer->lastChar == '#' )
	{
		/* perhaps the comment should be saved? */
        while((lexer->lastChar != '\n') && (lexer->lastChar != EOF))
        {
			Kitsune_Lex_getNextCharacter(lexer);
        }

		return Kitsune_Token_make(kitsune_tok_comment);
    }


	/* open paren */
	if(lexer->lastChar == '(' )
	{
		Kitsune_Lex_getNextCharacter(lexer);
		return Kitsune_Token_make(kitsune_tok_openParen);
	}
	

	/* close paren */
	if(lexer->lastChar == ')')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		return Kitsune_Token_make(kitsune_tok_closeParen);
	}


	/* check for string literal */
	if(lexer->lastChar == '\"')
	{
		i = 0;

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
					buffer[i] = 0;
					break;
				}
			}

			i++;
			buffer[i] = lexer->lastChar;
			Kitsune_Lex_getNextCharacter(lexer);
		}

		/* eat last " */
		Kitsune_Lex_getNextCharacter(lexer);

		curToken = Kitsune_Token_make(kitsune_tok_string);
		curToken->data.identifier = (char*)GC_MALLOC(sizeof(char) * i);
		strncpy(curToken->data.identifier, buffer, i);

        return curToken;
	}

	
	/* check for ints and floats */

/* this needs to be fixed up 

	    if(isdigit(lastChar))
	    {
	        std::string identifier = "";

	        bool hasPeriod = false;
	        bool isValid = true;

	        while( !IsReservedCharacter() )
	        {
	            identifier += lastChar;

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
	            }

	            if( !isdigit(lastChar) && (lastChar != '.'))
	            {
	                isValid = false;
	            }

	            GetNextCharacter();
	        }

	        if(!isValid)
	        {
	            currentToken.tokenType = tok_invalid;
	            currentToken.identifierValue = identifier;

	            return currentToken;
	        }

	        if(hasPeriod)
	        {
	            currentToken.floatValue = atof(identifier.c_str());
	            currentToken.tokenType = tok_float;

	            return currentToken;
	        }


	        currentToken.intValue = atoi(identifier.c_str());
	        currentToken.tokenType = tok_int;

	        return currentToken;
	    }
*/

	/* open brace */
	if(lexer->lastChar == '{')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		return Kitsune_Token_make(kitsune_tok_openBrace);
	}

	/* close brace */
	if(lexer->lastChar == '}')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		return Kitsune_Token_make(kitsune_tok_closeBrace);
	}

	/* pipe */
	if(lexer->lastChar == '|')
	{
		Kitsune_Lex_getNextCharacter(lexer);
		return Kitsune_Token_make(kitsune_tok_pipe);
	}

    /* check for the end of the file */
	if(lexer->lastChar == EOF)
	{
		return Kitsune_Token_make(kitsune_tok_eof);
	}



	/* Check for identifiers and reserved words */
	{
		i = 0;
		buffer[0] = '\0';

		while( !Kitsune_Lex_isReservedCharacter(lexer->lastChar) )
		{
			i++;
			buffer[i] = lexer->lastChar;
			Kitsune_Lex_getNextCharacter(lexer);
		}		
		i++;
		buffer[i] = '\0';

		if(strcmp(buffer,"def") == 0)
		{
			return Kitsune_Token_make(kitsune_tok_def);
		}

		curToken = Kitsune_Token_make(kitsune_tok_identifier);
		curToken->data.identifier = GC_MALLOC(sizeof(char) * i);
		strncpy(curToken->data.identifier, buffer, i);

		return curToken;
    }



	/* seems we found something we don't know about just return it */
	curToken = Kitsune_Token_make(lexer->lastChar);
	Kitsune_Lex_getNextCharacter(lexer);

	return curToken;
}


char* Kitsune_Lex_positionStr(Kitsune_LexerData* lexer)
{
	char* buffer = GC_MALLOC(sizeof(char) * 32);
	
	sprintf(buffer, "Line: %d Col: %d", lexer->curLine, lexer->curColumn);
	
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
		case EOF:
			return true;
			break;
	}
	
	return false;
}
