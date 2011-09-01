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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1943306719/main.o


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
LDLIBSOPTIONS=-Wl,-rpath ../PH_Input/../../../bin -L../PH_Input/../../../bin -lPH_Inputd -Wl,-rpath ../PH_Kernel/../../../bin -L../PH_Kernel/../../../bin -lPH_Kerneld

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/sample04

dist/Debug/GNU-Linux-x86/sample04: ../PH_Input/../../../bin/libPH_Inputd.so

dist/Debug/GNU-Linux-x86/sample04: ../PH_Kernel/../../../bin/libPH_Kerneld.so

dist/Debug/GNU-Linux-x86/sample04: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sample04 ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1943306719/main.o: ../../../src/sample_04/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1943306719
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -DPH_LINUX -DPH_GCC -I../../../include/input -I../../../include/kernel -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1943306719/main.o ../../../src/sample_04/main.cpp

# Subprojects
.build-subprojects:
	cd ../PH_Input && ${MAKE}  -f Makefile CONF=Debug
	cd ../PH_Kernel && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/sample04

# Subprojects
.clean-subprojects:
	cd ../PH_Input && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../PH_Kernel && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
