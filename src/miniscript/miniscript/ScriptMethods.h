#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-miniscript.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript script script methods
 * @author Andreas Drewke
 */
class miniscript::miniscript::ScriptMethods {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
