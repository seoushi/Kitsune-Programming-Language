cmake_minimum_required(VERSION 2.6)


PROJECT ( kitsuneLib )
PROJECT ( kitc )
PROJECT ( kit-hash )

SET ( kitsuneLib_SRCS	./runtime/src/array.cpp
			./runtime/src/boolean.cpp
			./runtime/src/float.cpp
			./runtime/src/integer.cpp
			./runtime/src/string.cpp
			./common/shared_ptr/libs/smart_ptr/src/sp_collector.cpp
                        ./common/shared_ptr/libs/smart_ptr/src/sp_debug_hooks.cpp)

SET ( kitc_SRCS ./compiler/src/main.cpp
		./compiler/src/expressions.cpp
		./compiler/src/lexer.cpp
		./compiler/src/parser.cpp
		./compiler/src/generator.cpp
		./compiler/src/identifiers.cpp
		./common/shared_ptr/libs/smart_ptr/src/sp_collector.cpp
		./common/shared_ptr/libs/smart_ptr/src/sp_debug_hooks.cpp)

SET ( kitHash_SRCS ./utils/src/kit-hash.cpp )

ADD_DEFINITIONS ( -Wall )

INCLUDE_DIRECTORIES ( ${CMAKE_BINARY_DIR} ./runtime/include/ ./compiler/include/ ./common/shared_ptr )

ADD_LIBRARY ( kitsune ${kitsuneLib_SRCS} )

ADD_EXECUTABLE ( kitc ${kitc_SRCS} )
target_link_libraries( kitc m )

ADD_EXECUTABLE ( kit-hash ${kitHash_SRCS} )
