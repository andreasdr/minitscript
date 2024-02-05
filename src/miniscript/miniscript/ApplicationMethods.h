#pragma once

#include <string>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/fwd-miniscript.h>

using miniscript::miniscript::MiniScript;

using std::string;

/**
 * MiniScript script application methods
 * @author Andreas Drewke
 */
class miniscript::miniscript::ApplicationMethods {
public:
	/**
	 * Register constants
	 * @param miniScript miniScript
	 */
	static void registerConstants(MiniScript* miniScript);

	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

	/**
	 * Execute
	 * @param command command
	 * @param exitCode pointer to exit code
	 * @param error pointer to error string
	 */
	static const string execute(const string& command, int* exitCode = nullptr, string* error = nullptr);

};
