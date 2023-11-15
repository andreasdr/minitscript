#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-miniscript.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript script map class
 * @author Andreas Drewke
 */
class miniscript::miniscript::MapMethods {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

};
