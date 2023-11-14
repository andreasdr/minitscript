#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-tdme.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript script set class
 * @author Andreas Drewke
 */
class miniscript::miniscript::MiniScriptSet {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
