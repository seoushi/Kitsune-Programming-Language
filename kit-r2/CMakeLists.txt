PROJECT ( libKitsune )

SET ( libKitsune_SRCS	./runtime/src/boolean.cpp
			./runtime/src/float.cpp
			./runtime/src/integer.cpp
			./runtime/src/string.cpp)

ADD_DEFINITIONS ( -Wall )

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} ./runtime/include/ )

ADD_LIBRARY ( libKitsune ${libKitsune_SRCS} )
