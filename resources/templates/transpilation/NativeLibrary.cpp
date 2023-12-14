#include <miniscript/miniscript/Library.h>

#include <memory>
#include <string>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

{$library-includes}

using std::make_unique;
using std::string;

using miniscript::miniscript::Library;

using miniscript::miniscript::Context;
using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;

Library::Library(_Context* context) {
	this->context = context;
}

Library::~Library() {
}

MiniScript* Library::loadScript(const string& pathName, const string& fileName) {
	Console::println("NativeLibrary::loadScript(): " + pathName + "/" + fileName);
	{$library-code}
	auto script = make_unique<MiniScript>();
	script->setContext(context);
	script->setLibrary(this);
	script->parseScript(pathName, fileName);
	return script.release();
}
