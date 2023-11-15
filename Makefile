#
BIN = bin
OBJ := obj
OBJ_DEBUG = obj-debug

# determine platform
OSSHORT := $(shell sh -c 'uname -o 2>/dev/null')
OS := $(shell sh -c 'uname -s 2>/dev/null')
MACHINE := $(shell sh -c 'uname -m 2>/dev/null')

#
NAME = miniscript
MAIN_LDFLAGS =

#
CPPVERSION = -std=c++2a
OFLAGS = -O3
EXTRAFLAGS = 
INCLUDES = -Isrc -Iext -I.

CPPFLAGS := $(INCLUDES)
CFLAGS := -g $(OFLAGS) $(EXTRAFLAGS) -pipe -MMD -MP -DNDEBUG
#CFLAGS := -g $(OFLAGS) $(EXTRAFLAGS) -pipe -MMD -MP
#CFLAGS := $(OFLAGS) $(EXTRAFLAGS) -pipe -MMD -MP -DNDEBUG
CFLAGS_DEBUG := -g -pipe -MMD -MP
CXXFLAGS := $(CFLAGS) $(CPPVERSION)
CXXFLAGS_DEBUG := $(CFLAGS_DEBUG) $(CPPVERSION)
CXXFLAGS_EXT_RP3D = $(CFLAGS_EXT_RP3D) $(CPPVERSION)

SRC = src
SHA256 = sha256

SRCS_DEBUG =

SRCS = \
	src/miniscript/utilities/Base64.cpp \
	src/miniscript/utilities/Console.cpp \
	src/miniscript/utilities/ExceptionBase.cpp \
	src/miniscript/utilities/FileSystem.cpp \
	src/miniscript/utilities/Float.cpp \
	src/miniscript/utilities/Hex.cpp \
	src/miniscript/utilities/Integer.cpp \
	src/miniscript/utilities/Properties.cpp \
	src/miniscript/utilities/SHA256.cpp \
	src/miniscript/utilities/StringTools.cpp \
	src/miniscript/utilities/StringTokenizer.cpp \
	src/miniscript/miniscript/ArrayMethods.cpp \
	src/miniscript/miniscript/BaseMethods.cpp \
	src/miniscript/miniscript/ConsoleMethods.cpp \
	src/miniscript/miniscript/JSONMethods.cpp \
	src/miniscript/miniscript/MapMethods.cpp \
	src/miniscript/miniscript/MathMethods.cpp \
	src/miniscript/miniscript/MiniScript.cpp \
	src/miniscript/miniscript/ScriptMethods.cpp \
	src/miniscript/miniscript/SetMethods.cpp \
	src/miniscript/miniscript/StringMethods.cpp \
	src/miniscript/miniscript/TimeMethods.cpp \
	src/miniscript/miniscript/Transpiler.cpp \
	src/miniscript/miniscript/XMLMethods.cpp \
	src/miniscript/miniscript/Version.cpp

EXT_SHA256_SRCS = \
	ext/sha256/sha256.cpp

MAIN_SRCS = \
	src/miniscript/tools/miniscriptdocumentation-main.cpp \
	src/miniscript/tools/miniscript-main.cpp \
	src/miniscript/tools/miniscriptmakefile-main.cpp \
	src/miniscript/tools/miniscriptnmakefile-main.cpp \
	src/miniscript/tools/miniscripttranspiler-main.cpp \
	src/miniscript/tools/miniscriptuntranspiler-main.cpp

MAINS = $(MAIN_SRCS:$(SRC)/%-main.cpp=$(BIN)/%)
OBJS = $(SRCS:$(SRC)/%.cpp=$(OBJ)/%.o)
EXT_SHA256_OBJS = $(EXT_SHA256_SRCS:ext/$(SHA256)/%.cpp=$(OBJ)/%.o)

define cpp-command
@mkdir -p $(dir $@);
@echo Compile $<; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
endef

define cpp-command-debug
@mkdir -p $(dir $@);
@echo Compile $<; $(CXX) $(CPPFLAGS) $(CXXFLAGS_DEBUG) -c -o $@ $<
endef

define c-command
@mkdir -p $(dir $@);
@echo Compile $<; $(CXX) -x c $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
endef

mains: $(MAINS)

$(OBJS):$(OBJ)/%.o: $(SRC)/%.cpp | print-opts
	$(cpp-command)

$(EXT_SHA256_OBJS):$(OBJ)/%.o: ext/$(SHA256)/%.cpp | print-opts
	$(cpp-command)

ifeq ($(OSSHORT), Msys)
$(MAINS):$(BIN)/%:$(SRC)/%-main.cpp $(OBJS) $(EXT_SHA256_OBJS)
	@mkdir -p $(dir $@);
	@scripts/windows-mingw-create-executable-rc.sh "$<" $@.rc
	@windres $@.rc -o coff -o $@.rc.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $@.rc.o $< $(OBJS) $(EXT_SHA256_OBJS) $(MAIN_LDFLAGS)
	@rm $@.rc
	@rm $@.rc.o
else
$(MAINS):$(BIN)/%:$(SRC)/%-main.cpp $(OBJS) $(EXT_SHA256_OBJS) $(EXT_SHA256_OBJS)
	@mkdir -p $(dir $@);
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(OBJS) $(EXT_SHA256_OBJS) $(MAIN_LDFLAGS)
endif

all: mains

clean:
	rm -rf obj obj-debug $(LIB_DIR) $(BIN)

print-opts:
	@echo Building with \"$(CXX) $(CPPFLAGS) $(CXXFLAGS)\"
	
.PHONY: all mains clean print-opts

-include $(OBJS:%.o=%.d)
