#include <minitscript/minitscript/Library.h>

#include <memory>
#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/MinitScript.h>
#include <minitscript/utilities/Console.h>

using std::make_unique;
using std::string;

using _Context = minitscript::minitscript::Context;
using _Library = minitscript::minitscript::Library;
using _Console = minitscript::utilities::Console;

_Library::Library(_Context* context) {
	this->context = context;
}

_Library::~Library() {
}

MinitScript* _Library::loadScript(const string& pathName, const string& fileName, const string& basePathName) {
	_Console::printLine("Library::loadScript(): " + pathName + "/" + fileName + (basePathName.empty() == false?"@" + basePathName:""));
	auto script = make_unique<MinitScript>();
	script->setContext(context);
	script->setLibrary(this);
	script->parseScript((basePathName.empty() == false?basePathName + "/":"") + pathName, fileName);
	return script.release();
}
