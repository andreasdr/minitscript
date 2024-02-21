#include <cstdlib>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/Context.h>
#include <minitscript/minitscript/Documentation.h>
#include <minitscript/minitscript/MinitScript.h>
#include <minitscript/minitscript/Version.h>
#include <minitscript/utilities/Console.h>
#include <minitscript/utilities/Properties.h>
#include <minitscript/utilities/StringTools.h>

using std::exit;
using std::make_pair;
using std::make_unique;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using minitscript::minitscript::Context;
using minitscript::minitscript::Documentation;
using minitscript::minitscript::MinitScript;
using minitscript::minitscript::Version;
using minitscript::utilities::Console;
using minitscript::utilities::Properties;
using minitscript::utilities::StringTools;

int main(int argc, char** argv)
{
	Console::printLine(string("minitscriptdocumentation ") + Version::getVersion());
	Console::printLine(Version::getCopyright());
	Console::printLine();

	//
	Properties descriptions;
	descriptions.load("resources/minitscript/documentation", "method-descriptions.properties");

	//
	MinitScript::initialize();

	//
	auto context = make_unique<Context>();
	auto minitScript = make_unique<MinitScript>();
	minitScript->setContext(context.get());
	minitScript->registerMethods();
	minitScript->registerVariables();

	//
	auto allClassMethods = Documentation::getAllClassMethods(minitScript.get());
	//
	auto baseMethodCategories = Documentation::getMethodsCategories(minitScript.get(), allClassMethods);
	// classes
	auto classesDocumentation = Documentation::generateClassesDocumentation("Classes", 6, minitScript.get(), descriptions, "minitscript.basemethod.", allClassMethods);
	// base methods
	auto methodsDocumentation = Documentation::generateMethodsDocumentation("Methods", 7, minitScript.get(), descriptions, "minitscript.basemethod.", allClassMethods);
	// variables
	auto variablesDocumentation = Documentation::generateVariablesDocumentation("Constants", 8, minitScript.get());
	// operators
	auto operatorsDocumentation = Documentation::generateOperatorsDocumentation("Operators", 9, minitScript.get());

	//
	Console::printLine("Classes");
	Console::printLine("---------");
	Console::printLine(classesDocumentation);

	//
	Console::printLine("Methods");
	Console::printLine("---------");
	Console::printLine(methodsDocumentation);

	//
	Console::printLine("Variables");
	Console::printLine("-----------");
	Console::printLine(variablesDocumentation);

	//
	Console::printLine("Operators");
	Console::printLine("-----------");
	Console::printLine(operatorsDocumentation);

	//
	Console::printLine();
	Console::printLine("# properties methodname=human readable string");
	//
	{
		Console::printLine("# base methods");
		//
		for (const auto& baseMethodCategory: baseMethodCategories) {
			Console::printLine("minitscript.basemethod.group." + (baseMethodCategory.empty() == true?"uncategorized":baseMethodCategory) + "=");
		}
		//
		auto scriptMethods = minitScript->getMethods();
		for (auto scriptMethod: scriptMethods) {
			Console::printLine("minitscript.basemethod." + scriptMethod->getMethodName() + "=");
		}
	}

	//
	Console::printLine();
	Console::printLine("Keywords: ");
	set<string> allMethods;
	{
		//
		auto scriptMethods = minitScript->getMethods();
		for (auto scriptMethod: scriptMethods) {
			allMethods.insert(scriptMethod->getMethodName());
		}
	}
	//
	for (const auto& method: allMethods) {
		Console::print(method + " ");
	}
	Console::printLine();

	//
	return EXIT_SUCCESS;
}
