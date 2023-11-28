#include <cstdlib>
#include <memory>
#include <string>

// Windows MSC: required for OpenSSL to work when having OpenSSL embedded in a DLL which is used here
#if defined(_MSC_VER)
	#include <openssl/applink.c>
#endif

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/miniscript/Version.h>
#include <miniscript/os/filesystem/FileSystem.h>
#include <miniscript/os/network/Network.h>
#include <miniscript/utilities/Console.h>

using std::exit;
using std::make_unique;
using std::string;
using std::unique_ptr;

using miniscript::miniscript::MiniScript;
using miniscript::miniscript::Version;
using miniscript::os::filesystem::FileSystem;
using miniscript::os::network::Network;
using miniscript::utilities::Console;

static void printInformation() {
	Console::println(string("miniscript ") + Version::getVersion());
	Console::println(Version::getCopyright());
	Console::println();
	Console::println("Usage: miniscript [--version] [--verbose] path_to_script");
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	//
	string pathToScript;
	auto verbose = false;
	auto version = false;
	for (auto i = 1; i < argc; i++) {
		string argument = argv[i];
		if (argument == "--help") {
			printInformation();
			exit(EXIT_SUCCESS);
		} else
		if (argument == "--version") {
			version = true;
		} else
		if (argument == "--verbose") {
			verbose = true;
		} else {
			if (pathToScript.empty() == false) {
				Console::println("Path to script already given");
				exit(EXIT_FAILURE);
			} else {
				pathToScript = argument;
			}
		}
	}

	// version
	if (version == true) {
		Console::println(string("miniscript ") + Version::getVersion());
		Console::println(Version::getCopyright());
		exit(EXIT_FAILURE);
	}

	// run script
	// EngineMiniScript::registerDataTypes();
	unique_ptr<MiniScript> script;
	if (pathToScript.empty() == false) {
		//
		script = make_unique<MiniScript>();
		script->parseScript(
			FileSystem::getPathName(pathToScript),
			FileSystem::getFileName(pathToScript)
		);
		if (script != nullptr) {
			// verbose
			if (verbose == true) Console::println(script->getInformation());
			//
			if (script->isValid() == false) {
				Console::println("Script not valid. Exiting");
			} else {
				// TODO: we need a MiniScript startup routine
				Network::initialize();
				// Windows MSC: required for OpenSSL to work when having OpenSSL embedded in a DLL which is used here
				#if defined(_MSC_VER)
					OPENSSL_Applink();
				#endif
				//
				while (script->isRunning() == true) {
					script->execute();
				}
			}
		}
	} else
	if (version == false) {
		printInformation();
		exit(EXIT_SUCCESS);
	}

	//
	exit(script == nullptr || script->isValid() == false?EXIT_FAILURE:EXIT_SUCCESS);
}
