#
BIN = bin
LIB_DIR = lib
OBJ = obj

# determine platform
OSSHORT = $(shell sh -c 'uname -o 2>/dev/null')
OS = $(shell sh -c 'uname -s 2>/dev/null')

#
NAME = miniscript
ifeq ($(OS), Darwin)
	LIB_EXT = .dylib
else ifeq ($(OSSHORT), Msys)
	LIB_EXT = .dll
else
	LIB_EXT = .so
endif
LIB = lib$(NAME)$(LIB_EXT)
MAIN_LDFLAGS = -L $(LIB_DIR) -l $(NAME)

#
CPPVERSION = -std=c++2a
OFLAGS = -O3
EXTRAFLAGS = 
INCLUDES = -Isrc -Iext -I.
STACKFLAGS =
PLATFORM = Unknown

# set platform specific flags
ifeq ($(OS), Darwin)
	# MacOSX
	LIBS_LDFLAGS = -lssl -lcrypto
	STACKFLAGS = -Wl,-stack_size -Wl,10485760
	PLATFORM = Darwin
else ifeq ($(OS), FreeBSD)
	# FreeBSD
	INCLUDES := $(INCLUDES) -I/usr/local/include
	LIBS_LDFLAGS = -L/usr/local/lib -lssl -lcrypto
	STACKFLAGS = -Wl,-stack_size -Wl,10485760
	PLATFORM = FreeBSD
else ifeq ($(OS), NetBSD)
	# NetBSD
	INCLUDES := $(INCLUDES) -I/usr/pkg/include
	LIBS_LDFLAGS = -L/usr/pkg/lib -lssl -lcrypto
	PLATFORM = NetBSD
else ifeq ($(OS), OpenBSD)
	# OpenBSD
	INCLUDES := $(INCLUDES) -I/usr/local/include
	LIBS_LDFLAGS = -L/usr/local/lib -lssl -lcrypto
	STACKFLAGS = -Wl,-stack_size -Wl,10485760
	PLATFORM = OpenBSD
else ifeq ($(OS), Haiku)
	# Haiku
	INCLUDES := $(INCLUDES) -I/boot/system/develop/headers
	LIBS_LDFLAGS = -lnetwork -lssl -lcrypto
	PLATFORM = Haiku
else ifeq ($(OS), Linux)
	# Linux
	LIBS_LDFLAGS = -L/usr/lib64 -lssl -lcrypto
	PLATFORM = Linux
else ifeq ($(OSSHORT), Msys)
	INCLUDES := $(INCLUDES) -I/mingw64/include
	LIBS_LDFLAGS = -L/mingw64/lib -lws2_32 -lssl -lcrypto
	STACKFLAGS = -Wl,--stack,10485760
	PLATFORM = Msys
endif

#
CPPFLAGS = $(INCLUDES)
CFLAGS = -g $(OFLAGS) $(EXTRAFLAGS) -pipe -MMD -MP -DNDEBUG -fPIC
CXXFLAGS = $(CFLAGS) $(CPPVERSION)

SRC = src

SRCS = \
	src/miniscript/miniscript/ApplicationMethods.cpp \
	src/miniscript/miniscript/ArrayMethods.cpp \
	src/miniscript/miniscript/BaseMethods.cpp \
	src/miniscript/miniscript/ByteArrayMethods.cpp \
	src/miniscript/miniscript/CryptographyMethods.cpp \
	src/miniscript/miniscript/ConsoleMethods.cpp \
	src/miniscript/miniscript/ContextMethods.cpp \
	src/miniscript/miniscript/Context.cpp \
	src/miniscript/miniscript/Documentation.cpp \
	src/miniscript/miniscript/FileSystemMethods.cpp \
	src/miniscript/miniscript/Generator.cpp \
	src/miniscript/miniscript/HTTPDownloadClientClass.cpp \
	src/miniscript/miniscript/JSONMethods.cpp \
	src/miniscript/miniscript/Library.cpp \
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
	src/miniscript/os/network/SecureTCPSocket.cpp \
	src/miniscript/os/network/TCPSocket.cpp \
	src/miniscript/utilities/Base64.cpp \
	src/miniscript/utilities/Console.cpp \
	src/miniscript/utilities/ErrorConsole.cpp \
	src/miniscript/utilities/ExceptionBase.cpp \
	src/miniscript/utilities/Float.cpp \
	src/miniscript/utilities/Hex.cpp \
	src/miniscript/utilities/Integer.cpp \
	src/miniscript/utilities/Properties.cpp \
	src/miniscript/utilities/SHA256.cpp \
	src/miniscript/utilities/StringTools.cpp \
	src/miniscript/utilities/StringTokenizer.cpp \
	src/miniscript/utilities/UTF8StringTools.cpp \
	src/miniscript/utilities/UTF8StringTokenizer.cpp

MAIN_SRCS = \
	src/miniscript/tools/miniscript-main.cpp \
	src/miniscript/tools/miniscriptdocumentation-main.cpp \
	src/miniscript/tools/miniscriptlibrary-main.cpp \
	src/miniscript/tools/miniscriptmain-main.cpp \
	src/miniscript/tools/miniscriptmakefile-main.cpp \
	src/miniscript/tools/miniscriptnmakefile-main.cpp \
	src/miniscript/tools/miniscripttranspiler-main.cpp \
	src/miniscript/tools/miniscriptuntranspiler-main.cpp

MAINS = $(MAIN_SRCS:$(SRC)/%-main.cpp=$(BIN)/%)
OBJS = $(SRCS:$(SRC)/%.cpp=$(OBJ)/%.o)

define cpp-command
@mkdir -p $(dir $@);
@echo Compile $<; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
endef

$(LIB_DIR)/$(LIB): $(OBJS)

$(OBJS):$(OBJ)/%.o: $(SRC)/%.cpp | print-opts
	$(cpp-command)

$(LIB_DIR)/$(LIB): $(OBJS)
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
	$(CXX) $(STACKFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $@ $@.rc.o $< $(MAIN_LDFLAGS) 
	@rm $@.rc
	@rm $@.rc.o
else
$(MAINS):$(BIN)/%:$(SRC)/%-main.cpp $(LIB_DIR)/$(LIB)
	@mkdir -p $(dir $@);
	$(CXX) $(STACKFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(MAIN_LDFLAGS)
endif

platform-check:
ifeq ($(PLATFORM), Unknown)
	@echo "Unknown platform. Exiting";\
	exit 1;
endif

mains: platform-check $(MAINS)

all: mains

clean:
	rm -rf obj obj-debug $(LIB_DIR) $(BIN)

print-opts:
	@echo Building with \"$(CXX) $(CPPFLAGS) $(CXXFLAGS)\"
	
.PHONY: all mains clean print-opts

-include $(OBJS:%.o=%.d)
