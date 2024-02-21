#include <cstdlib>
#include <string>
#include <vector>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/Generator.h>
#include <minitscript/minitscript/Version.h>
#include <minitscript/utilities/Console.h>
#include <minitscript/utilities/StringTools.h>

using std::exit;
using std::string;
using std::to_string;
using std::vector;

using minitscript::minitscript::Generator;
using minitscript::minitscript::Version;
using minitscript::utilities::Console;
using minitscript::utilities::StringTools;

int main(int argc, char** argv)
{
	Console::printLine(string("minitscriptmain ") + Version::getVersion());
	Console::printLine(Version::getCopyright());
	Console::printLine();

	//
	if ((argc != 4 && argc != 5) || (argc == 5 && string(argv[4]) != "--use-library")) {
		Console::printLine("Usage: minitscriptmain script_filename script_class_name main_filename [--use-library]");
		return EXIT_FAILURE;
	}

	//
	auto scriptURI = string(argv[1]);
	auto className = string(argv[2]);
	auto mainURI = string(argv[3]);
	auto useLibrary = argc == 5?string(argv[4]) == "--use-library":false;

	//
	if (StringTools::endsWith(mainURI, "-main.cpp") == false) {
		Console::printLine("Error: main_filename must end with '-main.cpp'");
		return EXIT_FAILURE;
	}

	// generate
	Generator::generateMain(scriptURI, className, mainURI, useLibrary);

	//
	return EXIT_SUCCESS;
}
