/*
 *  generator.cpp
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
#include "identifiers.hpp"
#include <stdlib.h>


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
	
	
	cFile << "kit::Object* kitsune_entry_function(kit::Object* self)" << std::endl;
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
		cFile << "kit::Object* kit_function_" << (i + 1);

		if(!GenFun(functions[i]))
		{
			break;
		}
	}


	// write function prototypes to header
//?	GenHeader(headerFile, NULL);

	for(unsigned int i = 0; i < functions.size(); i++)
	{
		headerFile << "kit::Object* kit_function_" << (i + 1) << "(kit::Object* self";

		// write out args from function expression
		FuncExpr* func = (FuncExpr*)functions[i];

		for(unsigned int j = 0; j < func->args.size(); j++)
		{
			headerFile << ", kit::Object* " << func->args[j];
		}

		headerFile << ");" << std::endl;
	}

	GenFooter();
	

	cFile.close();
	headerFile.close();
	

	std::string sysCommand = "g++ " + cFileName + " " + copts + " -lkitsune -lgc -lm";
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
			break;
		case ExprType::Assign:
			
			cFile << ((AssignExpr*)expr)->identifier << " = ";
			return GenExpr( ((AssignExpr*)expr)->expr );
		case ExprType::Line:
			{
				bool tmpResult = GenExpr(((LineExpr*)expr)->expr);
				cFile << ";" << std::endl;
				return tmpResult;
			}
			break;
		case ExprType::Function:

			functions.push_back(expr);

			cFile << "(kit::Object*) &kit_function_" << functions.size();
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
	cFile << "kit::Object* ";
	return GenExpr(((DefExpr*)expr)->expr);
}


bool Generator::GenFun(Expression* expr)
{
	FuncExpr* func = (FuncExpr*)expr;


	// write args
	cFile << "(kit::Object* self";
	
	for(unsigned int i = 0; i < func->args.size(); i++)
	{
		cFile << ", kit::Object* " << func->args[i];
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


	if(!GenExpr(funcCall->sender))
	{
		return false;
	}


	cFile << "->script(" << Identifiers::GetId(funcCall->funcName) << "/* " << funcCall->funcName << " */";

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
			cFile << "kit::String::make(\"" << lit->stringValue << "\")";
			break;
		case LiteralType::Int:
			cFile << "kit::Integer::make(" << lit->intValue << ")";
			break;
		case LiteralType::Float:
			cFile << "kit::Float::make(" << lit->floatValue << ")";
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
	cFile << "#include <kitpl/core.hpp>" << std::endl;
	cFile << "#include <kitpl/array.hpp>" << std::endl;
	cFile << "#include <kitpl/boolean.hpp>" << std::endl;
	cFile << "#include <kitpl/float.hpp>" << std::endl;
	cFile << "#include <kitpl/integer.hpp>" << std::endl;
	cFile << "#include <kitpl/string.hpp>" << std::endl << std::endl << std::endl;
	cFile << std::endl;
	
	
	cFile << "namespace kit" << std::endl;
	cFile << "{" << std::endl;
	cFile << "\ttypedef Object* (*EntryFuncPtr)(Object*);" << std::endl;
	cFile << "\tclass TopLevel : public Object" << std::endl;
	cFile << "\t{" << std::endl;
	cFile << "\t\tpublic:" << std::endl;
	cFile << "\t\t\tstatic Object* make(Object* arguments, EntryFuncPtr entry);" << std::endl;	
	cFile << "\t\t\tObject* script(MsgId message, ...);" << std::endl;
	cFile << std::endl;
	cFile << "\t\tprivate:" << std::endl;
	cFile << "\t\t\tObject* args;" << std::endl;
	cFile << "\tEntryFuncPtr entryFunc;" << std::endl;
	cFile << "\t};" << std::endl;
	cFile << std::endl;
	
	cFile << "\tObject* TopLevel::script(MsgId message, ...)" << std::endl;
	cFile << "\t{" << std::endl;
	cFile << "\t\tentryFunc(this);" << std::endl;
	cFile << "\t}" << std::endl;
	cFile << std::endl;
	
	cFile << "\tObject* TopLevel::make(Object* arguments, EntryFuncPtr entry)" << std::endl;
	cFile << "\t{" << std::endl;
	cFile << "\tTopLevel* top = new TopLevel();" << std::endl;
	cFile << "\t\ttop->args = arguments;" << std::endl;
	cFile << "\t\ttop->entryFunc = entry;" << std::endl;
	cFile << "\treturn top;" << std::endl;
	cFile << "\t}" << std::endl;
	cFile << std::endl;
	
	cFile << "}" << std::endl;
	cFile << std::endl;
}


void Generator::GenFooter()
{
	cFile << std::endl << std::endl;
	cFile << "int main(int argc, char** argv)" << std::endl;
	cFile << "{" << std::endl;

	cFile << "GC_INIT();" << std::endl;
	cFile << std::endl;
	
	cFile << "kit::Object* array = kit::Array::make();" << std::endl;
	cFile << std::endl;
	cFile << "for(unsigned i = 0; i < argc; i++)" << std::endl;
	cFile << "{" << std::endl;
	cFile << "\tarray->script(6382516965 /*add!*/, kit::String::make(argv[i]));" << std::endl;
	cFile << "}" << std::endl;
	cFile << std::endl;

	cFile << "kit::Object* topLevel = kit::TopLevel::make(array, &kitsune_entry_function);" << std::endl;
	cFile << "topLevel->script(210648428421 /*start*/);" << std::endl;
	
	cFile << "return 0;" << std::endl;
	
	cFile << "}" << std::endl;
	cFile << std::endl;
}
