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
	// Contains a class used for loading source files and getting thier tokens
	class SourceFile
	{
		public:
			
			// loads a file
			// returns true or false on success or failure
			bool load(std::string filename);
		
			// get the next token in the file
			std::string nextToken();

			// returns the current token
			std::string curToken();
		
			// return the current line number of the last token
			unsigned int curLine();
		
			// return the start position of the last token
			unsigned int curPos();
		
		private:
			// returns true is the character is a token delimiter
			bool isTokenDelim(const char c);
		
		
			std::string		data;		//store the data of the source file
			unsigned int	charPos;	// the position of the character marker in the data
		
			//unsigned int	linePos;	// the current position in the line
			unsigned int	line;		// the current line in the file
			unsigned int	lineStart;	// store the charPos of where the line started
			std::string		token;		// the current token
	};
}
