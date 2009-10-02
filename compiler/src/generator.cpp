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


#include "generator.hpp"
#include "lexer.hpp"


using namespace kitc;


Generator::Generator()
{
}


Generator::~Generator()
{
}


bool Generator::Generate(std::string filename, std::string copts)
{
	Lexer* lexer = new Lexer();
	std::vector<Expression*> exprs;
	
	if(!lexer->OpenFile(filename))
	{
		return false;
	}

	parser = new Parser(lexer);

	// parse the code
	while(true)
	{
		Expression* result = parser->Parse();

		if(parser->EncounteredError())
		{
			return false;
		}

		exprs.push_back(result);

		if(result->Type() == ExprType::Eof)
		{
			break;
		}
	}
	
	// generate C code
	std::string cFileName = filename;
	cFileName += ".cpp";
	cFile.open(cFileName.c_str(), std::ios::out);

	if(!cFile.is_open())
	{
		std::cerr << "Could not open file " << cFileName << std::endl;
		return false;
	}

	std::string cHeaderName = filename;
	cHeaderName += ".hpp";
	headerFile.open(cHeaderName.c_str(), std::ios::out);

	if(!headerFile.is_open())
	{
		std::cerr << "Could not open file " << cHeaderName << std::endl;
		return false;
	}

	GenHeader(cHeaderName);
	
	
	cFile << "Kitsune_Object* kitsune_entry_function(Kitsune_Object* self)" << std::endl;
	cFile << "{" << std::endl;

	for(unsigned int i = 0; i < exprs.size(); i++)
	{
		if(!GenExpr(exprs[i]))
		{
			break;
		}
	}

	cFile << "}" << std::endl << std::endl << std::endl;

	// write all loose function
	for(unsigned int i = 0; i < functions.size(); i++)
	{
		cFile << "Kitsune_Object* kit_function_" << (i + 1);

		if(!GenFun(functions[i]))
		{
			break;
		}
	}


	// write function prototypes to header
//?	GenHeader(headerFile, NULL);

	for(unsigned int i = 0; i < functions.size(); i++)
	{
		headerFile << "Kitsune_Object* kit_function_" << (i + 1) << "(Kitsune_Object* self";

		// write out args from function expression
		FuncExpr* func = (FuncExpr*)functions[i];

		for(unsigned int j = 0; j < func->args.size(); j++)
		{
			headerFile << ", Kitsune_Object* " << func->args[j];
		}

		headerFile << ");" << std::endl;
	}

	GenFooter();
	

	cFile.close();
	headerFile.close();
	

	std::string sysCommand = "gcc " + cFileName + " " + copts + " -lkitsune -lgc -lm";
	std::cout << sysCommand << std::endl;

	system(sysCommand.c_str());

	return true;
}



bool Generator::GenExpr(Expression* expr)
{
	switch(expr->Type())
	{
		case ExprType::Eof:
			return true;
			break;
		case ExprType::Def:
			return GenDef(expr);
		case ExprType::Assign:
			
			cFile << ((AssignExpr*)expr)->identifier << " = ";
			return GenExpr( ((AssignExpr*)expr)->expr );
		case ExprType::Line:
			{
				bool tmpResult = GenExpr(((LineExpr*)expr)->expr);
				cFile << std::endl;
				return tmpResult;
			}
			break;
		case ExprType::Function:

			functions.push_back(expr);

			cFile << "(Kitsune_Object*) &kit_function_" << functions.size();
			return true;

			break;
		case ExprType::FuncCall:
			return GenFunCall(expr);
			break;
		case ExprType::Literal:
			return GenLiteral(expr);
			break;
		case ExprType::Return:
			return GenReturn(expr);
			break;
		default:
			std::cerr << "ERROR: Unknown expression type " << expr->ToString() << "(" << expr->Type() << ")" << std::endl;
			return false;
			break;
	}
	
	return false;
}


bool Generator::GenDef(Expression* expr)
{
	cFile << "Kitsune_Object* ";	
	return GenExpr(((DefExpr*)expr)->expr);
}


bool Generator::GenFun(Expression* expr)
{
	FuncExpr* func = (FuncExpr*)expr;


	// write args
	cFile << "(Kitsune_Object* self";
	
	for(unsigned int i = 0; i < func->args.size(); i++)
	{
		cFile << ", Kitsune_Object* " << func->args[i];
	}
	cFile << " )" << std::endl << "{" << std::endl;


	// write function body
	for(unsigned int i = 0; i < func->bodyExprs.size(); i++)
	{
		
		
		if(!GenExpr(func->bodyExprs[i]))
		{
			return false;
		}
	}
	
	cFile << "}" << std::endl;
	
	return true;
}


bool Generator::GenFunCall(Expression* expr)
{
	FuncCallExpr* funcCall = (FuncCallExpr*)expr;


	cFile << "Kitsune_SendMessage(";

	if(!GenExpr(funcCall->sender))
	{
		return false;
	}

	cFile << ", \"" << funcCall->funcName << "\"";

	for(unsigned int i = 0; i < funcCall->args.size(); i++)
	{
		cFile << ", ";

		if(!GenExpr(funcCall->args[i]))
		{
			return false;
		}
	}

	cFile << ")";

	return true;
}


bool Generator::GenLiteral(Expression* expr)
{
	LitExpr* lit = (LitExpr*)expr;
	
	switch(lit->literalType)
	{
		case LiteralType::Identifier:
			cFile << lit->stringValue;
			break;
		case LiteralType::String:
			cFile << "Kitsune_MakeString(\"" << lit->stringValue << "\")";
			break;
		case LiteralType::Int:
			cFile << "Kitsune_MakeInteger(\"" << lit->intValue << ")";
			break;
		case LiteralType::Float:
			cFile << "Kitsune_MakeFloat(\"" << lit->floatValue << ")";
			break;
		default:
			return false;
			break;
	}
	
	return true;
}


bool Generator::GenReturn(Expression* expr)
{
	ReturnExpr* ret = (ReturnExpr*)expr;

	cFile << "return ";
	
	if(!GenExpr(ret->expr))
	{
		return false;
	}
	
	cFile << ";" << std::endl;
	
	return true;
}


void Generator::GenHeader(std::string headerName)
{
	// need to strip everything except the filename from the header name
	std::string headerFileName;

	// find last forward slash to strip from
	if(headerName.length() > 0)
	{
		for(unsigned int i = headerName.length() - 1; i >= 0; i--)
		{
			if(headerName[i] == '/')
			{
				headerFileName = std::string(&headerName.c_str()[i + 1]);
				break;
			}
		}
	}


	cFile << "/* this code is generated by kitsunec, it's not a good idea to make changes" << std::endl;
	cFile << " * in this file */" << std::endl;

	if(headerFileName.length() > 0)
	{
		cFile << "#include \"" << headerFileName << "\"" << std::endl << std::endl;
	}

	cFile << "#include <gc/gc.h>" << std::endl;
	cFile << "#include \"runtime/core.h\"" << std::endl;
	cFile << "#include \"runtime/array.h\"" << std::endl;
	cFile << "#include \"runtime/boolean.h\"" << std::endl;
	cFile << "#include \"runtime/character.h\"" << std::endl;
	cFile << "#include \"runtime/float.h\"" << std::endl;
	cFile << "#include \"runtime/integer.h\"" << std::endl;
	cFile << "#include \"runtime/object.h\"" << std::endl;
	cFile << "#include \"runtime/string.h\"" << std::endl << std::endl << std::endl;
}


void Generator::GenFooter()
{
	cFile << std::endl << std::endl;
	cFile << "int main(int argc, char** argv)" << std::endl;
	cFile << "{" << std::endl;

	cFile << "Kitsune_Object*\targuments;" << std::endl;
	cFile << "Kitsune_Object**\tarray;" << std::endl;
	cFile << "Kitsune_Object*\ttopLevel" << std::endl;
	cFile << "int\t\t\ti;" << std::endl;
	cFile << std::endl;

	cFile << "GC_INIT();" << std::endl;
	cFile << std::endl;
	
	cFile << "Kitsune_InitObject();" << std::endl;
	cFile << "Kitsune_InitArray();" << std::endl;
	cFile << "Kitsune_InitBoolean();" << std::endl;
	cFile << "Kitsune_InitCharacter();" << std::endl;
	cFile << "Kitsune_InitFloat();" << std::endl;
	cFile << "Kitsune_InitInteger();" << std::endl;
	cFile << "Kitsune_InitString();" << std::endl;
	cFile << std::endl;


	cFile << "array = (Kitsune_Object**)GC_MALLOC( sizeof(Kitsune_Object*) * argc);" << std::endl;
	cFile << std::endl;
	cFile << "for(i = 0; i < argc; i++)" << std::endl;
	cFile << "{" << std::endl;
	cFile << "\tarray[i] = Kitsune_MakeString(argv[i]);" << std::endl;
	cFile << "}" << std::endl;
	cFile << std::endl;


	cFile << "arguments = Kitsune_MakeArrayVec(argc, array);" << std::endl;
	cFile << "topLevel = Kitsune_SendMessage(Kitsune_InitObject(), \"clone\");" << std::endl;
	cFile << "Kitsune_SendMessage(topLevel, \"set-value\", \"arguments\", arguments);" << std::endl;
	cFile << "Kitsune_SendMessage(topLevel, \"set-method\", \"entry-point\", &kitsune_entry_function);" << std::endl;
	
	cFile << "Kitsune_SendMessage(topLevel, \"entry-point\");" << std::endl;
	cFile << "return 0;" << std::endl;
	cFile << "}" << std::endl;
	cFile << std::endl;
}