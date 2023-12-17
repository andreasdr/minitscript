#include <miniscript/miniscript/Generator.h>

#include <string>
#include <utility>
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

using std::pair;
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

void Generator::generateLibrary(
	const vector<pair<string, string>>& scriptClassPairs,
	const string& libraryURI
) {
	string libraryCode;
	libraryCode+= string() + "auto scriptURI = pathName + \"/\" + fileName;" + "\n";
	string libraryIncludes;
	for (const auto& scriptClassPair: scriptClassPairs) {
		auto scriptURI = scriptClassPair.first;
		auto className = scriptClassPair.second;
		libraryIncludes+= string() + "#include \"" + className + ".h\"" + "\n";
		libraryCode+= string() + "\t" + "if (scriptURI == \"" + scriptURI + "\") {" + "\n";
		libraryCode+= string() + "\t\t" + "script = make_unique<" + className + ">();" + "\n";
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
		FileSystem::setContentFromString(FileSystem::getPathName(libraryURI), FileSystem::getFileName(libraryURI), mainSource);
	} catch (Exception& exception) {
		Console::println("An error occurred: " + string(exception.what()));
	}
}
