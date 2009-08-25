#include <gc/gc.h>

#include "runtime/core.h"
#include "runtime/array.h"
#include "runtime/boolean.h"
#include "runtime/character.h"
#include "runtime/float.h"
#include "runtime/integer.h"
#include "runtime/object.h"
#include "runtime/string.h"

Kitsune_Object* entry_HYPHEN_point(Kitsune_Object* arguments)
{
	Kitsune_SendMessage( Kitsune_MakeString("Hello World"), "print-line" );
	
	return Kitsune_MakeInteger(0);
}


int main(int argc, char** argv)
{
	Kitsune_Object*		result;
	Kitsune_Object*		arguments;
	Kitsune_Object**	array;
	int 				i;

	GC_INIT();

	/* init runtime */
	Kitsune_InitObject();
	
	exit(0);
	
	Kitsune_InitArray();
	Kitsune_InitBoolean();
	Kitsune_InitCharacter();
	Kitsune_InitFloat();
	Kitsune_InitInteger();
	Kitsune_InitString();

	/* convert argv to kitsune array */
	array = (Kitsune_Object**)GC_MALLOC( sizeof(Kitsune_Object*) * argc);
	
	for(i = 0; i < argc; i++)
	{
		array[i] = Kitsune_MakeString(argv[i]);
	}
	
	arguments = Kitsune_MakeArrayVec(argc, array);

	result = entry_HYPHEN_point(arguments);
	
	return (int)Kitsune_SendMessage(result, "c-integer-value");
}