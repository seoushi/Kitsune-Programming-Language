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
CCC=
CXX=
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Linux-Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/lexer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/expressions.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/parser.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/generator.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/main.o

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
	${MAKE}  -f nbproject/Makefile-Linux-Debug.mk dist/Linux-Debug/GNU-Linux-x86/kitsunec

dist/Linux-Debug/GNU-Linux-x86/kitsunec: ${OBJECTFILES}
	${MKDIR} -p dist/Linux-Debug/GNU-Linux-x86
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/kitsunec ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/lexer.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/lexer.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/lexer.o ../../compiler/lexer.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/expressions.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/expressions.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/expressions.o ../../compiler/expressions.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/parser.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/parser.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/parser.o ../../compiler/parser.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/generator.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/generator.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/generator.o ../../compiler/generator.c

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/main.o: nbproject/Makefile-${CND_CONF}.mk ../../compiler/main.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/compiler/main.o ../../compiler/main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Linux-Debug
	${RM} dist/Linux-Debug/GNU-Linux-x86/kitsunec

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
