#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/main.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/parser.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/lexer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/generator.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/expressions.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-ansi -lgc

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-MacOSX/kitsunec

dist/Release/GNU-MacOSX/kitsunec: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-MacOSX
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kitsunec ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/main.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/main.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/main.o ../../compiler/main.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/parser.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/parser.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/parser.o ../../compiler/parser.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/lexer.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/lexer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/lexer.o ../../compiler/lexer.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/generator.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/generator.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/generator.o ../../compiler/generator.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/expressions.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/expressions.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/expressions.o ../../compiler/expressions.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-MacOSX/kitsunec

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
