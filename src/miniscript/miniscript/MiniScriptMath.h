#pragma once

#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-tdme.h>

using std::span;

using miniscript::math::Math;
using miniscript::miniscript::MiniScript;

/**
 * MiniScript math functions
 * @author Andreas Drewke
 */
class miniscript::miniscript::MiniScriptMath {
public:
	/**
	 * Constructor
	 * @param miniScript miniScript
	 */
	MiniScriptMath(MiniScript* miniScript): miniScript(miniScript) {}

	/**
	 * Register script data type
	 * @param scriptDataType script data type
	 */
	void registerDataType(MiniScript::ScriptDataType* scriptDataType);

	/**
	 * Register methods
	 */
	void registerMethods();

	/**
	 * Multiply
	 * @param argumentValues argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void mul(const span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement);

	/**
	 * Division
	 * @param argumentValues argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void div(const span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement);

	/**
	 * Addition
	 * @param argumentValues argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void add(const span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement);

	/**
	 * Subtraction
	 * @param argumentValues argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void sub(const span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement);

private:
	MiniScript* miniScript;
	vector<MiniScript::ScriptDataType*> scriptDataTypes;

};