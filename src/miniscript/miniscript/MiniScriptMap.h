#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-tdme.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript script map class
 * @author Andreas Drewke
 */
class miniscript::miniscript::MiniScriptMap {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
