#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/StringTools.h>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::StringTools;

/**
 * MiniScript transpiler
 * @author Andreas Drewke
 */
class miniscript::miniscript::Transpiler {
public:
	/**
	 * Transpile MiniScript script instance
	 * @param miniScript MiniScript script instance
	 * @param transpilationFileName transpilation file name
	 * @param miniScriptExtensionFileNames MiniScript extension file names
	 */
	static void transpile(MiniScript* miniScript, const string& transpilationFileName, const vector<string>& miniScriptExtensionFileNames);

	/**
	 * Untranspile
	 * @param scriptFileName script file name
	 * @param transpilationFileName transpilation file name
	 */
	static void untranspile(const string& scriptFileName, const string& transpilationFileName);

private:
	/**
	 * Get script type name
	 * @param type script type
	 * @return script type enum identifier
	 */
	inline static const string getScriptTypeReadableName(MiniScript::Script::Type type) {
		switch(type) {
			case MiniScript::Script::TYPE_NONE: return "none";
			case MiniScript::Script::TYPE_FUNCTION: return "function";
			case MiniScript::Script::TYPE_STACKLET: return "stacklet";
			case MiniScript::Script::TYPE_ON: return "on";
			case MiniScript::Script::TYPE_ONENABLED: return "on-enabled";
		};
		//
		return string();
	}

	/**
	 * Get script type enum identifier
	 * @param type script type
	 * @return script type enum identifier
	 */
	inline static const string getScriptTypeEnumIdentifier(MiniScript::Script::Type type) {
		switch(type) {
			case MiniScript::Script::TYPE_NONE: return "Script::TYPE_NONE";
			case MiniScript::Script::TYPE_FUNCTION: return "Script::TYPE_FUNCTION";
			case MiniScript::Script::TYPE_STACKLET: return "Script::TYPE_STACKLET";
			case MiniScript::Script::TYPE_ON: return "Script::TYPE_ON";
			case MiniScript::Script::TYPE_ONENABLED: return "Script::TYPE_ONENABLED";
		};
		//
		return string();
	}

	/**
	 * Create method name
	 * @param scriptIdx script index
	 * @return method name
	 */
	inline static const string createMethodName(MiniScript* miniScript, int scriptIdx) {
		if (scriptIdx < 0 || scriptIdx >= miniScript->getScripts().size()) return string();
		const auto& script = miniScript->getScripts()[scriptIdx];
		//
		string methodType;
		switch(script.type) {
			case MiniScript::Script::TYPE_NONE: break;
			case MiniScript::Script::TYPE_FUNCTION: methodType = "function"; break;
			case MiniScript::Script::TYPE_STACKLET: methodType = "stacklet"; break;
			case MiniScript::Script::TYPE_ON: methodType = "on"; break;
			case MiniScript::Script::TYPE_ONENABLED: methodType = "on_enabled"; break;
		};
		//
		return
			methodType + "_" +
			(script.name.empty() == false?script.name:(
				StringTools::regexMatch(script.condition, "[a-zA-Z0-9_]+") == true?
					script.condition:
					to_string(scriptIdx)
				)
			);
	}

	/**
	 * Create short method name
	 * @param scriptIdx script index
	 * @return short method name
	 */
	inline static const string createShortMethodName(MiniScript* miniScript, int scriptIdx) {
		if (scriptIdx < 0 || scriptIdx >= miniScript->getScripts().size()) return string();
		const auto& script = miniScript->getScripts()[scriptIdx];
		return
			(script.name.empty() == false?script.name:(
				StringTools::regexMatch(script.condition, "[a-zA-Z0-9_]+") == true?
					script.condition:
					to_string(scriptIdx)
				)
			);
	}

	/**
	 * Escape string variable
	 * @param str string
	 * @return escaped string
	 */
	inline static const string escapeString(const string& str) {
		return
			StringTools::replace(
				StringTools::replace(
					StringTools::replace(
						str,
						"\\",
						"\\\\"
					),
					"\"",
					"\\\""
				),
				"\n",
				"\\n"
			);
	}

	/**
	 * Check if variable has access statement
	 * @param variableStatement variable statement
	 * @return variable has statement
	 */
	inline static bool variableHasStatement(const string& variableStatement) {
		auto dotIdx = StringTools::indexOf(variableStatement, ".");
		if (dotIdx != string::npos) return true;
		auto squareBracketIdx = StringTools::indexOf(variableStatement, "[");
		if (squareBracketIdx != string::npos) return true;
		return false;
	}

	/**
	 * Create variable name
	 * @param variableStatement variable statement
	 * @return variable name
	 */
	inline static const string createVariableName(const string& variableStatement) {
		auto dotIdx = StringTools::indexOf(variableStatement, ".");
		if (dotIdx == string::npos) dotIdx = variableStatement.size();
		auto squareBracketIdx = StringTools::indexOf(variableStatement, "[");
		if (squareBracketIdx == string::npos) squareBracketIdx = variableStatement.size();
		auto cppVariableName = StringTools::substring(variableStatement, 0, dotIdx < squareBracketIdx?dotIdx:squareBracketIdx);
		return cppVariableName;
	}

	/**
	 * Create global variable name
	 * @param variableStatement variable statement
	 * @return global CPP variable name
	 */
	inline static const string createGlobalVariableName(const string& variableStatement) {
		auto dotIdx = StringTools::indexOf(variableStatement, ".");
		if (dotIdx == string::npos) dotIdx = variableStatement.size();
		auto squareBracketIdx = StringTools::indexOf(variableStatement, "[");
		if (squareBracketIdx == string::npos) squareBracketIdx = variableStatement.size();
		auto cppVariableName = "_" + StringTools::substring(variableStatement, 1, dotIdx < squareBracketIdx?dotIdx:squareBracketIdx);
		cppVariableName = StringTools::replace(cppVariableName, "$", "_");
		cppVariableName = StringTools::replace(cppVariableName, ":", "_");
		return cppVariableName;
	}

	/**
	 * Create local variable name
	 * @param variableStatement variable statement
	 * @return local CPP variable name
	 */
	inline static const string createLocalVariableName(const string& variableStatement) {
		auto dotIdx = StringTools::indexOf(variableStatement, ".");
		if (dotIdx == string::npos) dotIdx = variableStatement.size();
		auto squareBracketIdx = StringTools::indexOf(variableStatement, "[");
		if (squareBracketIdx == string::npos) squareBracketIdx = variableStatement.size();
		auto cppVariableName = "_" + StringTools::substring(variableStatement, 1, dotIdx < squareBracketIdx?dotIdx:squareBracketIdx);
		cppVariableName = StringTools::replace(cppVariableName, "$", "_");
		cppVariableName = StringTools::replace(cppVariableName, ":", "_");
		return cppVariableName;
	}

	/**
	 * Get all classes method names
	 * @param miniScript MiniScript instance
	 * @return all classes method names
	 */
	static const unordered_set<string> getAllClassesMethodNames(MiniScript* miniScript);

	/**
	 * Get all classes method names sorted
	 * @param miniScript MiniScript instance
	 * @return all classes method names sorted
	 */
	static const vector<string> getAllClassesMethodNamesSorted(MiniScript* miniScript);

	/**
	 * Get method names per classes
	 * @param miniScript MiniScript instance
	 * @return method names per classes
	 */
	static const unordered_map<string, vector<string>> getClassesMethodNames(MiniScript* miniScript);

	/**
	 * Determine variables
	 * @param miniScript MiniScript script instance
	 * @param globalVariables global variables
	 * @param localVariables local variables per script index
	 */
	static void determineVariables(MiniScript* miniScript, unordered_set<string>& globalVariables, vector<unordered_set<string>>& localVariables);

	/**
	 * Determine variables within syntax tree
	 * @param scriptIdx script index
	 * @param syntaxTreeNode syntax tree node
	 * @param globalVariables global variables
	 * @param localVariables local variables per script index
	 */
	static void determineVariables(int scriptIdx, const MiniScript::SyntaxTreeNode& syntaxTreeNode, unordered_set<string>& globalVariables, vector<unordered_set<string>>& localVariables);

	/**
	 * Gather method code
	 * @param miniScriptExtensionsCode MiniScript extensions code
	 * @param className class name
	 * @param register line register line
	 * @param methodCodeMap method code map
	 */
	static void gatherMethodCode(
		const vector<string>& miniScriptExtensionsCode,
		const string& className,
		int registerLine,
		unordered_map<string, vector<string>>& methodCodeMap
	);

	/**
	 * Generate variable access
	 * @param miniScript MiniScript instance
	 * @param generatedCode generated code
	 * @param scriptConditionIdx script condition index
	 * @param scriptIdx script index
	 * @param variableName variable name
	 * @param accessName access name
	 * @param indent indent
	 * @param getMethodArgumentVariable get method argument variable
	 * @param getVariable issue get variable access
	 * @param getVariableReference issue get variable reference access
	 * @param setVariable set variable
	 * @param setVariableReference set variable reference
	 * @param setConstant set constant
	 * @param returnValue return value
	 * @param statementEnd statement end
	 * @param getArgumentIdx get argument index
	 * @param setArgumentIdx set argument index
	 */
	static void generateVariableAccess(
		MiniScript* miniScript,
		string& generatedCode,
		int scriptConditionIdx,
		int scriptIdx,
		const string& variableName,
		const string& indent,
		bool getMethodArgumentVariable,
		bool getVariable,
		bool getVariableReference,
		bool setVariable,
		bool setVariableReference,
		bool setConstant,
		const string& returnValueStatement = "returnValue = ",
		const string& statementEnd = ";\n",
		int getArgumentIdx = 0,
		int setArgumentIdx = 1
	);

	/**
	 * Generate array access methods
	 * @param miniScript MiniScript instance
	 * @param generatedDefinitions generated definitions
	 * @param miniScriptClassName MiniScript class name
	 * @param scriptConditionIdx script condition index
	 * @param scriptIdx script index
	 * @param methodName method name
	 * @param syntaxTree syntax tree
	 * @param statement statement
	 * @param methodCodeMap method code map
	 * @param allMethods all methods
	 * @param condition condition
	 * @param argumentIndices argument indices stack
	 * @param depth depth
	 */
	static void generateArrayAccessMethods(
		MiniScript* miniScript,
		string& generatedDefinitions,
		const string& miniScriptClassName,
		int scriptConditionIdx,
		int scriptIdx,
		const string& methodName,
		const MiniScript::SyntaxTreeNode& syntaxTree,
		const MiniScript::Statement& statement,
		const unordered_map<string, vector<string>>& methodCodeMap,
		const unordered_set<string>& allMethods,
		bool condition,
		const vector<int>& argumentIndices = {},
		int depth = 0
	);

	/**
	 * Generate evaluate member access arrays
	 * @param miniScript MiniScript instance
	 * @param generatedDeclarations generated declarations
	 * @param generatedDefinitions generated definitions
	 */
	static void generateEvaluateMemberAccessArrays(
		MiniScript* miniScript,
		vector<string>& generatedDeclarations,
		vector<string>& generatedDefinitions
	);

	/**
	 * Generate array/map/set variable
	 * @param miniScript MiniScript instance
	 * @param scriptConditionIdx script condition index
	 * @param scriptIdx script index
	 * @param variable variable
	 * @param methodCodeMap method code map
	 * @param allMethods all methods
	 * @param methodName method name
	 * @param condition condition
	 * @param miniScriptClassName MiniScript class name
	 * @param generatedDefinitions generated definitions
	 * @param depth depth
	 * @param initializerDepth initializer depth
	 * @param postStatement post statement
	 */
	static void generateArrayMapSetVariable(
		MiniScript* miniScript,
		int scriptConditionIdx,
		int scriptIdx,
		const MiniScript::Variable& variable,
		const unordered_map<string, vector<string>>& methodCodeMap,
		const unordered_set<string>& allMethods,
		const string& methodName,
		bool condition,
		const string& miniScriptClassName,
		string& generatedDefinitions,
		int depth = 0,
		int initializerDepth = 0,
		const string& postStatement = string()
	);

	/**
	 * Generate array/map/set variable
	 * @param miniScript MiniScript instance
	 * @param generatedDefinitions generated definitions
	 * @param scriptConditionIdx script condition index
	 * @param scriptIdx script index
	 * @param miniScriptClassName MiniScript class name
	 * @param methodName method name
	 * @param syntaxTree syntax tree
	 * @param statement statement
	 * @param methodCodeMap method code map
	 * @param allMethods all methods
	 * @param condition condition
	 * @param argumentIndices argument indices
	 * @param depth depth
	 */
	static void generateArrayMapSetInitializer(
		MiniScript* miniScript,
		string& generatedDefinitions,
		int scriptConditionIdx,
		int scriptIdx,
		const string& miniScriptClassName,
		const string& methodName,
		const MiniScript::SyntaxTreeNode& syntaxTree,
		const MiniScript::Statement& statement,
		const unordered_map<string, vector<string>>& methodCodeMap,
		const unordered_set<string>& allMethods,
		bool condition,
		const vector<int>& argumentIndices = {},
		int depth = 0
	);

	/**
	 * Transpile statement
	 * @param miniScript MiniScript instance
	 * @param generatedCode generated code
	 * @param syntaxTree syntax tree
	 * @param statement script statement
	 * @param scriptConditionIdx script condition index
	 * @param scriptIdx script index
	 * @param statementIdx statement index
	 * @param methodCodeMap method code map
	 * @param allMethods all methods
	 * @param scriptStateChanged script could have state changed
	 * @param scriptStopped script could have been stopped
	 * @param enabledNamedConditions enabled named conditions
	 * @param depth depth
	 * @param argumentIndices argument indices
	 * @param returnValue return value
	 * @param injectCode code to additionally inject
	 * @param additionalIndent additional indent
	 */
	static bool transpileStatement(
		MiniScript* miniScript,
		string& generatedCode,
		const MiniScript::SyntaxTreeNode& syntaxTree,
		const MiniScript::Statement& statement,
		int scriptConditionIdx,
		int scriptIdx,
		int& statementIdx,
		const unordered_map<string, vector<string>>& methodCodeMap,
		const unordered_set<string>& allMethods,
		bool& scriptStateChanged,
		bool& scriptStopped,
		vector<string>& enabledNamedConditions,
		int depth = 0,
		const vector<int>& argumentIndices = {},
		const string& returnValue = string(),
		const string& injectCode = string(),
		int additionalIndent = 0
	);

	/**
	 * Transpile a script statement
	 * @param miniScript MiniScript instance
	 * @param generatedCode generated code
	 * @param scriptIdx script index
	 * @param methodCodeMap method code map
	 * @param allMethods all methods
	 * @return success
	 */
	static bool transpile(
		MiniScript* miniScript,
		string& generatedCode,
		int scriptIdx,
		const unordered_map<string, vector<string>>& methodCodeMap,
		const unordered_set<string>& allMethods
	);

	/**
	 * Transpile a condition
	 * @param miniScript MiniScript instance
	 * @param generatedCode generated code
	 * @param scriptIdx script index
	 * @param methodCodeMap method code map
	 * @param allMethods all methods
	 * @param returnValue return value
	 * @param injectCode inject code
	 * @return success
	 */
	static bool transpileCondition(
		MiniScript* miniScript,
		string& generatedCode,
		int scriptIdx,
		const unordered_map<string, vector<string>>& methodCodeMap,
		const unordered_set<string>& allMethods,
		const string& returnValue,
		const string& injectCode,
		int depth = 0
	);

	/**
	 * Create source code for given syntax tree node
	 * @param syntaxTreeNode syntax tree node
	 */
	static const string createSourceCode(const MiniScript::SyntaxTreeNode& syntaxTreeNode);

	/**
	 * Create source code for given syntax tree
	 * @param scriptType script type
	 * @param condition condition
	 * @param functionArguments function arguments
	 * @param name name of named conditions
	 * @param conditionSyntaxTree condition syntax tree
	 * @param syntaxTree syntax tree
	 */
	static const string createSourceCode(
		MiniScript::Script::Type scriptType,
		const string& condition,
		const vector<MiniScript::Script::Argument>& functionArguments,
		const string& name,
		const MiniScript::SyntaxTreeNode& conditionSyntaxTree,
		const vector<MiniScript::SyntaxTreeNode>& syntaxTree
	);

	/**
	 * Create source code for whole script
	 * @param miniScript MiniScript instance
	 * @return source code
	 */
	static const string createSourceCode(MiniScript* miniScript);

};
