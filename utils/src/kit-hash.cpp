#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <kitpl/core.hpp>

// this is a slightly modified version of djb2
// orignal source from http://www.cse.yorku.ca/~oz/hash.html
kit::MsgId hash(char* str)
{
	kit::MsgId hash = 5381;
	int c;

	while(c = *str++)
	{
		hash = ((hash << 5) + hash) ^ c; // hash * 33 ^ c
	}

	return hash;
}


int main(int argc, char** argv)
{	
	if(argc >= 2)
	{
		if((strcmp("help", argv[1]) == 0) || (strcmp("HELP", argv[1]) == 0))
		{
			goto printHelp;
		}
		else
		{
			std::cout << std::string(argv[1]) << ": " << hash(argv[1]) << std::endl;
		}
	}
	else
	{
printHelp:
		printf("Usage is: hash <string-to-hash> \n");
	}
	
	return 0;
}
