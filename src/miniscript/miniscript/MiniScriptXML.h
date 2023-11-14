#pragma once

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-tdme.h>

using miniscript::miniscript::MiniScript;

/**
 * MiniScript xml functions
 * @author Andreas Drewke
 */
class miniscript::miniscript::MiniScriptXML {
public:
	/**
	 * Register methods
	 * @param miniScript miniScript
	 */
	static void registerMethods(MiniScript* miniScript);

private:
	/**
	 * Escape given string in terms of XML
	 * @param str string
	 */
	static const string escape(const string& str);

};
