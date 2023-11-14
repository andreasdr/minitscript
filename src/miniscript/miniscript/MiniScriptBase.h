#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-tdme.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript base functions
 * @author Andreas Drewke
 */
class miniscript::miniscript::MiniScriptBase {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
