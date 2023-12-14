#include <miniscript/miniscript/Library.h>

#include <string>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

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
	Console::println("Library::loadScript(): " + pathName + "/" + fileName);
	auto script = new MiniScript();
	script->setContext(context);
	script->setLibrary(this);
	script->parseScript(pathName, fileName);
	return script;
}
