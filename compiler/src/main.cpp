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
#include <stdio.h>
#include "SourceFile.hpp"

int main (int argc, const char* argv[])
{
	// check for invalid usage
	if(argc != 2)
	{
		printf("Usage is \"kitc file.kit\" \n");
		return 1;
	}
	
	
	printf("working directory is: %s\v", argv[0]);
	printf("running: %s\n", argv[1]);
	
	// load file
	kitc::SourceFile main;
	if(!main.load(argv[1]))
	{
		printf("Failed to load file: \"%s\"\n", argv[1]);
		return 1;
	}
	
	// print tokens
	std::string token = "start";
	while(!token.empty())
	{
		token = main.nextToken();
		printf("%i-%i: %s\n", main.curLine(), main.curPos(), token.c_str());
	}
   
	printf("Loading file done!\n");
    return 0;
}
