/*
 * kitc - The Kitsune programming language compiler 
 * Copyright (C) 2010 Sean Chapel
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>


namespace kitc
{

namespace Token
{
	// enum for defining the types of tokens
	enum Type
	{
		Invalid,
		Identifier,
		Assignment,
		Number,
		EOS, // end of statement
		Commment,
		Comma,
	};
}

	
// interface class for describing a basic token	
class TokenInterface
{
	public:
		
		// a string representation of the token
		virtual std::string toString() = 0;
		
		Token::Type		getType();
		unsigned int	getLineNumber();
		unsigned int	getStartPosition();
		unsigned int	getEndPosition();
	
	protected:

		// way to set the positioning information
		void setPosition(unsigned int line, unsigned int start, unsigned int length);

		Token::Type		type;
		unsigned int	endPosition;
		unsigned int	startPosition;
		unsigned int	lineNumber;
};
	
	
// an invalid token, a token that doesn't match anything else
class InvalidToken : public TokenInterface
{
	public:
	
		InvalidToken(std::string text, unsigned int line, unsigned int column);
		virtual ~InvalidToken();

		virtual std::string toString();
	
	protected:
		std::string data;
};
	
}