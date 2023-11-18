#include <cstdlib>
#include <memory>

using std::make_unique;

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/FileSystem.h>

#include "{$script-class}.h"

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;
using miniscript::utilities::FileSystem;

int main(int argc, char *argv[]) {
	auto script = make_unique<{$script-class}>();
	string scriptFile = "{$script}";
	script->parseScript(FileSystem::getPathName(scriptFile), FileSystem::getFileName(scriptFile));
	if (script->isValid() == false) {
		Console::println("Script not valid. Exiting");
		return EXIT_FAILURE;
	}
	if (script->isNative() == false) {
		Console::println("Warning: Script not executed natively: " + scriptFile);
	}
	while (script->isRunning() == true) {
		script->execute();
	}
	//
	return EXIT_SUCCESS;
}
