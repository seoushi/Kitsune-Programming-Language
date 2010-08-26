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

#include <iostream>
#include <fstream>
#include <ctype.h>


#include "SourceFile.hpp"

using namespace std;

namespace kitc
{


bool SourceFile::load(string filename)
{
	// reset the source file data
	charPos	= 0;
	lineStart = 1;
	line = 1;
	token = "";
	data = "";
	
	
	// open the file
	ifstream file(filename.c_str(), ios::in);
	
	if(!file.is_open())
	{
		return false;
	}
	
	//get the size of the file
	file.seekg(ios_base::end);
	int size = file.tellg();
	
	//resize the string to the size of the file
	data.reserve(size);
	
	//read the file into memory
	file.seekg(0);

	while(!file.eof())
	{
		data += file.get();
	}
	
	file.close();
	
	
	//replace eof marker with a space
	data[data.length() - 1] = ' ';
	
	
	return true;
}

	
string SourceFile::nextToken()
{
	if(charPos >= data.length())
	{
		return ""; // eof is a blank token
	}

	//find the start of the token
	unsigned int startTok = charPos;
	
	while ((startTok < data.length()) && isspace(data[startTok]))
	{
		if(data[startTok] == '\n')
		{
			lineStart = startTok;
			line++;
		}

		startTok++;
	}
	
	unsigned int endTok = startTok;
	
	// read a comment till the end of the line
	if(data[startTok] == '#')
	{
		while ((endTok < data.length()) && (data[endTok] != '\n'))
		{
			endTok++;
		}
		endTok--; // go back a char as the current one is an eol
	}
	else if(data[startTok] == '\"') // read till end of the string
	{
		while ((endTok < data.length()))
		{
			// if we find a quote and it's not escaped, we found the end of the string
			if((endTok != startTok) && (data[endTok] == '\"') && (data[endTok - 1] != '\\'))
			{
				break;
			}
			endTok++;
		}
	}
	else
	{
		//find the end of the token
		while ((endTok < data.length()) && !isTokenDelim(data[endTok]))
		{
			endTok++;
		}
		
		if(endTok != startTok) // this happens on a token delimiter
		{
			endTok--; // go back a char as the current one is a delimiter
		}
	}

	//update the position marker
	charPos = endTok + 1;
	
	//update the token
	token = data.substr(startTok, endTok - startTok + 1);
	
	
	
	return token;
}


unsigned int SourceFile::curLine()
{
	return line;
}


unsigned int SourceFile::curPos()
{
	if(lineStart == 1) // prevents getting a -1 for the first line
	{
		return 1;
	}

	return charPos - lineStart - token.length();
}


string SourceFile::curToken()
{
	return token;
}

	
	bool SourceFile::isTokenDelim(const char c)
	{
		return isspace(c) 
		|| (c == ',')
		|| (c == '.')
		|| (c == '{')
		|| (c == '}')
		|| (c == '=')
		|| (c == '(')
		|| (c == ')')
		|| (c == '#');
	}

}
