#include <miniscript/miniscript/Generator.h>

#include <string>
#include <vector>

#include <miniscript/miniscript.h>
#include <miniscript/os/filesystem/FileSystem.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Exception.h>
#include <miniscript/utilities/StringTools.h>

using miniscript::miniscript::Generator;

using miniscript::os::filesystem::FileSystem;
using miniscript::utilities::Console;
using miniscript::utilities::Exception;
using miniscript::utilities::StringTools;

using std::string;
using std::vector;

void Generator::generateMain(
	const string& scriptURI,
	const string& className,
	const string& mainURI,
	bool useLibrary
) {
	string library;
	if (useLibrary == true) {
		library =
			string() +
			"auto library = make_unique<Library>(context.get());" + "\n" +
			"\t" + "script->setLibrary(library.get());";
	}
	//
	try {
		Console::println("Generating main C++ file: " + mainURI);

		auto mainSource = FileSystem::getContentAsString("./resources/miniscript/templates/transpilation", "script-main.cpp");
		mainSource = StringTools::replace(mainSource, "{$script}", scriptURI);
		mainSource = StringTools::replace(mainSource, "{$script-class}", className);
		mainSource = StringTools::replace(mainSource, "{$library}", library);
		FileSystem::setContentFromString(FileSystem::getPathName(mainURI), FileSystem::getFileName(mainURI), mainSource);
	} catch (Exception& exception) {
		Console::println("An error occurred: " + string(exception.what()));
	}
}

