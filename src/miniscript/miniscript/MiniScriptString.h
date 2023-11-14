#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-tdme.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript script string class
 * @author Andreas Drewke
 */
class miniscript::miniscript::MiniScriptString {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
