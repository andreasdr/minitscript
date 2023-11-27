#
BIN = bin
LIB_DIR = lib
OBJ := obj

# determine platform
OSSHORT := $(shell sh -c 'uname -o 2>/dev/null')
OS := $(shell sh -c 'uname -s 2>/dev/null')

#
NAME = miniscript
ifeq ($(OS), Darwin)
	LIB_EXT = .dylib
else ifeq ($(OSSHORT), Msys)
	LIB_EXT = .dll
else
	LIB_EXT = .so
endif
LIB := lib$(NAME)$(LIB_EXT)
MAIN_LDFLAGS = -L $(LIB_DIR) -l $(NAME) -L/usr/lib64 -lssl -lcrypto

#
CPPVERSION = -std=c++2a
OFLAGS = -O3
EXTRAFLAGS = 
INCLUDES = -Isrc -Iext -I.

#
CXX := $(CXX) -fPIC

# set platform specific flags
ifeq ($(OS), Haiku)
	# Haiku
	INCLUDES := $(INCLUDES) -I/boot/system/develop/headers
	LIBS_LDFLAGS := -lnetwork
else ifeq ($(OSSHORT), Msys)
	# Windows
	LIBS_LDFLAGS := -L/mingw64/lib -lws2_32
endif

#
LIBS_LDFLAGS := $(LIBS_LDFLAGS)

#
CPPFLAGS := $(INCLUDES)
CFLAGS := -g $(OFLAGS) $(EXTRAFLAGS) -pipe -MMD -MP -DNDEBUG
CXXFLAGS := $(CFLAGS) $(CPPVERSION)

SRC = src
SHA256 = sha256

SRCS = \
	src/miniscript/miniscript/ApplicationMethods.cpp \
	src/miniscript/miniscript/ArrayMethods.cpp \
	src/miniscript/miniscript/BaseMethods.cpp \
	src/miniscript/miniscript/CryptographyMethods.cpp \
	src/miniscript/miniscript/ConsoleMethods.cpp \
	src/miniscript/miniscript/FileSystemMethods.cpp \
	src/miniscript/miniscript/JSONMethods.cpp \
	src/miniscript/miniscript/MapMethods.cpp \
	src/miniscript/miniscript/MathMethods.cpp \
	src/miniscript/miniscript/MiniScript.cpp \
	src/miniscript/miniscript/NetworkMethods.cpp \
	src/miniscript/miniscript/ScriptMethods.cpp \
	src/miniscript/miniscript/SetMethods.cpp \
	src/miniscript/miniscript/StringMethods.cpp \
	src/miniscript/miniscript/TimeMethods.cpp \
	src/miniscript/miniscript/Transpiler.cpp \
	src/miniscript/miniscript/XMLMethods.cpp \
	src/miniscript/miniscript/Version.cpp \
	src/miniscript/network/httpclient/HTTPClient.cpp \
	src/miniscript/network/httpclient/HTTPClientException.cpp \
	src/miniscript/network/httpclient/HTTPDownloadClient.cpp \
	src/miniscript/os/filesystem/FileSystem.cpp \
	src/miniscript/os/network/Network.cpp \
	src/miniscript/os/network/NetworkException.cpp \
	src/miniscript/os/network/NetworkIOException.cpp \
	src/miniscript/os/network/NetworkSocket.cpp \
	src/miniscript/os/network/NetworkSocketClosedException.cpp \
	src/miniscript/os/network/NetworkSocketException.cpp \
	src/miniscript/os/network/TCPSocket.cpp \
	src/miniscript/utilities/Base64.cpp \
	src/miniscript/utilities/Console.cpp \
	src/miniscript/utilities/ExceptionBase.cpp \
	src/miniscript/utilities/Float.cpp \
	src/miniscript/utilities/Hex.cpp \
	src/miniscript/utilities/Integer.cpp \
	src/miniscript/utilities/Properties.cpp \
	src/miniscript/utilities/SHA256.cpp \
	src/miniscript/utilities/StringTools.cpp \
	src/miniscript/utilities/StringTokenizer.cpp

# workaround: ext-sha256.cpp was sha256.cpp, but miniscript also comes with SHA256.cpp,
#	which collides on Windows as we dont have subfolders for ext objects
EXT_SHA256_SRCS = \
	ext/sha256/ext-sha256.cpp

MAIN_SRCS = \
	src/miniscript/tools/miniscript-main.cpp \
	src/miniscript/tools/miniscriptdocumentation-main.cpp \
	src/miniscript/tools/miniscriptmain-main.cpp \
	src/miniscript/tools/miniscriptmakefile-main.cpp \
	src/miniscript/tools/miniscriptnmakefile-main.cpp \
	src/miniscript/tools/miniscripttranspiler-main.cpp \
	src/miniscript/tools/miniscriptuntranspiler-main.cpp \
	src/miniscript/tools/ssltest-main.cpp

MAINS = $(MAIN_SRCS:$(SRC)/%-main.cpp=$(BIN)/%)
OBJS = $(SRCS:$(SRC)/%.cpp=$(OBJ)/%.o)
EXT_SHA256_OBJS = $(EXT_SHA256_SRCS:ext/$(SHA256)/%.cpp=$(OBJ)/%.o)

define cpp-command
@mkdir -p $(dir $@);
@echo Compile $<; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
endef

$(LIB_DIR)/$(LIB): $(OBJS)

$(OBJS):$(OBJ)/%.o: $(SRC)/%.cpp | print-opts
	$(cpp-command)

$(EXT_SHA256_OBJS):$(OBJ)/%.o: ext/$(SHA256)/%.cpp | print-opts
	$(cpp-command)

$(LIB_DIR)/$(LIB): $(OBJS) $(EXT_SHA256_OBJS)
	@echo Creating shared library $@
	@mkdir -p $(dir $@)
	@rm -f $@
ifeq ($(OSSHORT), Msys)
	@scripts/windows-mingw-create-library-rc.sh $@ $@.rc
	@windres $@.rc -o coff -o $@.rc.o
	$(CXX) -shared $(patsubst %$(LIB_EXT),,$^) -o $@ $@.rc.o $(LIBS_LDFLAGS) -Wl,--out-implib,$(LIB_DIR)/$(LIB).a
	@rm $@.rc
	@rm $@.rc.o
else
	$(CXX) -shared $(patsubst %$(LIB_EXT),,$^) -o $@ $(LIBS_LDFLAGS)
endif
	@echo Done $@

ifeq ($(OSSHORT), Msys)
$(MAINS):$(BIN)/%:$(SRC)/%-main.cpp $(LIB_DIR)/$(LIB)
	@mkdir -p $(dir $@);
	@scripts/windows-mingw-create-executable-rc.sh "$<" $@.rc
	@windres $@.rc -o coff -o $@.rc.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $@.rc.o $< $(MAIN_LDFLAGS)
	@rm $@.rc
	@rm $@.rc.o
else
$(MAINS):$(BIN)/%:$(SRC)/%-main.cpp $(LIB_DIR)/$(LIB)
	@mkdir -p $(dir $@);
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(MAIN_LDFLAGS)
endif

mains: $(MAINS)

all: mains

clean:
	rm -rf obj obj-debug $(LIB_DIR) $(BIN)

print-opts:
	@echo Building with \"$(CXX) $(CPPFLAGS) $(CXXFLAGS)\"
	
.PHONY: all mains clean print-opts

-include $(OBJS:%.o=%.d)
