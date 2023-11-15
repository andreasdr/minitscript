#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-miniscript.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript script set class
 * @author Andreas Drewke
 */
class miniscript::miniscript::SetMethods {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
