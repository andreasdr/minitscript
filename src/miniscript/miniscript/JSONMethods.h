#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-miniscript.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript JSON functions
 * @author Andreas Drewke
 */
class miniscript::miniscript::JSONMethods {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
