#pragma once

#include <span>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/MinitScript.h>

using std::span;

using minitscript::minitscript::MinitScript;

/**
 * MinitScript math methods
 * @author Andreas Drewke
 */
class minitscript::minitscript::MathMethods {
public:
	/**
	 * Constructor
	 * @param minitScript minitScript
	 */
	MathMethods(MinitScript* minitScript): minitScript(minitScript) {}

	/**
	 * Register data type
	 * @param dataType data type
	 */
	void registerDataType(MinitScript::DataType* dataType);

	/**
	 * Register constants
	 */
	void registerConstants();

	/**
	 * Register methods
	 */
	void registerMethods();

	/**
	 * Multiply
	 * @param arguments argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void mul(const span<MinitScript::Variable>& arguments, MinitScript::Variable& returnValue, const MinitScript::Statement& statement);

	/**
	 * Division
	 * @param arguments argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void div(const span<MinitScript::Variable>& arguments, MinitScript::Variable& returnValue, const MinitScript::Statement& statement);

	/**
	 * Addition
	 * @param arguments argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void add(const span<MinitScript::Variable>& arguments, MinitScript::Variable& returnValue, const MinitScript::Statement& statement);

	/**
	 * Subtraction
	 * @param arguments argument values
	 * @param returnValue return value
	 * @param statement statement
	 */
	void sub(const span<MinitScript::Variable>& arguments, MinitScript::Variable& returnValue, const MinitScript::Statement& statement);

private:
	MinitScript* minitScript;
	vector<MinitScript::DataType*> dataTypes;

};
