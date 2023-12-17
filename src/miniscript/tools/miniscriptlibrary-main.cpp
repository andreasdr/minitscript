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
	if ((argc - 2) / 2 == 0 || argc % 2 == 1) {
		Console::println("Usage: miniscriptlibrary script_filename1 script_class_name1 [script_filenameN script_class_nameN] library_filename");
		exit(EXIT_FAILURE);
	}

	string pathToLibrary = string(argv[argc - 1]);
	string libraryCode;
	libraryCode+= string() + "auto scriptURI = pathName + \"/\" + fileName;" + "\n";
	string libraryIncludes;
	for (auto i = 0; i < ((argc - 2) / 2); i++) {
		auto scriptFileName = string(argv[1 + i * 2]);
		auto scriptClassName = string(argv[1 + i * 2 + 1]);
		libraryIncludes+= string() + "#include \"" + scriptClassName + ".h\"" + "\n";
		libraryCode+= string() + "\t" + "if (scriptURI == \"" + scriptFileName + "\") {" + "\n";
		libraryCode+= string() + "\t\t" + "script = make_unique<" + scriptClassName + ">();" + "\n";
		libraryCode+= string() + "\t" + "}" + "\n";
	}
	libraryCode+= string() + "\t" + "else {" + "\n";
	libraryCode+= string() + "\t\t" + "script = make_unique<MiniScript>();" + "\n";
	libraryCode+= string() + "\t" + "}";

	//
	try {
		Console::println("Generating library C++ file");
		auto mainSource = FileSystem::getContentAsString("./resources/miniscript/templates/transpilation", "NativeLibrary.cpp");
		mainSource = StringTools::replace(mainSource, "{$library-includes}", libraryIncludes);
		mainSource = StringTools::replace(mainSource, "{$library-code}", libraryCode);
		FileSystem::setContentFromString(FileSystem::getPathName(pathToLibrary), FileSystem::getFileName(pathToLibrary), mainSource);
	} catch (Exception& exception) {
		Console::println("An error occurred: " + string(exception.what()));
	}

	//
	exit(EXIT_SUCCESS);
}
