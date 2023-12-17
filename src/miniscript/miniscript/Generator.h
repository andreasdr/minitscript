#pragma once

#include <string>
#include <vector>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/fwd-miniscript.h>

using std::string;
using std::vector;

/**
 * MiniScript generator
 * @author Andreas Drewke
 */
class miniscript::miniscript::Generator {
public:

	/**
	 * Generate main CPP file
	 * @param scriptURI script URI
	 * @param className script class name
	 * @param mainURI main C++ file URI
	 * @param useLibrary use library
	 */
	static void generateMain(
		const string& scriptURI,
		const string& className,
		const string& mainURI,
		bool useLibrary
	);
};
