#############################################################################
# Makefile for building: modulo
# Generated by qmake (3.0) (Qt 5.3.0)
# Project:  modulo.pro
# Template: app
# Command: /usr/lib/x86_64-linux-gnu/qt5/bin/qmake -o Makefile modulo.pro
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES)
CXXFLAGS      = -m64 -pipe -std=c++11 -O2 -Wall -W -D_REENTRANT -fPIE $(DEFINES)
INCPATH       = 
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS) -lpthread 
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./../build

####### Files

SOURCES       = main.cpp \
		Solver/solver.cpp \
		Modulo/field.cpp \
		Modulo/game.cpp \
		Modulo/pos.cpp \
		Modulo/tile.cpp \
		IO/IO.cpp \
		Exceptions/modiszero.cpp \
		Exceptions/stringexception.cpp \
		Solver/deepsearch.cpp \
		Solver/Sat/mod2tosat.cpp \
		Solver/Sat/satbaseclass.cpp \
		Solver/Sat/mod3tosat.cpp \
		Solver/Sat/mod4tosat.cpp \
		Solver/Sat/tosathalf.cpp \
		Solver/Sat/tosat.cpp \
		Solver/Planning/pddladl.cpp \
		Solver/Planning/pddlbaseclass.cpp \
		Solver/Planning/pddlstrips.cpp \
		Complextest/statistic.cpp \
		Complextest/compositiontile.cpp \
		Solver/Planning/pddladlconst.cpp \
		Solver/Planning/pddlstripsconst.cpp \
		Solver/Planning/pddladlnumerics.cpp 
OBJECTS       = $(OBJECTS_DIR)/main.o \
		$(OBJECTS_DIR)/solver.o \
		$(OBJECTS_DIR)/field.o \
		$(OBJECTS_DIR)/game.o \
		$(OBJECTS_DIR)/pos.o \
		$(OBJECTS_DIR)/tile.o \
		$(OBJECTS_DIR)/IO.o \
		$(OBJECTS_DIR)/modiszero.o \
		$(OBJECTS_DIR)/stringexception.o \
		$(OBJECTS_DIR)/deepsearch.o \
		$(OBJECTS_DIR)/mod2tosat.o \
		$(OBJECTS_DIR)/satbaseclass.o \
		$(OBJECTS_DIR)/mod3tosat.o \
		$(OBJECTS_DIR)/mod4tosat.o \
		$(OBJECTS_DIR)/tosathalf.o \
		$(OBJECTS_DIR)/tosat.o \
		$(OBJECTS_DIR)/pddladl.o \
		$(OBJECTS_DIR)/pddlbaseclass.o \
		$(OBJECTS_DIR)/pddlstrips.o \
		$(OBJECTS_DIR)/statistic.o \
		$(OBJECTS_DIR)/compositiontile.o \
		$(OBJECTS_DIR)/pddladlconst.o \
		$(OBJECTS_DIR)/pddlstripsconst.o \
		$(OBJECTS_DIR)/pddladlnumerics.o
DIST    =   	modulo.pro main.cpp \
		Solver/solver.cpp \
		Modulo/field.cpp \
		Modulo/game.cpp \
		Modulo/pos.cpp \
		Modulo/tile.cpp \
		IO/IO.cpp \
		Exceptions/modiszero.cpp \
		Exceptions/stringexception.cpp \
		Solver/deepsearch.cpp \
		Solver/Sat/mod2tosat.cpp \
		Solver/Sat/satbaseclass.cpp \
		Solver/Sat/mod3tosat.cpp \
		Solver/Sat/mod4tosat.cpp \
		Solver/Sat/tosathalf.cpp \
		Solver/Sat/tosat.cpp \
		Solver/Planning/pddladl.cpp \
		Solver/Planning/pddlbaseclass.cpp \
		Solver/Planning/pddlstrips.cpp \
		Complextest/statistic.cpp \
		Complextest/compositiontile.cpp \
		Solver/Planning/pddladlconst.cpp \
		Solver/Planning/pddlstripsconst.cpp \
		Solver/Planning/pddladlnumerics.cpp
QMAKE_TARGET  = modulo
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = modulo


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(OBJECTS_DIR)/$(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

dist: 
	@test -d .tmp/modulo1.0.0 || mkdir -p .tmp/modulo1.0.0
	$(COPY_FILE) --parents $(DIST) .tmp/modulo1.0.0/ && $(COPY_FILE) --parents Solver/solver.h Modulo/field.h Modulo/game.h Modulo/pos.h Modulo/tile.h IO/IO.h Defines/compileoptions.h Exceptions/modiszero.h Exceptions/stringexception.h Solver/deepsearch.h Solver/Sat/mod2tosat.h Solver/Sat/satbaseclass.h Solver/Sat/mod3tosat.h Solver/Sat/mod4tosat.h Solver/Sat/tosathalf.h Solver/Sat/tosat.h Solver/Planning/pddladl.h Solver/Planning/pddlbaseclass.h Solver/Planning/pddlstrips.h Complextest/statistic.h Complextest/compositiontile.h Solver/Planning/pddladlconst.h Solver/Planning/pddlstripsconst.h Solver/Planning/pddladlnumerics.h .tmp/modulo1.0.0/ && $(COPY_FILE) --parents main.cpp Solver/solver.cpp Modulo/field.cpp Modulo/game.cpp Modulo/pos.cpp Modulo/tile.cpp IO/IO.cpp Exceptions/modiszero.cpp Exceptions/stringexception.cpp Solver/deepsearch.cpp Solver/Sat/mod2tosat.cpp Solver/Sat/satbaseclass.cpp Solver/Sat/mod3tosat.cpp Solver/Sat/mod4tosat.cpp Solver/Sat/tosathalf.cpp Solver/Sat/tosat.cpp Solver/Planning/pddladl.cpp Solver/Planning/pddlbaseclass.cpp Solver/Planning/pddlstrips.cpp Complextest/statistic.cpp Complextest/compositiontile.cpp Solver/Planning/pddladlconst.cpp Solver/Planning/pddlstripsconst.cpp Solver/Planning/pddladlnumerics.cpp .tmp/modulo1.0.0/ && (cd `dirname .tmp/modulo1.0.0` && $(TAR) modulo1.0.0.tar modulo1.0.0 && $(COMPRESS) modulo1.0.0.tar) && $(MOVE) `dirname .tmp/modulo1.0.0`/modulo1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/modulo1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


####### Sub-libraries

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

$(OBJECTS_DIR)/main.o: main.cpp Modulo/field.h \
		Modulo/game.h \
		Modulo/tile.h \
		Solver/Sat/mod2tosat.h \
		Modulo/pos.h \
		Solver/Sat/satbaseclass.h \
		Solver/solver.h \
		Solver/Sat/mod4tosat.h \
		Solver/Sat/mod3tosat.h \
		Solver/Sat/tosathalf.h \
		Solver/Sat/tosat.h \
		Solver/Sat/tosatdebug.h \
		IO/IO.h \
		Complextest/statistic.h \
		Solver/Planning/pddladl.h \
		Solver/Planning/pddlbaseclass.h \
		Solver/Planning/pddladlconst.h \
		Solver/Planning/pddlstrips.h \
		Solver/Planning/pddladlnumerics.h \
		Solver/Planning/pddlstripsconst.h
	$(CXX) -c $(CXXFLAGS) -o $(OBJECTS_DIR)/main.o main.cpp

$(OBJECTS_DIR)/solver.o: Solver/solver.cpp Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/solver.o Solver/solver.cpp

$(OBJECTS_DIR)/field.o: Modulo/field.cpp Modulo/field.h \
		Defines/compileoptions.h \
		Exceptions/stringexception.h \
		Exceptions/modiszero.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/field.o Modulo/field.cpp

$(OBJECTS_DIR)/game.o: Modulo/game.cpp Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Exceptions/stringexception.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/game.o Modulo/game.cpp

$(OBJECTS_DIR)/pos.o: Modulo/pos.cpp Modulo/pos.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pos.o Modulo/pos.cpp

$(OBJECTS_DIR)/tile.o: Modulo/tile.cpp Modulo/tile.h \
		Exceptions/stringexception.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/tile.o Modulo/tile.cpp

$(OBJECTS_DIR)/IO.o: IO/IO.cpp Exceptions/stringexception.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/IO.o IO/IO.cpp

$(OBJECTS_DIR)/modiszero.o: Exceptions/modiszero.cpp Exceptions/modiszero.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/modiszero.o Exceptions/modiszero.cpp

$(OBJECTS_DIR)/stringexception.o: Exceptions/stringexception.cpp Exceptions/stringexception.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/stringexception.o Exceptions/stringexception.cpp

$(OBJECTS_DIR)/deepsearch.o: Solver/deepsearch.cpp Solver/deepsearch.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/deepsearch.o Solver/deepsearch.cpp

$(OBJECTS_DIR)/mod2tosat.o: Solver/Sat/mod2tosat.cpp Solver/Sat/mod2tosat.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Solver/Sat/satbaseclass.h \
		Solver/solver.h \
		Defines/compileoptions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/mod2tosat.o Solver/Sat/mod2tosat.cpp

$(OBJECTS_DIR)/satbaseclass.o: Solver/Sat/satbaseclass.cpp Solver/Sat/satbaseclass.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Modulo/pos.h \
		Solver/solver.h \
		Defines/compileoptions.h \
		Exceptions/stringexception.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/satbaseclass.o Solver/Sat/satbaseclass.cpp

$(OBJECTS_DIR)/mod3tosat.o: Solver/Sat/mod3tosat.cpp Solver/Sat/mod3tosat.h \
		Solver/Sat/satbaseclass.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Modulo/pos.h \
		Solver/solver.h \
		Defines/compileoptions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/mod3tosat.o Solver/Sat/mod3tosat.cpp

$(OBJECTS_DIR)/mod4tosat.o: Solver/Sat/mod4tosat.cpp Solver/Sat/mod4tosat.h \
		Solver/Sat/satbaseclass.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Modulo/pos.h \
		Solver/solver.h \
		Defines/compileoptions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/mod4tosat.o Solver/Sat/mod4tosat.cpp

$(OBJECTS_DIR)/tosathalf.o: Solver/Sat/tosathalf.cpp Solver/Sat/tosathalf.h \
		Solver/Sat/satbaseclass.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Modulo/pos.h \
		Solver/solver.h \
		Defines/compileoptions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/tosathalf.o Solver/Sat/tosathalf.cpp

$(OBJECTS_DIR)/tosat.o: Solver/Sat/tosat.cpp Solver/Sat/tosat.h \
		Solver/Sat/satbaseclass.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Modulo/pos.h \
		Solver/solver.h \
		Exceptions/stringexception.h \
		Defines/compileoptions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/tosat.o Solver/Sat/tosat.cpp

$(OBJECTS_DIR)/pddladl.o: Solver/Planning/pddladl.cpp Solver/Planning/pddladl.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Solver/Planning/pddlbaseclass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pddladl.o Solver/Planning/pddladl.cpp

$(OBJECTS_DIR)/pddlbaseclass.o: Solver/Planning/pddlbaseclass.cpp Solver/Planning/pddlbaseclass.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pddlbaseclass.o Solver/Planning/pddlbaseclass.cpp

$(OBJECTS_DIR)/pddlstrips.o: Solver/Planning/pddlstrips.cpp Solver/Planning/pddlstrips.h \
		Solver/Planning/pddlbaseclass.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pddlstrips.o Solver/Planning/pddlstrips.cpp

$(OBJECTS_DIR)/statistic.o: Complextest/statistic.cpp Complextest/statistic.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		IO/IO.h \
		Complextest/compositiontile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/statistic.o Complextest/statistic.cpp

$(OBJECTS_DIR)/compositiontile.o: Complextest/compositiontile.cpp Complextest/compositiontile.h \
		Modulo/tile.h \
		Modulo/game.h \
		Modulo/field.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/compositiontile.o Complextest/compositiontile.cpp

$(OBJECTS_DIR)/pddladlconst.o: Solver/Planning/pddladlconst.cpp Solver/Planning/pddladlconst.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h \
		Solver/Planning/pddlbaseclass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pddladlconst.o Solver/Planning/pddladlconst.cpp

$(OBJECTS_DIR)/pddlstripsconst.o: Solver/Planning/pddlstripsconst.cpp Solver/Planning/pddlstripsconst.h \
		Solver/Planning/pddlbaseclass.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pddlstripsconst.o Solver/Planning/pddlstripsconst.cpp

$(OBJECTS_DIR)/pddladlnumerics.o: Solver/Planning/pddladlnumerics.cpp Solver/Planning/pddladlnumerics.h \
		Solver/Planning/pddlbaseclass.h \
		Solver/solver.h \
		Modulo/pos.h \
		Modulo/game.h \
		Modulo/field.h \
		Modulo/tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/pddladlnumerics.o Solver/Planning/pddladlnumerics.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

