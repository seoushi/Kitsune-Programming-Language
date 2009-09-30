cmake_minimum_required(VERSION 2.6)


PROJECT ( kitsuneLib )
PROJECT ( kitc )

SET ( kitsuneLib_SRCS	./runtime/src/array.cpp
			./runtime/src/boolean.cpp
			./runtime/src/float.cpp
			./runtime/src/integer.cpp
			./runtime/src/string.cpp)

SET ( kitc_SRCS ./compiler/src/main.cpp
				./compiler/src/expressions.cpp
				./compiler/src/lexer.cpp)
ADD_DEFINITIONS ( -Wall )

INCLUDE_DIRECTORIES ( ${CMAKE_BINARY_DIR} ./runtime/include/ ./compiler/include/ )

ADD_LIBRARY ( kitsune ${kitsuneLib_SRCS} )

ADD_EXECUTABLE ( kitc ${kitc_SRCS} )
target_link_libraries( kitc gc m)
