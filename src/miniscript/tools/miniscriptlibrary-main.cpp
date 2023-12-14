#include <cstdlib>
#include <string>
#include <vector>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/Version.h>
#include <miniscript/os/filesystem/FileSystem.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Exception.h>
#include <miniscript/utilities/StringTools.h>

using std::exit;
using std::string;
using std::to_string;
using std::vector;

using miniscript::miniscript::Version;
using miniscript::os::filesystem::FileSystem;
using miniscript::utilities::Console;
using miniscript::utilities::Exception;
using miniscript::utilities::StringTools;

int main(int argc, char** argv)
{
	Console::println(string("miniscriptlibrary ") + Version::getVersion());
	Console::println(Version::getCopyright());
	Console::println();

	//
	if (argc != 4) {
		Console::println("Usage: miniscriptlibrary script_filename1 script_class_name1 [script_filenameN script_class_nameN]");
		exit(EXIT_FAILURE);
	}

	//
	try {
		Console::println("Generating library C++ file");
		/*
		auto mainSource = FileSystem::getContentAsString("./resources/templates/transpilation", "script-main.cpp");
		mainSource = StringTools::replace(mainSource, "{$script}", pathToScript);
		mainSource = StringTools::replace(mainSource, "{$script-class}", scriptClassName);
		FileSystem::setContentFromString(FileSystem::getPathName(pathToMain), FileSystem::getFileName(pathToMain), mainSource);
		*/
	} catch (Exception& exception) {
		Console::println("An error occurred: " + string(exception.what()));
	}

	//
	exit(EXIT_SUCCESS);
}
