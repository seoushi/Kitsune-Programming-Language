/*
 *  generator.c
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


#include "generator.h"
#include "lexer.h"

#include <gc/gc.h>


int						_numFunctions	= 0;
Kitsune_Expression**	_functions		= 0;


bool Kitsune_Gen_expr(FILE* outFile, Kitsune_Expression* expr);
bool Kitsune_Gen_def(FILE* outFile, Kitsune_Expression* expr);
bool Kitsune_Gen_fun(FILE* outFile, Kitsune_Expression* expr);
bool Kitsune_Gen_funCall(FILE* outFile, Kitsune_Expression* expr);
bool Kitsune_Gen_literal(FILE* outFile, Kitsune_Expression* expr);
bool Kitsune_Gen_return(FILE* outFile, Kitsune_Expression* expr);
void Kitsune_Gen_header(FILE* outFile, char* headerName);
void Kitsune_Gen_footer(FILE* outFile);

bool Kitsune_Generate(char* filename, char* copts)
{
	Kitsune_LexerData*			lexer = Kitsune_Lex_make();
	Kitsune_ResultTuple*		result;
	Kitsune_Expression**		exprs = 0;
	Kitsune_Expression*			curExpr = 0;
	int							numExprs = 0;
	FILE*						outFile;
	FILE*						headerFile;
	char*						cFileName;
	char*						cHeaderName;
	char*						sysCommand;
	int							i;
	int							j;
	char						tmpBuffer[32];
	Kitsune_FunctionExpr_Data*	funcData;
	
	/* parse the code */
	if(Kitsune_Lex_openFile(lexer, filename))
	{
		while(true)
		{
			result = Kitsune_Parse_topLevel(lexer);
			
			
			if(!result->succeeded)
			{
				return false;
			}
			
			numExprs++;
			if(numExprs == 1)
			{
				exprs = (Kitsune_Expression**)GC_MALLOC(sizeof(Kitsune_Expression*));
			}
			else
			{
				exprs = (Kitsune_Expression**)GC_REALLOC(exprs, sizeof(Kitsune_Expression*) * numExprs);
			}
			exprs[numExprs - 1] = result->expr;


			if((result->expr->type == Kitsune_ExprType_Eof))
			{
				break;
			}
		}
		
		/* generate C code */
		cFileName = (char*)GC_MALLOC(sizeof(char) * (strlen(filename) + 1 + strlen(".c")) );
		sprintf(cFileName, "%s.c", filename);
		outFile = fopen(cFileName, "w");

		
		if(!outFile)
		{
			fprintf(stderr, "Could not open file %s \n", cFileName);
			return false;
		}

		cHeaderName = (char*)GC_MALLOC(sizeof(char) * (strlen(filename) + 1 + strlen(".c")) );
		sprintf(cHeaderName, "%s.h", filename);
		headerFile = fopen(cHeaderName, "w");

		if(!headerFile)
		{
			fprintf(stderr, "Could not open file %s \n", cHeaderName);
			return false;
		}

		
		Kitsune_Gen_header(outFile, cHeaderName);

		fwrite("Kitsune_Object* kitsune_entry_function(Kitsune_Object* self)\n", 61, 1, outFile);
		fwrite("{\n", 2, 1, outFile);

                for(i = 0; i < numExprs; i++)
		{
			curExpr = exprs[i];
			
			if(!Kitsune_Gen_expr(outFile, curExpr))
			{
				break;
			}
		}

		fwrite("}\n\n\n", 4, 1, outFile);

		/* write all loose function */
                for(i = 0; i < _numFunctions; i++)
		{
			curExpr = _functions[i];

                        fwrite("Kitsune_Object* kit_function_", 29, 1, outFile);

			sprintf(tmpBuffer, "%i", i + 1);
			fwrite(tmpBuffer, strlen(tmpBuffer), 1, outFile);

			if(!Kitsune_Gen_fun(outFile, curExpr))
			{
				break;
			}
		}


		/* write function prototypes to header */
		Kitsune_Gen_header(headerFile, NULL);

		for(i = 0; i < _numFunctions; i++)
		{
			fwrite("Kitsune_Object* kit_function_", 29, 1, headerFile);
			
			sprintf(tmpBuffer, "%i", i + 1);
			fwrite(tmpBuffer, strlen(tmpBuffer), 1, headerFile);

			fwrite("(Kitsune_Object* self", 21, 1, headerFile);

			/* write out args from function expression */
			funcData = (Kitsune_FunctionExpr_Data*) _functions[i]->data;

			for(j = 0; j < funcData->numArgs; j++)
			{
				fwrite(", Kitsune_Object* ", 18, 1, headerFile);
				fwrite(funcData->args[j], strlen(funcData->args[j]), 1, headerFile);
			}

			fwrite(");\n", 3, 1, headerFile);
		}

		Kitsune_Gen_footer(outFile);
		
		fclose(outFile);
		fclose(headerFile);

		
		sysCommand = GC_MALLOC(sizeof(char) * (strlen(filename) + 55 + strlen(copts)));
		sprintf(sysCommand, "gcc %s %s -lkitsune -lgc -lm", cFileName, copts);

		printf("%s\n", sysCommand);

		system(sysCommand);

		return true;
	}
	
	return false;
}



bool Kitsune_Gen_expr(FILE* outFile, Kitsune_Expression* expr)
{
	char tmpBuffer[32];

	switch(expr->type)
	{
		case Kitsune_ExprType_Eof:
			return true;
			break;
		case Kitsune_ExprType_Def:
			return Kitsune_Gen_def(outFile, expr);
			break;
		case Kitsune_ExprType_Function:
			/* add to function list */
			/* return Kitsune_Gen_fun(outFile, expr); */

			_numFunctions++;

			if(_numFunctions == 1)
			{
				_functions = (Kitsune_Expression**)GC_MALLOC(sizeof(Kitsune_Expression*));
			}
			else
			{
				_functions = (Kitsune_Expression**)GC_REALLOC(_functions, sizeof(Kitsune_Expression*));
			}

			_functions[_numFunctions - 1] = expr;


			sprintf(tmpBuffer, "%i", _numFunctions);

			fwrite("(Kitsune_Object*) &kit_function_", 32, 1, outFile);
			fwrite(tmpBuffer, strlen(tmpBuffer), 1, outFile);
			return true;

			break;
		case Kitsune_ExprType_FunCall:
			return Kitsune_Gen_funCall(outFile, expr);
			break;
		case Kitsune_ExprType_Literal:
			return Kitsune_Gen_literal(outFile, expr);
			break;
		case Kitsune_ExprType_Return:
			return Kitsune_Gen_return(outFile, expr);
			break;
		default:
			fprintf(stderr, "ERROR: Unknown expression type %s(%i)\n", Kitsune_ExprType_toString(expr->type), expr->type);
			return false;
			break;
	}
	
	return false;
}


bool Kitsune_Gen_def(FILE* outFile, Kitsune_Expression* expr)
{
	int						i;
	Kitsune_DefExpr_Data*	data;
	bool					result;
	
	
	/* write header */
	fwrite("Kitsune_Object* ", 16, 1, outFile);
	
	/* write name */
	data = expr->data;
	for( i = 0; i < strlen(data->identifer); i++)
	{
		/* replace - with _HYPHEN_ */
		if(data->identifer[i] == '-')
		{
			fwrite("_HYPHEN_", 8, 1, outFile);
		}
		else
		{
			fwrite(&data->identifer[i], 1, 1, outFile);
		}
	}

	fwrite(" = ", 3, 1, outFile);


	result = Kitsune_Gen_expr(outFile, data->expr);

	fwrite(";\n", 2, 1, outFile);

	return result;
}


bool Kitsune_Gen_fun(FILE* outFile, Kitsune_Expression* expr)
{
	Kitsune_FunctionExpr_Data* 	data = expr->data;
	int							i;
	bool						succeeded;
	

	/* write args */
	fwrite("(Kitsune_Object* self", 21, 1, outFile);
	
	for(i = 0; i < data->numArgs; i++)
	{
		fwrite(", ", 2, 1, outFile);
		fwrite("Kitsune_Object* ", 16, 1, outFile);
		fwrite(data->args[i], strlen(data->args[i]), 1, outFile);
	}
	fwrite(" )\n{\n", 5, 1, outFile);


	/* write function body */
	for(i = 0; i < data->numBodyExprs; i++)
	{
		succeeded = Kitsune_Gen_expr(outFile, data->bodyExprs[i]);
		
		if(!succeeded)
		{
			/* return false; */
		}
	}
	
	fwrite("}\n", 2, 1, outFile);
	
	
	return true;
}


bool Kitsune_Gen_funCall(FILE* outFile, Kitsune_Expression* expr)
{
	Kitsune_FunCallExpr_Data*	data = expr->data;
	int							i;
	bool						succeeded;

	fwrite("Kitsune_SendMessage(", 20, 1, outFile);


	succeeded = Kitsune_Gen_expr(outFile, data->object);

	if(!succeeded)
	{
		return false;
	}

	fwrite(", \"", 3, 1, outFile);
	fwrite(data->funName, strlen(data->funName), 1, outFile);
	fwrite("\"", 1, 1, outFile);

	for(i = 0; i < data->numArgs; i++)
	{
		succeeded = Kitsune_Gen_expr(outFile, data->args[i]);
		if(!succeeded)
		{
			return false;
		}
	}

	fwrite(");\n", 3, 1, outFile);

	return true;
}


bool Kitsune_Gen_literal(FILE* outFile, Kitsune_Expression* expr)
{
	Kitsune_LiteralExpr_Data*	data = expr->data;
	char 						buffer[32];
	int							length;
	
	switch(data->type)
	{
		case Kitsune_Literal_Identifier:
			fwrite(data->data.identifier, strlen(data->data.identifier), 1, outFile);
			break;
		case Kitsune_Literal_String:
			fwrite("Kitsune_MakeString(\"", 20, 1, outFile);
			fwrite(data->data.stringData, strlen(data->data.stringData), 1, outFile);
			fwrite("\")", 2, 1, outFile);
			break;
		case Kitsune_Literal_Int:
			fwrite("Kitsune_MakeInteger(", 20, 1, outFile);
			
			length = sprintf(buffer,"%i", data->data.intValue);
			fwrite(buffer, length, 1, outFile);
			
			fwrite(")", 1, 1, outFile);
			break;
		case Kitsune_Literal_Float:
			fwrite("Kitsune_MakeInteger(", 20, 1, outFile);
			
			length = sprintf(buffer,"%f", data->data.floatValue);
			fwrite(buffer, length, 1, outFile);
			
			fwrite(")", 1, 1, outFile);
			break;
	}
	
	return true;
}


bool Kitsune_Gen_return(FILE* outFile, Kitsune_Expression* expr)
{
	Kitsune_ReturnExpr_Data* 	data = expr->data;
	bool						succeeded;
	
	
	fwrite("return ", 7, 1, outFile);
	
	succeeded = Kitsune_Gen_expr(outFile, data->expr);
	
	if(!succeeded)
	{
		/* return false; */
	}
	
	fwrite(";\n", 2, 1, outFile);
	
	return succeeded;
}


void Kitsune_Gen_header(FILE* outFile, char* headerName)
{
	/* need to strip everything except the filename from the header name */
	char*	headerFileName = headerName;
	int		idx;
	int		i;

	/* find last forward slash to strip from */
	if(headerName)
	{
		for(i = strlen(headerName) - 1; i >= 0; i--)
		{
			if(headerName[i] == '/')
			{
				headerFileName = &headerName[i + 1];
				break;
			}
		}
	}



	fwrite("/* this code is generated by kitsunec, it's not a good idea to make changes\n", 76, 1, outFile);
	fwrite("* in this file */\n\n", 19, 1, outFile);

	if(headerName)
	{
		fwrite("#include \"", 10, 1, outFile);
		fwrite(headerFileName, strlen(headerFileName), 1, outFile);
		fwrite("\"\n\n", 3, 1, outFile);
	}

	fwrite("#include <gc/gc.h>\n", 19, 1, outFile);
	fwrite("#include \"runtime/core.h\"\n", 26, 1, outFile);
	fwrite("#include \"runtime/array.h\"\n", 27, 1, outFile);
	fwrite("#include \"runtime/boolean.h\"\n", 29, 1, outFile);
	fwrite("#include \"runtime/character.h\"\n", 31, 1, outFile);
	fwrite("#include \"runtime/float.h\"\n", 27, 1, outFile);
	fwrite("#include \"runtime/integer.h\"\n", 29, 1, outFile);
	fwrite("#include \"runtime/object.h\"\n", 28, 1, outFile);
	fwrite("#include \"runtime/string.h\"\n\n\n", 30, 1, outFile);
}


void Kitsune_Gen_footer(FILE* outFile)
{
	fwrite("\n\nint main(int argc, char** argv)\n", 34, 1, outFile);
	fwrite("{\n", 2, 1, outFile);
	fwrite("Kitsune_Object*\targuments;\n", 27, 1, outFile);
	fwrite("Kitsune_Object**\tarray;\n", 24, 1, outFile);
        fwrite("Kitsune_Object*\ttopLevel;\n", 26, 1, outFile);
	fwrite("int\t\t\ti;\n\n", 10, 1, outFile);

	fwrite("GC_INIT();\n\n", 12, 1, outFile);
	
	fwrite("Kitsune_InitObject();\n", 22, 1, outFile);
	fwrite("Kitsune_InitArray();\n", 21, 1, outFile);
	fwrite("Kitsune_InitBoolean();\n", 23, 1, outFile);
	fwrite("Kitsune_InitCharacter();\n", 25, 1, outFile);
	fwrite("Kitsune_InitFloat();\n", 21, 1, outFile);
	fwrite("Kitsune_InitInteger();\n", 23, 1, outFile);
	fwrite("Kitsune_InitString();\n\n", 23, 1, outFile);

	fwrite("array = (Kitsune_Object**)GC_MALLOC( sizeof(Kitsune_Object*) * argc);\n\n", 71, 1, outFile);
	fwrite("for(i = 0; i < argc; i++)\n", 26, 1, outFile);
	fwrite("{\n", 2, 1, outFile);
	fwrite("\tarray[i] = Kitsune_MakeString(argv[i]);\n", 41, 1, outFile);
	fwrite("}\n\n", 3, 1, outFile);

	fwrite("arguments = Kitsune_MakeArrayVec(argc, array);\n", 47, 1, outFile);

	fwrite("topLevel = Kitsune_SendMessage(Kitsune_InitObject(), \"clone\");\n", 63, 1, outFile);
	fwrite("Kitsune_SendMessage(topLevel, \"set-value\", \"arguments\", arguments);\n", 68, 1, outFile);
	fwrite("Kitsune_SendMessage(topLevel, \"set-method\", \"entry-point\", &kitsune_entry_function);\n", 84, 1, outFile);
	
	fwrite("Kitsune_SendMessage(topLevel, \"entry-point\");\n", 46, 1, outFile);
	fwrite("return 0;\n", 10, 1, outFile);
	fwrite("}\n\n", 3, 1, outFile);


}