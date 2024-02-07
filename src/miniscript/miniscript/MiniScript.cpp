#include <miniscript/miniscript/MiniScript.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <span>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <miniscript/miniscript/MiniScript.h>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/ApplicationMethods.h>
#include <miniscript/miniscript/ArrayMethods.h>
#include <miniscript/miniscript/BaseMethods.h>
#include <miniscript/miniscript/ByteArrayMethods.h>
#include <miniscript/miniscript/CryptographyMethods.h>
#include <miniscript/miniscript/ConsoleMethods.h>
#include <miniscript/miniscript/ContextMethods.h>
#include <miniscript/miniscript/FileSystemMethods.h>
#include <miniscript/miniscript/HTTPDownloadClientClass.h>
#include <miniscript/miniscript/JSONMethods.h>
#include <miniscript/miniscript/MapMethods.h>
#include <miniscript/miniscript/MathMethods.h>
#include <miniscript/miniscript/NetworkMethods.h>
#include <miniscript/miniscript/ScriptMethods.h>
#include <miniscript/miniscript/SetMethods.h>
#include <miniscript/miniscript/StringMethods.h>
#include <miniscript/miniscript/TimeMethods.h>
#include <miniscript/miniscript/XMLMethods.h>

#include <miniscript/math/Math.h>
#include <miniscript/os/filesystem/FileSystem.h>
#include <miniscript/os/threading/Thread.h>
#include <miniscript/utilities/Character.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Integer.h>
#include <miniscript/utilities/StringTokenizer.h>
#include <miniscript/utilities/StringTools.h>
#include <miniscript/utilities/SHA256.h>
#include <miniscript/utilities/Time.h>

using std::find;
using std::ifstream;
using std::initializer_list;
using std::make_unique;
using std::map;
using std::move;
using std::remove;
using std::reverse;
using std::smatch;
using std::sort;
using std::span;
using std::stack;
using std::string;
using std::string_view;
using std::stringstream;
using std::to_string;
using std::unique_ptr;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using miniscript::miniscript::MiniScript;

using miniscript::miniscript::ApplicationMethods;
using miniscript::miniscript::ArrayMethods;
using miniscript::miniscript::BaseMethods;
using miniscript::miniscript::ByteArrayMethods;
using miniscript::miniscript::CryptographyMethods;
using miniscript::miniscript::ConsoleMethods;
using miniscript::miniscript::ContextMethods;
using miniscript::miniscript::FileSystemMethods;
using miniscript::miniscript::HTTPDownloadClientClass;
using miniscript::miniscript::JSONMethods;
using miniscript::miniscript::MapMethods;
using miniscript::miniscript::MathMethods;
using miniscript::miniscript::NetworkMethods;
using miniscript::miniscript::ScriptMethods;
using miniscript::miniscript::SetMethods;
using miniscript::miniscript::StringMethods;
using miniscript::miniscript::TimeMethods;
using miniscript::miniscript::XMLMethods;

using _Math = miniscript::math::Math;
using _FileSystem = miniscript::os::filesystem::FileSystem;
using _Character = miniscript::utilities::Character;
using _Console = miniscript::utilities::Console;
using _Integer = miniscript::utilities::Integer;
using _StringTokenizer = miniscript::utilities::StringTokenizer;
using _StringTools = miniscript::utilities::StringTools;
using _SHA256 = miniscript::utilities::SHA256;
using _Time = miniscript::utilities::Time;
using _Thread = miniscript::os::threading::Thread;

const string MiniScript::OPERATOR_CHARS = "+-!~/%<>=&^|";

vector<MiniScript::DataType*> MiniScript::dataTypes;
MiniScript::ShutdownRAII MiniScript::shutdownRAII(MiniScript::dataTypes);

const string MiniScript::METHOD_SCRIPTCALL = "script.call";
const string MiniScript::METHOD_SCRIPTCALLSTACKLET = "script.callStacklet";
const string MiniScript::METHOD_SCRIPTCALLBYINDEX = "script.callByIndex";
const string MiniScript::METHOD_SCRIPTCALLSTACKLETBYINDEX = "script.callStackletByIndex";

const string MiniScript::METHOD_ENABLENAMEDCONDITION = "script.enableNamedCondition";
const string MiniScript::METHOD_DISABLENAMEDCONDITION = "script.disableNamedCondition";

const string MiniScript::Variable::TYPENAME_NONE = "";
const string MiniScript::Variable::TYPENAME_NULL = "Null";
const string MiniScript::Variable::TYPENAME_BOOLEAN = "Boolean";
const string MiniScript::Variable::TYPENAME_INTEGER = "Integer";
const string MiniScript::Variable::TYPENAME_FLOAT = "Float";
const string MiniScript::Variable::TYPENAME_FUNCTION = "Function";
const string MiniScript::Variable::TYPENAME_STACKLET = "Stacklet";
const string MiniScript::Variable::TYPENAME_NUMBER = "Number";
const string MiniScript::Variable::TYPENAME_MIXED = "Mixed";
const string MiniScript::Variable::TYPENAME_STRING = "String";
const string MiniScript::Variable::TYPENAME_BYTEARRAY = "ByteArray";
const string MiniScript::Variable::TYPENAME_ARRAY = "Array";
const string MiniScript::Variable::TYPENAME_MAP = "Map";
const string MiniScript::Variable::TYPENAME_SET = "Set";

const vector<string> MiniScript::Method::CONTEXTFUNCTIONS_ALL = {};

void MiniScript::initialize() {
	//
	_Console::initialize();
	//
	registerDataType(new HTTPDownloadClientClass());
	//
	HTTPDownloadClientClass::initialize();
}

const string MiniScript::getBaseClassHeader() {
	return "miniscript/miniscript/MiniScript.h";
}

const string MiniScript::getBaseClass() {
	return "miniscript::miniscript::MiniScript";
}

const vector<string> MiniScript::getTranspilationUnits() {
	return {
		"src/miniscript/miniscript/ApplicationMethods.cpp",
		"src/miniscript/miniscript/ArrayMethods.cpp",
		"src/miniscript/miniscript/BaseMethods.cpp",
		"src/miniscript/miniscript/ByteArrayMethods.cpp",
		"src/miniscript/miniscript/ConsoleMethods.cpp",
		"src/miniscript/miniscript/ContextMethods.cpp",
		"src/miniscript/miniscript/CryptographyMethods.cpp",
		"src/miniscript/miniscript/FileSystemMethods.cpp",
		"src/miniscript/miniscript/HTTPDownloadClientClass.cpp",
		"src/miniscript/miniscript/JSONMethods.cpp",
		"src/miniscript/miniscript/MapMethods.cpp",
		"src/miniscript/miniscript/MathMethods.cpp",
		"src/miniscript/miniscript/NetworkMethods.cpp",
		"src/miniscript/miniscript/ScriptMethods.cpp",
		"src/miniscript/miniscript/SetMethods.cpp",
		"src/miniscript/miniscript/StringMethods.cpp",
		"src/miniscript/miniscript/TimeMethods.cpp",
		"src/miniscript/miniscript/XMLMethods.cpp"
	};
}

MiniScript::MiniScript() {
	for (auto dataType: dataTypes) {
		if (dataType->isRequiringGarbageCollection() == false) continue;
		// create script context
		auto scriptContext = dataType->createScriptContext();
		scriptContext->setMiniScript(this);
		scriptContext->setIndex(garbageCollectionDataTypes.size());
		//
		garbageCollectionDataTypes.emplace_back(dataType,scriptContext);
		garbageCollectionScriptContextsByDataType[dataType->getType()] = scriptContext;
	}
	setNative(false);
	pushScriptState();
}

MiniScript::~MiniScript() {
	for (const auto& [methodName, method]: this->methods) delete method;
	for (const auto& [stateMachineStateId, stateMachineState]: this->stateMachineStates) delete stateMachineState;
	while (scriptStateStack.empty() == false) popScriptState();
	garbageCollection();
	for (auto& garbageCollectionDataType: garbageCollectionDataTypes) garbageCollectionDataType.dataType->deleteScriptContext(garbageCollectionDataType.context);
}

void MiniScript::registerStateMachineState(StateMachineState* state) {
	auto stateMachineStateIt = stateMachineStates.find(state->getId());
	if (stateMachineStateIt != stateMachineStates.end()) {
		_Console::printLine("MiniScript::registerStateMachineState(): " + scriptFileName + ": state with id + " + to_string(state->getId()) + ", name " + state->getName() + " already registered.");
		return;
	}
	stateMachineStates[state->getId()] = state;
}

void MiniScript::initializeNative() {
}

void MiniScript::complain(const string& methodName, const Statement& statement) {
	auto argumentsInformation = getArgumentsInformation(methodName);
	if (argumentsInformation.empty() == true) argumentsInformation = "None";
	_Console::printLine(methodName + "(): " + getStatementInformation(statement) + ": argument mismatch: expected arguments: " + argumentsInformation);
}

void MiniScript::complain(const string& methodName, const Statement& statement, const string& message) {
	_Console::printLine(methodName + "(): " + getStatementInformation(statement) + ": " + message);
}

void MiniScript::registerMethod(Method* method) {
	auto methodsIt = methods.find(method->getMethodName());
	if (methodsIt != methods.end()) {
		_Console::printLine("MiniScript::registerMethod(): " + scriptFileName + ": method with name " + method->getMethodName() + " already registered.");
		return;
	}
	methods[method->getMethodName()] = method;
}

void MiniScript::registerDataType(DataType* dataType) {
	dataType->setType(static_cast<VariableType>(TYPE_PSEUDO_DATATYPES + dataTypes.size()));
	dataTypes.push_back(dataType);
}

void MiniScript::executeNextStatement() {
	auto& scriptState = getScriptState();
	if (scriptState.scriptIdx == SCRIPTIDX_NONE || scriptState.statementIdx == STATEMENTIDX_NONE || scriptState.running == false) return;
	//
	const auto& script = scripts[scriptState.scriptIdx];
	if (script.statements.empty() == true) return;
	// take goto statement index into account
	if (scriptState.gotoStatementIdx != STATEMENTIDX_NONE) {
		scriptState.statementIdx = scriptState.gotoStatementIdx;
		scriptState.gotoStatementIdx = STATEMENTIDX_NONE;
	}
	//
	const auto& statement = script.statements[scriptState.statementIdx];
	const auto& syntaxTree = script.syntaxTree[scriptState.statementIdx];
	if (VERBOSE == true) _Console::printLine("MiniScript::executeScriptLine(): " + getStatementInformation(statement));

	//
	auto returnValue = executeStatement(syntaxTree, statement);

	//
	scriptState.statementIdx++;
	if (scriptState.statementIdx >= script.statements.size()) {
		scriptState.scriptIdx = SCRIPTIDX_NONE;
		scriptState.statementIdx = STATEMENTIDX_NONE;
		setScriptStateState(STATEMACHINESTATE_WAIT_FOR_CONDITION);
	}
}

bool MiniScript::parseStatement(const string_view& executableStatement, string_view& methodName, vector<string_view>& arguments, const Statement& statement, string& accessObjectMemberStatement) {
	if (VERBOSE == true) _Console::printLine("MiniScript::parseStatement(): " + getStatementInformation(statement) + ": '" + string(executableStatement) + "'");
	string_view objectMemberAccessObject;
	string_view objectMemberAccessMethod;
	int executableStatementStartIdx = 0;
	auto objectMemberAccess = getObjectMemberAccess(executableStatement, objectMemberAccessObject, objectMemberAccessMethod, executableStatementStartIdx, statement);
	auto bracketCount = 0;
	auto squareBracketCount = 0;
	auto curlyBracketCount = 0;
	auto quote = '\0';
	auto methodStart = string::npos;
	auto methodEnd = string::npos;
	auto argumentStart = string::npos;
	auto argumentEnd = string::npos;
	auto quotedArgumentStart = string::npos;
	auto quotedArgumentEnd = string::npos;
	auto lc  = '\0';
	//
	auto viewIsStringMethodAccess = [](const string_view& candidate) -> bool {
		if (candidate.size() == 0) return false;
		//
		auto i = 0;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++); if (i >= candidate.size()) return false;
		// -
		if (candidate[i++] != '-') return false;
		//
		if (i >= candidate.size()) return false;
		// >
		if (candidate[i++] != '>') return false;
		//
		return true;
	};
	//
	for (auto i = executableStatementStartIdx; i < executableStatement.size(); i++) {
		auto c = executableStatement[i];
		// quotes
		if (squareBracketCount == 0 && curlyBracketCount == 0 && ((c == '"' || c == '\'') && lc != '\\')) {
			if (bracketCount == 1) {
				if (quote == '\0') {
					quotedArgumentStart = i;
					quote = c;
				} else
				if (quote == c) {
					quotedArgumentEnd = i;
					quote = '\0';
				}
			} else {
				if (quote == '\0') {
					if (argumentStart == string::npos) {
						argumentStart = i;
					}
					quote = c;
				} else
				if (quote == c) {
					argumentEnd = i;
					quote = '\0';
				}
			}
		} else
		// quotes end
		if (quote != '\0') {
			if (bracketCount == 1) {
				quotedArgumentEnd = i;
			} else {
				if (argumentStart == string::npos) {
					argumentStart = i;
				} else {
					argumentEnd = i;
				}
			}
		} else {
			// no quotes, handle (
			if (c == '(') {
				bracketCount++;
				if (bracketCount > 1) {
					if (argumentStart == string::npos) {
						argumentStart = i;
					} else {
						argumentEnd = i;
					}
				}
			} else
			// )
			if (c == ')') {
				bracketCount--;
				if (bracketCount == 0) {
					if (quotedArgumentStart != string::npos) {
						// do we have a quoted argument end
						if (quotedArgumentEnd == string::npos) {
							quotedArgumentEnd = i - 1;
						} else
						// extend string with string method access if feasible
						if (quotedArgumentEnd + 1 < executableStatement.size() &&
							viewIsStringMethodAccess(string_view(&executableStatement[quotedArgumentEnd + 1], executableStatement.size() - (quotedArgumentEnd + 1))) == true) {
							quotedArgumentEnd = i - 1;
						}
						//
						auto argumentLength = quotedArgumentEnd - quotedArgumentStart + 1;
						if (argumentLength > 0) arguments.push_back(_StringTools::viewTrim(string_view(&executableStatement[quotedArgumentStart], argumentLength)));
						quotedArgumentStart = string::npos;
						quotedArgumentEnd = string::npos;
					} else
					if (argumentStart != string::npos) {
						if (argumentEnd == string::npos) argumentEnd = i - 1;
						auto argumentLength = argumentEnd - argumentStart + 1;
						if (argumentLength > 0) arguments.push_back(_StringTools::viewTrim(string_view(&executableStatement[argumentStart], argumentLength)));
						argumentStart = string::npos;
						argumentEnd = string::npos;
					}
				} else {
					if (argumentStart == string::npos) {
						argumentStart = i;
					} else {
						argumentEnd = i;
					}
				}
			} else
			// [
			if (c == '[' && curlyBracketCount == 0) {
				if (squareBracketCount == 0) {
					if (argumentStart == string::npos) {
						argumentStart = i;
					} else {
						argumentEnd = i;
					}
				}
				squareBracketCount++;
			} else
			// ]
			if (c == ']' && curlyBracketCount == 0) {
				squareBracketCount--;
				if (squareBracketCount == 0) {
					if (argumentStart == string::npos) {
						argumentStart = i;
					} else {
						argumentEnd = i;
					}
				}
			} else
			// {
			if (c == '{') {
				if (curlyBracketCount == 0) {
					if (argumentStart == string::npos) {
						argumentStart = i;
					} else {
						argumentEnd = i;
					}
				}
				curlyBracketCount++;
			} else
			// }
			if (c == '}') {
				curlyBracketCount--;
				if (curlyBracketCount == 0) {
					if (argumentStart == string::npos) {
						argumentStart = i;
					} else {
						argumentEnd = i;
					}
				}
			} else
			// ,
			if (squareBracketCount == 0 && curlyBracketCount == 0) {
				if (c == ',') {
					if (bracketCount == 1) {
						if (quotedArgumentStart != string::npos) {
							// do we have a quoted argument end
							if (quotedArgumentEnd == string::npos) {
								quotedArgumentEnd = i - 1;
							} else
							// extend string with string method access if feasible
							if (quotedArgumentEnd + 1 < executableStatement.size() &&
								viewIsStringMethodAccess(string_view(&executableStatement[quotedArgumentEnd + 1], executableStatement.size() - (quotedArgumentEnd + 1))) == true) {
								quotedArgumentEnd = i - 1;
							}
							auto argumentLength = quotedArgumentEnd - quotedArgumentStart + 1;
							if (argumentLength > 0) arguments.push_back(_StringTools::viewTrim(string_view(&executableStatement[quotedArgumentStart], argumentLength)));
							quotedArgumentStart = string::npos;
							quotedArgumentEnd = string::npos;
						} else
						if (argumentStart != string::npos) {
							if (argumentEnd == string::npos) argumentEnd = i - 1;
							auto argumentLength = argumentEnd - argumentStart + 1;
							if (argumentLength > 0) arguments.push_back(_StringTools::viewTrim(string_view(&executableStatement[argumentStart], argumentEnd - argumentStart + 1)));
							argumentStart = string::npos;
							argumentEnd = string::npos;
						}
					} else {
						if (argumentStart == string::npos) {
							argumentStart = i + 1;
						} else {
							argumentEnd = i;
						}
					}
				} else
				if (bracketCount == 0) {
					if (methodStart == string::npos) methodStart = i; else methodEnd = i;
				} else {
					if (argumentStart == string::npos) {
						if (_Character::isSpace(c) == false) {
							argumentStart = i;
						}
					} else {
						argumentEnd = i;
					}
				}
			}
		}
		//
		lc = c;
	}

	// extract method name
	if (methodStart != string::npos && methodEnd != string::npos) {
		methodName = _StringTools::viewTrim(string_view(&executableStatement[methodStart], methodEnd - methodStart + 1));
	}

	// handle object member access and generate internal.script.evaluateMemberAccess call
	if (objectMemberAccess == true) {
		// construct executable statement and arguments
		string_view evaluateMemberAccessMethodName;
		vector<string_view> evaluateMemberAccessArguments;

		//
		auto objectMemberAccessObjectVariable = viewIsVariableAccess(objectMemberAccessObject);

		// construct new method name and argument string views
		accessObjectMemberStatement.reserve(16384); // TODO: check me later
		auto idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= "internal.script.evaluateMemberAccess";
		evaluateMemberAccessMethodName = string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx);
		accessObjectMemberStatement+= "(";
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= objectMemberAccessObjectVariable == true?"\"" + string(objectMemberAccessObject) + "\"":"null";
		evaluateMemberAccessArguments.push_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx));
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= ", ";
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= objectMemberAccessObjectVariable == true?"null":string(objectMemberAccessObject);
		evaluateMemberAccessArguments.push_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx));
		accessObjectMemberStatement+= ", ";
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= "\"" + string(methodName) + "\"";
		evaluateMemberAccessArguments.push_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx));
		for (const auto& argument: arguments) {
			auto argumentVariable = viewIsVariableAccess(argument);
			accessObjectMemberStatement+= ", ";
			idx = accessObjectMemberStatement.size();
			accessObjectMemberStatement+= argumentVariable == true?"\"" + string(argument) + "\"":"null";
			evaluateMemberAccessArguments.push_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx));
			accessObjectMemberStatement+= ", ";
			idx = accessObjectMemberStatement.size();
			accessObjectMemberStatement+= argumentVariable == true?"null":string(argument);
			evaluateMemberAccessArguments.push_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx));
		}
		accessObjectMemberStatement+= ")";
		// set up new results
		methodName = evaluateMemberAccessMethodName;
		arguments = evaluateMemberAccessArguments;
	}

	//
	if (VERBOSE == true) {
		_Console::print("MiniScript::parseStatement(): " + getStatementInformation(statement) + ": method: '" + string(methodName) + "', arguments: ");
		int variableIdx = 0;
		for (const auto& argument: arguments) {
			if (variableIdx > 0) _Console::print(", ");
			_Console::print("'" + string(argument) + "'");
			variableIdx++;
		}
		_Console::printLine();
	}

	// complain about bracket count
	if (bracketCount != 0) {
		_Console::printLine(getStatementInformation(statement) + ": " + string(executableStatement) + "': unbalanced bracket count: " + to_string(_Math::abs(bracketCount)) + " " + (bracketCount < 0?"too much closed":"still open"));
		//
		parseErrors.push_back(string(executableStatement) + ": unbalanced bracket count: " + to_string(_Math::abs(bracketCount)) + " " + (bracketCount < 0?"too much closed":"still open"));
		//
		return false;
	}
	// complain about square bracket count
	if (squareBracketCount != 0) {
		_Console::printLine(getStatementInformation(statement) + ": " + string(executableStatement) + "': unbalanced square bracket count: " + to_string(_Math::abs(squareBracketCount)) + " " + (squareBracketCount < 0?"too much closed":"still open"));
		//
		parseErrors.push_back(string(executableStatement) + ": unbalanced square bracket count: " + to_string(_Math::abs(squareBracketCount)) + " " + (squareBracketCount < 0?"too much closed":"still open"));
		//
		return false;
	}
	// complain about curly bracket count
	if (curlyBracketCount != 0) {
		_Console::printLine(getStatementInformation(statement) + ": " + string(executableStatement) + "': unbalanced curly bracket count: " + to_string(_Math::abs(curlyBracketCount)) + " " + (curlyBracketCount < 0?"too much closed":"still open"));
		//
		parseErrors.push_back(string(executableStatement) + ": unbalanced curly bracket count: " + to_string(_Math::abs(curlyBracketCount)) + " " + (curlyBracketCount < 0?"too much closed":"still open"));
		//
		return false;
	}

	//
	return true;
}

MiniScript::Variable MiniScript::executeStatement(const SyntaxTreeNode& syntaxTree, const Statement& statement) {
	if (VERBOSE == true) _Console::printLine("MiniScript::executeStatement(): " + getStatementInformation(statement) + "': " + syntaxTree.value.getValueAsString() + "(" + getArgumentsAsString(syntaxTree.arguments) + ")");
	// return on literal or empty syntaxTree
	if (syntaxTree.type == SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL) {
		return initializeVariable(syntaxTree.value);
	}
	//
	vector<Variable> arguments;
	Variable returnValue;
	// construct argument values
	for (const auto& argument: syntaxTree.arguments) {
		switch (argument.type) {
			case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
				{
					arguments.push_back(initializeVariable(argument.value));
					break;
				}
			case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:
			case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD:
				{
					arguments.push_back(executeStatement(argument, statement));
					break;
				}
			default:
				break;
		}
	}
	//
	if (VERBOSE == true) {
		_Console::printLine("MiniScript::executeStatement(): '" + getStatementInformation(statement) + ": " + syntaxTree.value.getValueAsString() + "(" + getArgumentsAsString(syntaxTree.arguments) + ")");
	}
	// try first function
	if (syntaxTree.type == SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION) {
		// call
		span argumentsSpan(arguments);
		call(syntaxTree.getScriptIdx(), argumentsSpan, returnValue);
		//
		return returnValue;
	} else
	if (syntaxTree.type == SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_STACKLET) {
		// call
		span argumentsSpan(arguments);
		callStacklet(syntaxTree.getScriptIdx(), argumentsSpan, returnValue);
		//
		return returnValue;
	} else
	if (syntaxTree.type == SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD) {
		// try methods next
		auto method = syntaxTree.getMethod();
		// validate arguments
		{
			auto argumentIdx = 0;
			for (const auto& argumentType: method->getArgumentTypes()) {
				auto argumentOk = true;
				// nullable and NULL argument
				if (argumentType.nullable == true &&
					argumentIdx >= 0 && argumentIdx < arguments.size() &&
					arguments[argumentIdx].getType() == TYPE_NULL) {
					argumentOk = true;
				} else {
					// otherwise check the argument
					switch(argumentType.type) {
						case TYPE_NULL:
							break;
						case TYPE_BOOLEAN:
							{
								bool booleanValue;
								argumentOk = getBooleanValue(arguments, argumentIdx, booleanValue, argumentType.optional);
							}
							break;
						case TYPE_INTEGER:
							{
								int64_t integerValue;
								argumentOk = getIntegerValue(arguments, argumentIdx, integerValue, argumentType.optional);
							}
							break;
						case TYPE_FLOAT:
							{
								float floatValue;
								argumentOk = getFloatValue(arguments, argumentIdx, floatValue, argumentType.optional);
							}
							break;
						case TYPE_PSEUDO_NUMBER:
							{
								float floatValue;
								argumentOk = getFloatValue(arguments, argumentIdx, floatValue, argumentType.optional);
								break;
							}
						case TYPE_PSEUDO_MIXED:
							{
								argumentOk = true;
								break;
							}
						case TYPE_STRING:
							{
								string stringValue;
								argumentOk = getStringValue(arguments, argumentIdx, stringValue, argumentType.optional);
							}
							break;
						case TYPE_BYTEARRAY:
							{
								argumentOk =
									argumentIdx < 0 || argumentIdx >= arguments.size()?
										argumentType.optional:
										arguments[argumentIdx].getType() == TYPE_BYTEARRAY;
								break;
							}
						case TYPE_ARRAY:
							{
								argumentOk =
									argumentIdx < 0 || argumentIdx >= arguments.size()?
										argumentType.optional:
										arguments[argumentIdx].getType() == TYPE_ARRAY;
								break;
							}
						case TYPE_MAP:
							{
								argumentOk =
									argumentIdx < 0 || argumentIdx >= arguments.size()?
										argumentType.optional:
										arguments[argumentIdx].getType() == TYPE_MAP;
								break;
							}
						case TYPE_SET:
							{
								argumentOk =
									argumentIdx < 0 || argumentIdx >= arguments.size()?
										argumentType.optional:
										arguments[argumentIdx].getType() == TYPE_SET;
								break;
							}
						default:
							{
								// custom data types
								argumentOk =
									argumentIdx < 0 || argumentIdx >= arguments.size()?
										argumentType.optional:
										arguments[argumentIdx].getType() == argumentType.type;
								break;
							}

					}
				}
				if (argumentOk == false) {
					_Console::printLine(
						getStatementInformation(statement) +
						": method '" + string(syntaxTree.value.getValueAsString()) + "'" +
						": argument value @ " + to_string(argumentIdx) + ": expected " + Variable::getTypeAsString(argumentType.type) + ", but got: " + (argumentIdx < arguments.size()?arguments[argumentIdx].getAsString():"nothing"));
				}
				argumentIdx++;
			}
			if (method->isVariadic() == false && arguments.size() > method->getArgumentTypes().size()) {
				_Console::printLine(
					getStatementInformation(statement) +
					": method '" + string(syntaxTree.value.getValueAsString()) + "'" +
					": too many arguments: expected: " + to_string(method->getArgumentTypes().size()) + ", got " + to_string(arguments.size()));
			}
		}
		// execute method
		span argumentsSpan(arguments);
		method->executeMethod(argumentsSpan, returnValue, statement);
		// check return type
		if (method->isReturnValueNullable() == true && returnValue.getType() == TYPE_NULL) {
			// no op, this is a valid return value
		} else
		if (MiniScript::Variable::isExpectedType(returnValue.getType(), method->getReturnValueType()) == false) {
			_Console::printLine(
				getStatementInformation(statement) +
				": method '" + string(syntaxTree.value.getValueAsString()) + "'" +
				": return value: expected " + Variable::getReturnTypeAsString(method->getReturnValueType(), method->isReturnValueNullable()) + ", but got: " + Variable::getReturnTypeAsString(returnValue.getType(), false));
		}
		//
		return returnValue;
	}
	//
	return returnValue;
}

bool MiniScript::createStatementSyntaxTree(int scriptIdx, const string_view& methodName, const vector<string_view>& arguments, const Statement& statement, SyntaxTreeNode& syntaxTree) {
	if (VERBOSE == true) _Console::printLine("MiniScript::createScriptStatementSyntaxTree(): " + getStatementInformation(statement) + ": " + string(methodName) + "(" + getArgumentsAsString(arguments) + ")");
	// method/function
	auto functionScriptIdx = SCRIPTIDX_NONE;
	Method* method = nullptr;
	// try first user functions
	{
		auto functionsIt = functions.find(string(methodName));
		if (functionsIt != functions.end()) {
			functionScriptIdx = functionsIt->second;
		}
	}
	// try methods next
	{
		auto methodsIt = methods.find(string(methodName));
		if (methodsIt != methods.end()) {
			method = methodsIt->second;
		}
	}

	// arguments
	vector<bool> argumentReferences(0);
	if (functionScriptIdx != SCRIPTIDX_NONE) {
		const auto& script = scripts[functionScriptIdx];
		if (script.type == Script::TYPE_STACKLET) {
			if (arguments.empty() == false) {
				_Console::printLine(getStatementInformation(statement) + ": A stacklet must not be called with any arguments: " + string(methodName));
				//
				parseErrors.push_back(getStatementInformation(statement) + ": A stacklet must not be called with any arguments: " + string(methodName));
				//
				return false;
			}
		} else {
			argumentReferences.resize(script.arguments.size());
			auto argumentIdx = 0;
			for (const auto& argument: scripts[functionScriptIdx].arguments) {
				argumentReferences[argumentIdx++] = argument.reference;
			}
		}
	} else
	if (method != nullptr) {
		argumentReferences.resize(method->getArgumentTypes().size());
		auto argumentIdx = 0;
		for (const auto& argument: method->getArgumentTypes()) {
			argumentReferences[argumentIdx++] = argument.reference;
		}
	}
	auto argumentIdx = 0;
	for (const auto& argument: arguments) {
		// object member access
		string_view accessObjectMemberObject;
		string_view accessObjectMemberMethod;
		int accessObjectMemberStartIdx;
		vector<string_view> lamdaFunctionStackletArguments;
		string_view lamdaFunctionStackletScriptCode;
		if (viewIsLamdaFunction(argument, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode) == true) {
			Variable variable;
			createLamdaFunction(variable, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode, false, statement);
			SyntaxTreeNode subSyntaxTree(SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL, variable, nullptr, {});
			syntaxTree.arguments.push_back(subSyntaxTree);
		} else
		if (viewIsStacklet(argument, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode) == true) {
			string scopeName;
			// empty scope means root scope
			if (scriptIdx != SCRIPTIDX_NONE) {
				// function are a valid scope for stacklets
				if (scripts[scriptIdx].type == Script::TYPE_FUNCTION) {
					scopeName = scripts[scriptIdx].condition;
				}
				// TODO: as well as stacklets
			}
			Variable variable;
			createStacklet(variable, scopeName, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode, statement);
			SyntaxTreeNode subSyntaxTree(SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL, variable, nullptr, {});
			syntaxTree.arguments.push_back(subSyntaxTree);
		} else
		if (getObjectMemberAccess(argument, accessObjectMemberObject, accessObjectMemberMethod, accessObjectMemberStartIdx, statement) == true) {
			// method call
			string_view subMethodName;
			vector<string_view> subArguments;
			string accessObjectMemberStatement;
			if (parseStatement(argument, subMethodName, subArguments, statement, accessObjectMemberStatement) == true) {
				SyntaxTreeNode subSyntaxTree;
				if (createStatementSyntaxTree(scriptIdx, subMethodName, subArguments, statement, subSyntaxTree) == false) {
					return false;
				}
				syntaxTree.arguments.push_back(subSyntaxTree);
			} else {
				return false;
			}
		} else
		// plain variable
		if (viewIsVariableAccess(argument) == true) {
			//
			Variable value;
			value.setValue(deescape(argument, statement));

			// look up getVariable method
			string methodName = argumentIdx >= argumentReferences.size() || argumentReferences[argumentIdx] == false?"getVariable":"getVariableReference";
			Method* method = nullptr;
			{
				auto methodsIt = methods.find(methodName);
				if (methodsIt != methods.end()) {
					method = methodsIt->second;
				} else {
					_Console::printLine(getStatementInformation(statement) + ": Unknown method: " + methodName);
					//
					parseErrors.push_back(getStatementInformation(statement) + ": Unknown method: " + methodName);
					//
					return false;
				}
			}

			syntaxTree.arguments.emplace_back(
				SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD,
				MiniScript::Variable(deescape(methodName, statement)),
				method,
				initializer_list<SyntaxTreeNode>
					{
						{
							SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL,
							value,
							nullptr,
							{}
						}
					}
			);
		} else
		// method call
		if (argument.empty() == false &&
			_StringTools::viewStartsWith(argument, "\"") == false &&
			_StringTools::viewEndsWith(argument, "\"") == false &&
			_StringTools::viewStartsWith(argument, "'") == false &&
			_StringTools::viewEndsWith(argument, "'") == false &&
			_StringTools::viewStartsWith(argument, "[") == false &&
			_StringTools::viewEndsWith(argument, "]") == false &&
			_StringTools::viewStartsWith(argument, "}") == false &&
			_StringTools::viewEndsWith(argument, "}") == false &&
			argument.find('(') != string::npos &&
			argument.find(')') != string::npos) {
			// method call
			string_view subMethodName;
			vector<string_view> subArguments;
			string accessObjectMemberStatement;
			if (parseStatement(argument, subMethodName, subArguments, statement, accessObjectMemberStatement) == true) {
				SyntaxTreeNode subSyntaxTree;
				if (createStatementSyntaxTree(scriptIdx, subMethodName, subArguments, statement, subSyntaxTree) == false) {
					return false;
				}
				syntaxTree.arguments.push_back(subSyntaxTree);
			} else {
				//
				return false;
			}
		} else {
			// string literal
			Variable argumentValue;
			if ((_StringTools::viewStartsWith(argument, "\"") == true &&
				_StringTools::viewEndsWith(argument, "\"") == true) ||
				(_StringTools::viewStartsWith(argument, "'") == true &&
				_StringTools::viewEndsWith(argument, "'") == true)) {
				//
				Variable value;
				value.setValue(deescape(_StringTools::viewSubstring(argument, 1, argument.size() - 1), statement));
				//
				syntaxTree.arguments.emplace_back(
					SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL,
					value,
					nullptr,
					initializer_list<SyntaxTreeNode>{}
				);
			} else {
				// implicitely literal
				Variable value;
				value.setImplicitTypedValueFromStringView(argument, this, scriptIdx, statement);
				//
				syntaxTree.arguments.emplace_back(
					SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL,
					value,
					nullptr,
					initializer_list<SyntaxTreeNode>{}
				);
			}
		}
		//
		argumentIdx++;
	}
	// try first user functions
	if (functionScriptIdx != SCRIPTIDX_NONE) {
		syntaxTree.type = scripts[functionScriptIdx].type == Script::TYPE_FUNCTION?SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_STACKLET;
		syntaxTree.value.setValue(deescape(methodName, statement));
		syntaxTree.setScriptIdx(functionScriptIdx);
		//
		return true;
	} else
	// try methods next
	if (method != nullptr) {
		syntaxTree.type = SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD;
		syntaxTree.value.setValue(deescape(methodName, statement));
		syntaxTree.setMethod(method);
		//
		return true;
	} else {
		_Console::printLine(getStatementInformation(statement) + ": Unknown function/method: " + string(methodName) + "()");
		//
		parseErrors.push_back(getStatementInformation(statement) + ": Unknown function/method: " + string(methodName) + "()");
		//
		return false;
	}
	//
	return false;
}

bool MiniScript::setupFunctionAndStackletScriptIndices(int scriptIdx) {
	//
	auto& script = scripts[scriptIdx];
	auto statementIdx = STATEMENTIDX_FIRST;
	//
	for (auto& syntaxTreeNode: script.syntaxTree) {
		auto& statement = script.statements[statementIdx++];
		//
		if (setupFunctionAndStackletScriptIndices(syntaxTreeNode, statement) == false) {
			//
			return false;
		}
	}
	//
	return true;

}

bool MiniScript::setupFunctionAndStackletScriptIndices(SyntaxTreeNode& syntaxTreeNode, const Statement& statement) {
	switch (syntaxTreeNode.type) {
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
			{
				if (syntaxTreeNode.value.getType() == MiniScript::TYPE_FUNCTION_ASSIGNMENT) {
					string function;
					auto functionScriptIdx = SCRIPTIDX_NONE;
					if (syntaxTreeNode.value.getFunctionValue(function, functionScriptIdx) == false ||
						(functionScriptIdx = getFunctionScriptIdx(function)) == SCRIPTIDX_NONE) {
						//
						_Console::printLine(
							getStatementInformation(statement) +
							": Function not found: " +
							syntaxTreeNode.value.getValueAsString()
						);
						//
						parseErrors.push_back(
							getStatementInformation(statement) +
							": Function not found: " +
							syntaxTreeNode.value.getValueAsString()
						);
						//
						return false;
					}
					//
					syntaxTreeNode.value.setFunctionAssignment(function, functionScriptIdx);
				} else
				if (syntaxTreeNode.value.getType() == MiniScript::TYPE_STACKLET_ASSIGNMENT) {
					string stacklet;
					auto stackletScriptIdx = SCRIPTIDX_NONE;
					if (syntaxTreeNode.value.getStackletValue(stacklet, stackletScriptIdx) == false ||
						(stackletScriptIdx = getFunctionScriptIdx(stacklet)) == SCRIPTIDX_NONE) {
						//
						_Console::printLine(
							getStatementInformation(statement) +
							": Stacklet not found" +
							syntaxTreeNode.value.getValueAsString()
						);
						//
						parseErrors.push_back(
							getStatementInformation(statement) +
							": Stacklet not found: " +
							syntaxTreeNode.value.getValueAsString()
						);
						//
						return false;
					}
					//
					syntaxTreeNode.value.setStackletAssignment(stacklet, stackletScriptIdx);
				}
				//
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD:
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:
			{
				for (auto& argument: syntaxTreeNode.arguments) {
					if (setupFunctionAndStackletScriptIndices(argument, statement) == false) return false;
				}
				//
				break;
			}
		default:
			break;
	}
	//
	return true;
}

int MiniScript::getStackletScopeScriptIdx(int scriptIdx) {
	if (scriptIdx < 0 || scriptIdx >= scripts.size() ||
		scripts[scriptIdx].type != MiniScript::Script::TYPE_STACKLET) {
		_Console::printLine("MiniScript::getStackletScopeScriptIdx(): No stacklet: " + to_string(scriptIdx));
		return MiniScript::SCRIPTIDX_NONE;
	}
	//
	const auto& stackletScript = scripts[scriptIdx];
	const auto& stackletScopeName = stackletScript.arguments.size() == 1?stackletScript.arguments[0].name:string();
	if (stackletScopeName.empty() == true) {
		return MiniScript::SCRIPTIDX_NONE;
	}
	//
	for (auto i = 0; i < scripts.size(); i++) {
		if (i == scriptIdx) continue;
		const auto& scriptCandidate = scripts[i];
		if (scriptCandidate.type != MiniScript::Script::TYPE_FUNCTION && scriptCandidate.type != MiniScript::Script::TYPE_STACKLET) continue;
		if (scriptCandidate.condition == stackletScopeName) {
			if (scriptCandidate.type == MiniScript::Script::TYPE_STACKLET) return getStackletScopeScriptIdx(i); else return i;
		}
	}
	//
	return MiniScript::SCRIPTIDX_NONE;
}

bool MiniScript::validateStacklets(int scriptIdx) {
	//
	const auto& script = scripts[scriptIdx];
	auto statementIdx = STATEMENTIDX_FIRST;
	//
	for (const auto& syntaxTreeNode: script.syntaxTree) {
		const auto& statement = script.statements[statementIdx++];
		//
		if (validateStacklets(script.type == Script::TYPE_FUNCTION?scriptIdx:SCRIPTIDX_NONE, syntaxTreeNode, statement) == false) {
			//
			return false;
		}
	}
	//
	return true;
}

bool MiniScript::validateStacklets(const string& function, int scopeScriptIdx) {
	auto functionScriptIdx = getFunctionScriptIdx(function);
	if (functionScriptIdx == SCRIPTIDX_NONE) {
		_Console::printLine("MiniScript::validateStacklet(): function not found: " + function);
		return false;
	}
	//
	const auto& script = scripts[functionScriptIdx];
	auto statementIdx = STATEMENTIDX_FIRST;
	//
	for (const auto& syntaxTreeNode: script.syntaxTree) {
		const auto& statement = script.statements[statementIdx++];
		//
		if (validateStacklets(scopeScriptIdx == MiniScript::SCRIPTIDX_NONE?functionScriptIdx:scopeScriptIdx, syntaxTreeNode, statement) == false) {
			//
			return false;
		}
	}
	//
	return true;
}

bool MiniScript::validateStacklets(int scopeScriptIdx, const SyntaxTreeNode& syntaxTreeNode, const Statement& statement) {
	switch (syntaxTreeNode.type) {
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
			{
				// TODO: improve me! This is actually litaral only, which can be also set as variable and be reused later
				//	basically we forbid here to create a stacklet assignment variable with wrong scope in a given scope
				if (syntaxTreeNode.value.getType() == MiniScript::TYPE_STACKLET_ASSIGNMENT) {
					// we only allow assignments of stacklets with a correct scope, means
					string stackletName;
					auto stackletScriptIdx = SCRIPTIDX_NONE;
					if (syntaxTreeNode.value.getStackletValue(stackletName, stackletScriptIdx) == false ||
						(stackletScriptIdx = getFunctionScriptIdx(stackletName)) == SCRIPTIDX_NONE) {
						//
						_Console::printLine(
							getStatementInformation(statement) +
							": " +
							syntaxTreeNode.value.getValueAsString() +
							": Stacklet not found"
						);
						//
						parseErrors.push_back(
							getStatementInformation(statement) +
							": " +
							syntaxTreeNode.value.getValueAsString() +
							": Stacklet not found"
						);
						//
						return false;
					}
					//
					int stackletScopeScriptIdx = getStackletScopeScriptIdx(stackletScriptIdx);
					if (stackletScopeScriptIdx != scopeScriptIdx) {
						// construct scope error
						string scopeErrorMessage;
						if (stackletScopeScriptIdx == SCRIPTIDX_NONE) {
							scopeErrorMessage = "Stacklet requires root scope";
						} else {
							scopeErrorMessage = "Stacklet requires scope of " + scripts[stackletScopeScriptIdx].condition + "()";
						}
						scopeErrorMessage+= ", but has scope of ";
						if (scopeScriptIdx == SCRIPTIDX_NONE) {
							scopeErrorMessage+= "root scope";
						} else {
							scopeErrorMessage+= scripts[scopeScriptIdx].condition + "()";
						}
						//
						_Console::printLine(
							getStatementInformation(statement) +
							": " +
							syntaxTreeNode.value.getValueAsString() +
							": Stacklet scope invalid: " +
							scopeErrorMessage
						);
						//
						parseErrors.push_back(
							getStatementInformation(statement) +
							": " +
							syntaxTreeNode.value.getValueAsString() +
							": Stacklet scope invalid" +
							scopeErrorMessage
						);
						//
						return false;
					}
					// check stacklet itself for stacklet litarals
					if (validateStacklets(stackletName, scopeScriptIdx) == false) return false;
				}
				//
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD:
			{
				for (const auto& argument: syntaxTreeNode.arguments) {
					if (validateStacklets(scopeScriptIdx, argument, statement) == false) return false;
				}
				//
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:
			{
				for (const auto& argument: syntaxTreeNode.arguments) {
					if (validateStacklets(scopeScriptIdx, argument, statement) == false) return false;
				}
				//
				if (getFunctionScriptIdx(syntaxTreeNode.value.getValueAsString()) == scopeScriptIdx) {
					// recursion
				} else {
					validateStacklets(syntaxTreeNode.value.getValueAsString());
				}
				//
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_STACKLET:
			{
				//
				string stackletName = syntaxTreeNode.value.getValueAsString();
				auto stackletScriptIdx = syntaxTreeNode.getScriptIdx();
				if (stackletName.empty() == true || stackletScriptIdx == SCRIPTIDX_NONE) {
					//
					_Console::printLine(
						getStatementInformation(statement) +
						": " +
						syntaxTreeNode.value.getValueAsString() +
						": Stacklet not found"
					);
					//
					parseErrors.push_back(
						getStatementInformation(statement) +
						": " +
						syntaxTreeNode.value.getValueAsString() +
						": Stacklet not found"
					);
					//
					return false;
				}
				//
				int stackletScopeScriptIdx = getStackletScopeScriptIdx(stackletScriptIdx);
				if (stackletScopeScriptIdx != scopeScriptIdx) {
					// construct scope error
					string scopeErrorMessage;
					if (stackletScopeScriptIdx == SCRIPTIDX_NONE) {
						scopeErrorMessage = "Stacklet requires root scope";
					} else {
						scopeErrorMessage = "Stacklet requires scope of " + scripts[stackletScopeScriptIdx].condition + "()";
					}
					scopeErrorMessage+= ", but has scope of ";
					if (scopeScriptIdx == SCRIPTIDX_NONE) {
						scopeErrorMessage+= "root scope";
					} else {
						scopeErrorMessage+= scripts[scopeScriptIdx].condition + "()";
					}
					//
					_Console::printLine(
						getStatementInformation(statement) +
						": " +
						syntaxTreeNode.value.getValueAsString() +
						": Stacklet scope invalid: " +
						scopeErrorMessage
					);
					//
					parseErrors.push_back(
						getStatementInformation(statement) +
						": " +
						syntaxTreeNode.value.getValueAsString() +
						": Stacklet scope invalid" +
						scopeErrorMessage
					);
					//
					return false;
				}
				//
				validateStacklets(syntaxTreeNode.value.getValueAsString(), scopeScriptIdx);
				//
				break;
			}
		default:
			break;
	}
	//
	return true;
}

bool MiniScript::validateCallable(const string& function) {
	auto functionScriptIdx = getFunctionScriptIdx(function);
	if (functionScriptIdx == SCRIPTIDX_NONE) {
		_Console::printLine("MiniScript::validateCallable(): function not found: " + function);
		return false;
	}
	//
	const auto& script = scripts[functionScriptIdx];
	auto statementIdx = STATEMENTIDX_FIRST;
	//
	for (const auto& syntaxTreeNode: script.syntaxTree) {
		const auto& statement = script.statements[statementIdx++];
		//
		if (validateCallable(syntaxTreeNode, statement) == false) {
			//
			return false;
		}
	}
	//
	return true;
}

bool MiniScript::validateCallable(const SyntaxTreeNode& syntaxTreeNode, const Statement& statement) {
	//
	switch (syntaxTreeNode.type) {
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
			{
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD:
			{
				const auto& contextFunctions = syntaxTreeNode.getMethod()->getContextFunctions();
				if (contextFunctions.empty() == false) {
					//
					_Console::printLine(
						getStatementInformation(statement) +
						": " +
						syntaxTreeNode.getMethod()->getMethodName() + "() can not be called within a callable function"
					);
					//
					parseErrors.push_back(
						getStatementInformation(statement) +
						": " +
						syntaxTreeNode.getMethod()->getMethodName() + "() can not be called within a callable function"
					);
					//
					return false;
				}
			}
			break;
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:
			{
				for (const auto& argument: syntaxTreeNode.arguments) {
					if (validateCallable(argument, statement) == false) return false;
				}
				//
				validateCallable(syntaxTreeNode.value.getValueAsString());
				//
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_STACKLET:
			{
				validateCallable(syntaxTreeNode.value.getValueAsString());
				//
				break;
			}
		default:
			break;
	}
	//
	return true;
}

bool MiniScript::validateContextFunctions(const string& function, vector<string>& functionStack) {
	auto functionScriptIdx = getFunctionScriptIdx(function);
	if (functionScriptIdx == SCRIPTIDX_NONE) {
		_Console::printLine("MiniScript::validateContextFunctions(): Function not found: " + function);
		return false;
	}
	//
	const auto& script = scripts[functionScriptIdx];
	auto statementIdx = STATEMENTIDX_FIRST;
	//
	functionStack.push_back(script.condition);
	//
	for (const auto& syntaxTreeNode: script.syntaxTree) {
		const auto& statement = script.statements[statementIdx++];
		//
		if (validateContextFunctions(syntaxTreeNode, functionStack, statement) == false) {
			//
			return false;
		}
	}
	//
	functionStack.erase(functionStack.begin() + functionStack.size() - 1);
	//
	return true;
}

bool MiniScript::validateContextFunctions(const SyntaxTreeNode& syntaxTreeNode, vector<string>& functionStack, const Statement& statement) {
	//
	switch (syntaxTreeNode.type) {
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
			{
				break;
			}
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD:
			{
				const auto& contextFunctions = syntaxTreeNode.getMethod()->getContextFunctions();
				if (contextFunctions.empty() == false) {
					//
					string contextFunctionsString;
					for (const auto &contextFunction: contextFunctions) {
						if (contextFunctionsString.empty() == false) contextFunctionsString+= ", ";
						contextFunctionsString+= contextFunction + "()";
					}
					//
					const auto& functionStackFunction = functionStack[0];
					if (find(contextFunctions.begin(), contextFunctions.end(), functionStackFunction) == contextFunctions.end()) {
						//
						string contextFunctionsString;
						for (const auto &contextFunction: contextFunctions) {
							if (contextFunctionsString.empty() == false) contextFunctionsString+= ", ";
							contextFunctionsString+= contextFunction + "()";
						}
						//
						_Console::printLine(
							getStatementInformation(statement) +
							": " +
							syntaxTreeNode.getMethod()->getMethodName() + "() can only be called within the following functions: " +
							contextFunctionsString +
							", but was called from " +
							functionStackFunction + "()"
						);
						//
						parseErrors.push_back(
							getStatementInformation(statement) +
							": " +
							syntaxTreeNode.getMethod()->getMethodName() + "() can only be called within the following functions: " +
							contextFunctionsString +
							", but was called from " +
							functionStackFunction + "()"
						);
						//
						return false;
					}
				}
			}
			break;
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_STACKLET:
			{
				for (const auto& argument: syntaxTreeNode.arguments) {
					if (validateContextFunctions(argument, functionStack, statement) == false) return false;
				}
				//
				validateContextFunctions(syntaxTreeNode.value.getValueAsString(), functionStack);
				//
				break;
			}
		default:
			break;
	}
	//
	return true;
}

void MiniScript::emit(const string& condition) {
	if (VERBOSE == true) _Console::printLine("MiniScript::emit(): " + scriptFileName + ": " + condition);
	auto scriptIdxToStart = 0;
	for (const auto& script: scripts) {
		auto conditionMet = true;
		if (script.name.empty() == false && script.name == condition) {
			break;
		} else
		if (script.condition == condition) {
			break;
		} else {
			scriptIdxToStart++;
		}
	}
	if (scriptIdxToStart == scripts.size()) {
		scriptIdxToStart = SCRIPTIDX_NONE;
		startErrorScript();
		return;
	}
	//
	getScriptState().running = true;
	resetScriptExecutationState(scriptIdxToStart, STATEMACHINESTATE_NEXT_STATEMENT);
}

void MiniScript::executeStateMachine() {
	while (true == true) {
		{
			auto& scriptState = getScriptState();
			// determine state machine state if it did change
			{
				if (scriptState.lastStateMachineState == nullptr || scriptState.state != scriptState.lastState) {
					scriptState.lastState = scriptState.state;
					scriptState.lastStateMachineState = nullptr;
					auto stateMachineStateIt = stateMachineStates.find(scriptState.state);
					if (stateMachineStateIt != stateMachineStates.end()) {
						scriptState.lastStateMachineState = stateMachineStateIt->second;
					}
				}
			}

			// execute state machine
			if (scriptState.lastStateMachineState != nullptr) {
				if (native == true && scriptState.state == STATEMACHINESTATE_NEXT_STATEMENT) {
					// ignore STATEMACHINESTATE_NEXT_STATEMENT on native
				} else {
					scriptState.lastStateMachineState->execute();
				}
			} else {
				// we can ignore this here and break as our state machine is unset
				break;
			}
		}

		// native
		//	also do not run enabled conditions when beeing in (user script) function
		if (native == true && isFunctionRunning() == false) {
			const auto& scriptState = getScriptState();
			// check named conditions
			auto now = _Time::getCurrentMillis();
			if (enabledNamedConditions.empty() == false &&
				(timeEnabledConditionsCheckLast == TIME_NONE || now >= timeEnabledConditionsCheckLast + 100LL)) {
				auto scriptIdxToStart = determineNamedScriptIdxToStart();
				if (scriptIdxToStart != SCRIPTIDX_NONE && scriptIdxToStart != scriptState.scriptIdx) {
					//
					resetScriptExecutationState(scriptIdxToStart, STATEMACHINESTATE_NEXT_STATEMENT);
				}
				timeEnabledConditionsCheckLast = now;
			}
			// stop here
			break;
		} else {
			// break if no next statement but other state machine state or not running
			const auto& scriptState = getScriptState();
			if (scriptState.state != STATEMACHINESTATE_NEXT_STATEMENT || scriptState.running == false) break;
		}
	}
}

void MiniScript::execute() {
	const auto& scriptState = getScriptState();

	//
	if (scriptState.running == false || scriptState.state == STATEMACHINESTATE_NONE) return;

	// check named conditions
	auto now = _Time::getCurrentMillis();
	if (isFunctionRunning() == false &&
		enabledNamedConditions.empty() == false &&
		(timeEnabledConditionsCheckLast == TIME_NONE || now >= timeEnabledConditionsCheckLast + 100LL)) {
		auto scriptIdxToStart = determineNamedScriptIdxToStart();
		if (scriptIdxToStart != SCRIPTIDX_NONE && scriptIdxToStart != scriptState.scriptIdx) {
			//
			resetScriptExecutationState(scriptIdxToStart, STATEMACHINESTATE_NEXT_STATEMENT);
		}
		timeEnabledConditionsCheckLast = now;
	}

	// execute while having statements to be processed
	executeStateMachine();

	// try garbage collection
	tryGarbageCollection();
}

const string MiniScript::getNextStatement(const string& scriptCode, int& i, int& line) {
	string statementCode;
	vector<string> statementCodeLines;
	statementCodeLines.emplace_back();
	auto quote = '\0';
	auto expectBracket = false;
	auto canExpectStacklet = false;
	auto inlineFunctionArguments = false;
	auto bracketCount = 0;
	auto squareBracketCount = 0;
	auto curlyBracketCount = 0;
	auto hash = false;
	auto lc = '\0';
	for (; i < scriptCode.size(); i++) {
		auto c = scriptCode[i];
		auto nc = i + 1 < scriptCode.size()?scriptCode[i + 1]:'\0';
		// this is some sort of hack, but it works, we need a more sophisticated parser later
		if (c != '-' && c != '>' &&
			c != ' ' && c != '\t' && c != '\n' && c != '\r') canExpectStacklet = c == ',' || c == '(';
		// handle quotes
		if ((c == '"' || c == '\'') && lc != '\\') {
			if (quote == '\0') {
				quote = c;
			} else
			if (quote == c) {
				quote = '\0';
			}
			// add char to script line
			statementCodeLines.back() += c;
			//
			inlineFunctionArguments = false;
		} else
		if (quote != '\0') {
			// no op
			statementCodeLines.back() += c;
		} else
		// brackets
		if (c == '(') {
			inlineFunctionArguments = false;
			//
			bracketCount++;
			expectBracket = false;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (c == ')') {
			//
			inlineFunctionArguments = false;
			//
			bracketCount--;
			// add char to script line
			statementCodeLines.back() += c;
			//
			inlineFunctionArguments = true;
		} else
		// square brackets
		if (c == '[') {
			//
			inlineFunctionArguments = false;
			//
			squareBracketCount++;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (c == ']') {
			//
			inlineFunctionArguments = false;
			//
			squareBracketCount--;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		// curly brackets
		if (c == '{') {
			//
			inlineFunctionArguments = false;
			//
			curlyBracketCount++;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (c == '}') {
			//
			inlineFunctionArguments = false;
			//
			curlyBracketCount--;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		// hash
		if (c == '#') {
			// hash
			hash = true;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		// new line
		if (c == '\r') {
			// ignore
		} else
		if (lc == '-' && c == '>') {
			// we expect a bracket now for object->xyz() member method call, if we have a possible identifier
			if (inlineFunctionArguments == false && canExpectStacklet == false) expectBracket = true;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if ((c == '\n' && ++line) || (hash == false && c == ';')) {
			// break here and process script line
			if (expectBracket == false && bracketCount == 0 && squareBracketCount == 0 && curlyBracketCount == 0) break;
			// unset hash after newline
			if (c == '\n') {
				//
				hash = false;
				//
				statementCodeLines.emplace_back();
			} else {
				statementCodeLines.back() += c;
				//
				inlineFunctionArguments = false;
			}
		} else {
			//
			if (_Character::isSpace(c) == false && c != '-' && nc != '>') inlineFunctionArguments = false;
			// add char to script line
			statementCodeLines.back() += c;
		}
		//
		lc = c;
	}

	//
	auto lineIdx = 0;
	for (const auto& line: statementCodeLines) {
		auto trimmedLine = _StringTools::trim(line);
		if (trimmedLine.empty() == true || _StringTools::startsWith(trimmedLine, "#") == true) continue;
		statementCode+= trimmedLine;
		if (lineIdx != statementCodeLines.size() - 1) statementCode+= "\n";
		lineIdx++;
	}

	// add last line index
	if (i == scriptCode.size() && scriptCode.back() != '\n') ++line;

	//
	return statementCode;
}

bool MiniScript::parseScriptInternal(const string& scriptCode) {
	//
	auto scriptCount = scripts.size();
	auto haveScript = false;
	auto lineIdx = LINE_FIRST;
	auto currentLineIdx = 0;
	auto statementIdx = STATEMENTIDX_FIRST;
	struct Block {
		enum Type { TYPE_FOR, TYPE_FOREACH, TYPE_IF, TYPE_ELSE, TYPE_ELSEIF, TYPE_SWITCH, TYPE_CASE, TYPE_DEFAULT };
		Block(Type type, int statementIdx): type(type), statementIdx(statementIdx) {}
		Type type;
		int statementIdx;
	};
	vector<Block> blockStack;
	//
	for (auto i = 0; i < scriptCode.size(); i++) {
		//
		currentLineIdx = lineIdx;

		// try to get next statement code
		auto statementCode = getNextStatement(scriptCode, i, lineIdx);

		// add last line index
		if (i == scriptCode.size() && scriptCode.back() != '\n') ++lineIdx;
		//
		if (statementCode.empty() == true) {
			continue;
		}

		// no script yet
		if (haveScript == false) {
			// check if we have to read additional info from code
			if (statementCode == "function:" ||
				statementCode == "stacklet:" ||
				statementCode == "on:" ||
				statementCode == "on-enabled:" ||
				statementCode == "callable:") {
				//
				i++;
				// we need the condition or name
				for (; i < scriptCode.size(); i++) {
					auto nextStatementCode = getNextStatement(scriptCode, i, lineIdx);
					if (nextStatementCode.empty() == false) {
						statementCode+= " " + nextStatementCode;
						break;
					}
				}
			}
			// check if we need to parse ":= name"
			//	applies to on: and on-enabled only
			if (_StringTools::startsWith(statementCode, "on:") == true ||
				_StringTools::startsWith(statementCode, "on-enabled:") == true) {
				//
				if (statementCode.rfind(":=") == string::npos) {
					//
					auto gotName = false;
					//
					auto _i = i;
					auto _lineIdx = lineIdx;
					auto _statementCode = statementCode;
					//
					auto endStack = 0;
					//
					i++;
					//
					for (; i < scriptCode.size(); i++) {
						auto nextStatementCode = getNextStatement(scriptCode, i, lineIdx);
						if (nextStatementCode.empty() == false) {
							//
							if (_StringTools::startsWith(nextStatementCode, "function:") == true ||
								_StringTools::startsWith(nextStatementCode, "on:") == true ||
								_StringTools::startsWith(nextStatementCode, "on-enabled:") == true ||
								_StringTools::startsWith(nextStatementCode, "callable:") == true) break;
							//
							statementCode+= " " + nextStatementCode;
							// break here if we got our := or reached next declaration
							auto lc = '\0';
							auto quote = '\0';
							for (auto j = 0; j < statementCode.size(); j++) {
								auto c = statementCode[j];
								// handle quotes
								if ((c == '"' || c == '\'') && lc != '\\') {
									if (quote == '\0') {
										quote = c;
									} else
									if (quote == c) {
										quote = '\0';
									}
								} else
								if (quote != '\0') {
									// no op
								} else
								if (lc == ':' && c == '=') {
									gotName = true;
									//
									break;
								}
								//
								lc = c;
							}
							//
							if (gotName == true) break;
						}
					}
					// did we got our ":= name", nope?
					if (gotName == false) {
						// reset
						i = _i;
						lineIdx = _lineIdx;
						statementCode = _statementCode;
					}
				}
				// we still need the name
				if (_StringTools::endsWith(statementCode, ":=") == true) {
					//
					i++;
					//
					for (; i < scriptCode.size(); i++) {
						auto nextStatementCode = getNextStatement(scriptCode, i, lineIdx);
						if (nextStatementCode.empty() == false) {
							statementCode+= " " + nextStatementCode;
							break;
						}
					}
				}
			}
			// script type
			auto callable = false;
			auto scriptType = Script::TYPE_NONE;
			if (_StringTools::startsWith(statementCode, "function:") == true) scriptType = Script::TYPE_FUNCTION; else
			if (_StringTools::startsWith(statementCode, "stacklet:") == true) scriptType = Script::TYPE_STACKLET; else
			if (_StringTools::startsWith(statementCode, "on:") == true) scriptType = Script::TYPE_ON; else
			if (_StringTools::startsWith(statementCode, "on-enabled:") == true) scriptType = Script::TYPE_ONENABLED; else
			if (_StringTools::startsWith(statementCode, "callable:") == true) {
				callable = true;
				scriptType = Script::TYPE_FUNCTION;
			}
			// no, but did we got a new script?
			if (scriptType != Script::TYPE_NONE) {
				// yes
				haveScript = true;
				// functions: argument names
				vector<Script::Argument> arguments;
				// determine statement
				string statement;
				if (scriptType == Script::TYPE_FUNCTION) {
					statement = callable == true?
						_StringTools::trim(_StringTools::substring(statementCode, string("callable:").size())):
						_StringTools::trim(_StringTools::substring(statementCode, string("function:").size()));
				} else
				if (scriptType == Script::TYPE_STACKLET) {
					statement = _StringTools::trim(_StringTools::substring(statementCode, string("stacklet:").size()));
				} else
				if (scriptType == Script::TYPE_ON)
					statement = _StringTools::trim(_StringTools::substring(statementCode, string("on:").size())); else
				if (scriptType == Script::TYPE_ONENABLED)
					statement = _StringTools::trim(_StringTools::substring(statementCode, string("on-enabled:").size()));
				// and name
				string name;
				auto scriptLineNameSeparatorIdx =
					scriptType == Script::TYPE_FUNCTION?
						statement.rfind("function:"):
						(scriptType == Script::TYPE_STACKLET?
							statement.rfind("stacklet:"):
							statement.rfind(":=")
						);
				if (scriptLineNameSeparatorIdx != string::npos) {
					name =
						_StringTools::trim(
							_StringTools::substring(
								statement,
								scriptLineNameSeparatorIdx +
									(scriptType == Script::TYPE_FUNCTION?string("function").size():(scriptType == Script::TYPE_STACKLET?string("stacklet").size():string(":=").size()))
							)
						);
					statement = _StringTools::trim(_StringTools::substring(statement, 0, scriptLineNameSeparatorIdx));
				}
				if (scriptType == Script::TYPE_FUNCTION ||
					scriptType == Script::TYPE_STACKLET) {
					auto leftBracketIdx = statement.find('(');
					auto rightBracketIdx = statement.find(')');
					if (leftBracketIdx != string::npos || leftBracketIdx != string::npos) {
						if (leftBracketIdx == string::npos) {
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": unbalanced bracket count");
							//
							parseErrors.push_back(to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": unbalanced bracket count");
							//
							scriptValid = false;
						} else
						if (rightBracketIdx == string::npos) {
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": unbalanced bracket count");
							//
							parseErrors.push_back(to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": unbalanced bracket count");
							//
							scriptValid = false;
						} else {
							auto argumentNamesString = _StringTools::trim(_StringTools::substring(statement, leftBracketIdx + 1, rightBracketIdx));
							auto argumentNamesTokenized = _StringTools::tokenize(argumentNamesString, ",");
							statement = _StringTools::substring(statement, 0, leftBracketIdx);
							for (const auto& argumentName: argumentNamesTokenized) {
								auto argumentNameTrimmed = _StringTools::trim(argumentName);
								auto reference = false;
								if (_StringTools::startsWith(argumentNameTrimmed, "&") == true) {
									reference = true;
									argumentNameTrimmed = _StringTools::trim(_StringTools::substring(argumentNameTrimmed, 1));
								}
								if (scriptType == Script::TYPE_FUNCTION) {
									if (_StringTools::regexMatch(argumentNameTrimmed, "\\$[a-zA-Z0-9_]+") == true) {
										arguments.emplace_back(
											argumentNameTrimmed,
											reference
										);
									} else {
										_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": invalid argument name: '" + argumentNameTrimmed + "'");
										//
										parseErrors.push_back(to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": invalid argument name: '" + argumentNameTrimmed + "'");
										//
										scriptValid = false;
									}
								} else
								if (scriptType == Script::TYPE_STACKLET) {
									if (_StringTools::regexMatch(argumentNameTrimmed, "[a-zA-Z0-9_]+") == true) {
										arguments.emplace_back(
											argumentNameTrimmed,
											reference
										);
									} else {
										_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": invalid stacklet parent stacklet/function: '" + argumentNameTrimmed + "'");
										//
										parseErrors.push_back(to_string(currentLineIdx) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": invalid stacklet parent stacklet/function: '" + argumentNameTrimmed + "'");
										//
										scriptValid = false;
									}
								}
							}
						}

					}
				}
				auto trimmedStatement = _StringTools::trim(statement);
				Statement evaluateStatement(
					currentLineIdx,
					STATEMENTIDX_FIRST,
					"internal.script.evaluate(" + _StringTools::replace(_StringTools::replace(trimmedStatement, "\\", "\\\\"), "\"", "\\\"") + ")",
					"internal.script.evaluate(" + _StringTools::replace(_StringTools::replace(trimmedStatement, "\\", "\\\\"), "\"", "\\\"") + ")",
					STATEMENTIDX_NONE
				);
				auto conditionOrNameExecutable = doStatementPreProcessing(trimmedStatement, evaluateStatement);
				auto conditionOrName = trimmedStatement;
				auto emitCondition = _StringTools::regexMatch(conditionOrName, "[a-zA-Z0-9_]+");
				statementIdx = STATEMENTIDX_FIRST;
				// add to user functions
				if (scriptType == Script::TYPE_FUNCTION || scriptType == Script::TYPE_STACKLET) {
					functions[conditionOrName] = scripts.size();
				}

				// push to scripts
				scripts.emplace_back(
					scriptType,
					currentLineIdx,
					conditionOrName,
					conditionOrNameExecutable,
					Statement(currentLineIdx, statementIdx, conditionOrName, conditionOrNameExecutable, STATEMENTIDX_NONE),
					SyntaxTreeNode(),
					name,
					emitCondition,
					initializer_list<Statement>{},
					initializer_list<SyntaxTreeNode>{},
					callable,
					arguments
				);
			} else {
				_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": expecting 'on:', 'on-enabled:', 'stacklet:', 'function:', 'callable:'");
				//
				parseErrors.push_back(to_string(currentLineIdx) + ": expecting 'on:', 'on-enabled:', 'stacklet:', 'function:', 'callable:'");
				//
				scriptValid = false;
			}
		} else {
			if (_StringTools::startsWith(statementCode, "function:") == true ||
				_StringTools::startsWith(statementCode, "stacklet:") == true ||
				_StringTools::startsWith(statementCode, "on:") == true ||
				_StringTools::startsWith(statementCode, "on-enabled:") == true ||
				_StringTools::startsWith(statementCode, "callable:") == true
			) {
				_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
				//
				parseErrors.push_back(to_string(currentLineIdx) + ": unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
				//
				scriptValid = false;
			} else
			if (statementCode == "end") {
				if (blockStack.empty() == false) {
					auto block = blockStack.back();
					blockStack.erase(blockStack.begin() + blockStack.size() - 1);
					switch(block.type) {
						case Block::TYPE_FOR:
						case Block::TYPE_FOREACH:
							{
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, block.statementIdx);
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
							}
							break;
						case Block::TYPE_IF:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_ELSE:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_ELSEIF:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_SWITCH:
							{
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_CASE:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size() + 1;
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_DEFAULT:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size() + 1;
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
					}
				} else{
					scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
					haveScript = false;
				}
			} else
			if (statementCode == "else") {
				if (blockStack.empty() == false) {
					auto block = blockStack.back();
					blockStack.erase(blockStack.begin() + blockStack.size() - 1);
					switch(block.type) {
						case Block::TYPE_IF:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_ELSEIF:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
							}
							break;
						default:
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": else without if/elseif");
							//
							parseErrors.push_back(to_string(currentLineIdx) + ": else without if/elseif");
							//
							scriptValid = false;
							break;
					}
					blockStack.emplace_back(
						Block::TYPE_ELSE,
						statementIdx
					);
				} else {
					_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": else without if");
					//
					parseErrors.push_back(to_string(currentLineIdx) + ": else without if");
					//
					scriptValid = false;
				}
			} else
			if (_StringTools::regexMatch(statementCode, "^elseif[\\s]*\\(.*\\)$") == true) {
				Statement elseIfStatement(
					currentLineIdx,
					STATEMENTIDX_FIRST,
					_StringTools::replace(_StringTools::replace(statementCode, "\\", "\\\\"), "\"", "\\\""),
					_StringTools::replace(_StringTools::replace(statementCode, "\\", "\\\\"), "\"", "\\\""),
					STATEMENTIDX_NONE
				);
				auto executableStatement = doStatementPreProcessing(statementCode, elseIfStatement);
				if (blockStack.empty() == false) {
					auto block = blockStack.back();
					blockStack.erase(blockStack.begin() + blockStack.size() - 1);
					switch(block.type) {
						case Block::TYPE_IF:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, executableStatement, STATEMENTIDX_NONE);
							}
							break;
						case Block::TYPE_ELSEIF:
							{
								scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								scripts.back().statements.emplace_back(currentLineIdx, statementIdx, statementCode, executableStatement, STATEMENTIDX_NONE);
							}
							break;
						default:
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": elseif without if");
							scriptValid = false;
							break;
					}
					blockStack.emplace_back(
						Block::TYPE_ELSEIF,
						statementIdx
					);
				} else {
					_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": elseif without if");
					//
					parseErrors.push_back(to_string(currentLineIdx) + ": elseif without if");
					//
					scriptValid = false;
				}
			} else {
				smatch matches;
				Statement generatedStatement(
					currentLineIdx,
					STATEMENTIDX_FIRST,
					_StringTools::replace(_StringTools::replace(statementCode, "\\", "\\\\"), "\"", "\\\""),
					_StringTools::replace(_StringTools::replace(statementCode, "\\", "\\\\"), "\"", "\\\""),
					STATEMENTIDX_NONE
				);
				if (_StringTools::regexMatch(statementCode, "^for[\\s]*\\(.*\\)$") == true) {
					// parse for statement
					string_view forMethodName;
					vector<string_view> forArguments;
					string accessObjectMemberStatement;
					string executableStatement = doStatementPreProcessing(statementCode, generatedStatement);
					// success?
					if (parseStatement(executableStatement, forMethodName, forArguments, generatedStatement, accessObjectMemberStatement) == true &&
						forArguments.size() == 3) {
						// create initialize statement
						string initializeStatement = string(forArguments[0]);
						scripts.back().statements.emplace_back(currentLineIdx, statementIdx++, statementCode, initializeStatement, STATEMENTIDX_NONE);
						//
						blockStack.emplace_back(
							Block::TYPE_FOR,
							statementIdx
						);
						//
						statementCode = "forCondition(" + string(forArguments[1]) + ", -> { " + string(forArguments[2]) + " })";
					} else {
						_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": Invalid for statement");
						//
						parseErrors.push_back(to_string(currentLineIdx) + ": Invalid for statement");
						//
						scriptValid = false;
						//
						return false;
					}
				} else
				// array/set forEach
				if (_StringTools::regexMatch(statementCode, "^forEach[\\s]*\\([\\s]*(&?{0,1}\\$[a-zA-Z0-9_]+)[\\s]*\\:[\\s]*((\\$[a-zA-Z0-9_]+)|(\\[.*\\])|(\\{.*\\}))[\\s]*\\)$", &matches) == true) {
					auto iterationDepth = 0;
					for (const auto& block: blockStack) {
						if (block.type == Block::TYPE_FOREACH) iterationDepth++;
					}
					//
					auto entryReference = false;
					auto entryVariable = matches[1].str();
					if (_StringTools::startsWith(entryVariable, "&") == true) {
						entryReference = true;
						entryVariable = _StringTools::substring(entryVariable, 1);
					}
					auto containerByInitializer = false;
					auto containerVariable = matches[2].str();
					string containerInitializer;
					if (_StringTools::startsWith(containerVariable, "[") == true || _StringTools::startsWith(containerVariable, "{") == true) {
						containerByInitializer = true;
						containerInitializer = containerVariable;
						containerVariable = string("$___cv_" + to_string(iterationDepth));
					}
					auto initializationStackletVariable = string("$___is_" + to_string(iterationDepth));
					auto containerVariableType = string("$___vt_" + to_string(iterationDepth));
					auto iterationVariable = string("$___it_" + to_string(iterationDepth));
					auto entryVariableBackup = string("$___evb_" + to_string(iterationDepth));
					auto containerArrayVariable = string("$___cav_" + to_string(iterationDepth));
					auto containerArrayVariableBackup = string("$___cavb_" + to_string(iterationDepth));
					string iterationUpdate =
						entryReference == true?
							"setVariableReference(\"" + entryVariable + "\", " + containerArrayVariable + "[" + iterationVariable + "])":
							entryVariable + " = " + containerArrayVariable + "[" + iterationVariable + "]";
					//
					string initialization =
						initializationStackletVariable + " = -> { " +
						containerVariableType + " = getType(" + containerVariable + "); " +
						"if (" + containerVariableType + " == \"Array\"); " +
							"if (script.isNative() == true); " +
								"setVariableReference(\"" + containerArrayVariableBackup + "\", " + containerArrayVariable + "); " +
							"end; " +
							"setVariableReference(\"" + containerArrayVariable + "\", " + containerVariable + "); " +
						"elseif (" + containerVariableType + " == \"Set\"); " +
							containerArrayVariable + " = " + containerVariable + "->getKeys(); " +
						"else; " +
							"console.printLine(\"forEach() expects array or set as container, but got \" + String::toLowerCase(getType(" + containerVariable + "))); " +
							"script.emit(\"error\"); " +
						"end; ";
					// create initialize statements
					if (entryReference == true) {
						initialization+=
							string() +
							"if (script.isNative() == true); " +
							"setVariableReference(\"" + entryVariableBackup + "\", " + entryVariable + "); " +
							"end; ";
					}
					initialization+=
						iterationVariable + " = 0; " +
						iterationUpdate + "; " +
						"}";
					//
					if (containerByInitializer == true) {
						scripts.back().statements.emplace_back(
							currentLineIdx,
							statementIdx++,
							statementCode,
							doStatementPreProcessing(containerVariable + " = " + containerInitializer, generatedStatement),
							STATEMENTIDX_NONE
						);
					}
					scripts.back().statements.emplace_back(
						currentLineIdx,
						statementIdx++,
						statementCode,
						doStatementPreProcessing(initialization, generatedStatement),
						STATEMENTIDX_NONE
					);
					scripts.back().statements.emplace_back(
						currentLineIdx,
						statementIdx++,
						statementCode,
						"internal.script.callStacklet(" + initializationStackletVariable + ")",
						STATEMENTIDX_NONE
					);
					blockStack.emplace_back(
						Block::TYPE_FOREACH,
						statementIdx
					);
					//
					statementCode =
						"forCondition(" + iterationVariable + " < " + containerArrayVariable + "->getSize(), " +
						"-> { " +
						iterationVariable + "++" + "; " +
						"if (" + iterationVariable + " < " + containerArrayVariable + "->getSize()); " +
							iterationUpdate + "; " +
						"else; " +
						(entryReference == true?
							string() +
							"if (script.isNative() == true); " +
								"setVariableReference(\"" + containerArrayVariable + "\", " + containerArrayVariableBackup + "); " +
								"setVariableReference(\"" + entryVariable + "\", " + entryVariableBackup + "); " +
							"else; " +
								"setVariableReference(\"" + containerArrayVariable + "\", $___ARRAY); " +
								"setVariableReference(\"" + entryVariable + "\", $___NULL); " +
							"end; "
							:
							"setVariable(\"" + entryVariable + "\", $___NULL); "
						) +
						(containerByInitializer == true?"setVariable(\"" + containerVariable + "\", $___ARRAY); ":"") +
						"end; " +
						"})";
				} else
				// map forEach
				if (_StringTools::regexMatch(statementCode, "^forEach[\\s]*\\([\\s]*(\\$[a-zA-Z0-9_]+)[\\s]*,[\\s]*(&?{0,1}\\$[a-zA-Z0-9_]+)[\\s]*\\:[\\s]*((\\$[a-zA-Z0-9_]+)|(\\[.*\\])|(\\{.*\\}))[\\s]*\\)$", &matches) == true) {
					auto iterationDepth = 0;
					for (const auto& block: blockStack) {
						if (block.type == Block::TYPE_FOREACH) iterationDepth++;
					}
					//
					auto entryKeyVariable = matches[1].str();
					auto entryValueReference = false;
					auto entryValueVariable = matches[2].str();
					if (_StringTools::startsWith(entryValueVariable, "&") == true) {
						entryValueReference = true;
						entryValueVariable = _StringTools::substring(entryValueVariable, 1);
					}
					auto containerByInitializer = false;
					auto containerVariable = matches[3].str();
					string containerInitializer;
					if (_StringTools::startsWith(containerVariable, "[") == true || _StringTools::startsWith(containerVariable, "{") == true) {
						containerByInitializer = true;
						containerInitializer = containerVariable;
						containerVariable = string("$___cv_" + to_string(iterationDepth));
					}
					auto initializationStackletVariable = string("$___is_" + to_string(iterationDepth));
					auto containerVariableType = string("$___vt_" + to_string(iterationDepth));
					auto iterationVariable = string("$___it_" + to_string(iterationDepth));
					auto entryValueVariableBackup = string("$___evb_" + to_string(iterationDepth));
					auto containerArrayVariable = string("$___cav_" + to_string(iterationDepth));
					auto containerArrayVariableBackup = string("$___cavb_" + to_string(iterationDepth));
					string iterationUpdate =
						entryKeyVariable + " = " + containerArrayVariable + "[" + iterationVariable + "]; " +
						(entryValueReference == true?
							"setVariableReference(\"" + entryValueVariable + "\", " + containerVariable + "->getReference(" + entryKeyVariable + "))":
							entryValueVariable + " = " + containerVariable + "->get(" + entryKeyVariable + ")"
						);
					//
					string initialization =
						initializationStackletVariable + " = -> { " +
						containerVariableType + " = getType(" + containerVariable + "); " +
						"if (" + containerVariableType + " == \"Map\"); " +
							containerArrayVariable + " = " + containerVariable + "->getKeys(); " +
						"else; " +
							"console.printLine(\"forEach() expects map as container, but got \" + String::toLowerCase(getType(" + containerVariable + "))); " +
							"script.emit(\"error\"); " +
						"end; ";
					// create initialize statements
					if (entryValueReference == true) {
						initialization+=
							string() +
							"if (script.isNative() == true); " +
							"setVariableReference(\"" + entryValueVariableBackup + "\", " + entryValueVariable + "); " +
							"end; ";
					}
					initialization+=
						iterationVariable + " = 0; " +
						iterationUpdate + "; " +
						"}";
					//
					if (containerByInitializer == true) {
						scripts.back().statements.emplace_back(
							currentLineIdx,
							statementIdx++,
							statementCode,
							doStatementPreProcessing(containerVariable + " = " + containerInitializer, generatedStatement),
							STATEMENTIDX_NONE
						);
						scripts.back().statements.emplace_back(
							currentLineIdx,
							statementIdx++,
							statementCode,
							doStatementPreProcessing("console.printLine('a: ' + " + containerVariable + ")", generatedStatement),
							STATEMENTIDX_NONE
						);
					}
					scripts.back().statements.emplace_back(
						currentLineIdx,
						statementIdx++,
						statementCode,
						doStatementPreProcessing(initialization, generatedStatement),
						STATEMENTIDX_NONE
					);
					scripts.back().statements.emplace_back(
						currentLineIdx,
						statementIdx++,
						statementCode,
						"internal.script.callStacklet(" + initializationStackletVariable + ")",
						STATEMENTIDX_NONE
					);
					blockStack.emplace_back(
						Block::TYPE_FOREACH,
						statementIdx
					);
					//
					statementCode =
						"forCondition(" + iterationVariable + " < " + containerArrayVariable + "->getSize(), " +
						"-> { " +
						iterationVariable + "++" + "; " +
						"if (" + iterationVariable + " < " + containerArrayVariable + "->getSize()); " +
							iterationUpdate + "; " +
						"else; " +
						(entryValueReference == true?
							string() +
							"if (script.isNative() == true); " +
								"setVariableReference(\"" + containerArrayVariable + "\", " + containerArrayVariableBackup + "); " +
								"setVariableReference(\"" + entryValueVariable + "\", " + entryValueVariableBackup + "); " +
							"else; " +
								"setVariableReference(\"" + containerArrayVariable + "\", $___ARRAY); " +
								"setVariableReference(\"" + entryValueVariable + "\", $___NULL); " +
							"end; " +
							"setVariable(\"" + entryKeyVariable + "\", $___NULL); "
							:
							"setVariable(\"" + entryValueVariable + "\", $___NULL); " +
							"setVariable(\"" + entryKeyVariable + "\", $___NULL); "
						) +
						(containerByInitializer == true?"setVariable(\"" + containerVariable + "\", $___ARRAY); ":"") +
						"end; " +
						"})";
				} else
				if (_StringTools::regexMatch(statementCode, "^forEach[\\s]*\\(.*\\)$", &matches) == true) {
					_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx) + ": Invalid forEach statement");
					//
					parseErrors.push_back(to_string(currentLineIdx) + ": Invalid forEach statement");
					//
					scriptValid = false;
					//
					return false;
				} else
				if (_StringTools::regexMatch(statementCode, "^forTime[\\s]*\\(.*\\)$") == true ||
					_StringTools::regexMatch(statementCode, "^forCondition[\\s]*\\(.*\\)$") == true) {
					blockStack.emplace_back(
						Block::TYPE_FOR,
						statementIdx
					);
				} else
				if (_StringTools::regexMatch(statementCode, "^if[\\s]*\\(.*\\)$") == true) {
					blockStack.emplace_back(
						Block::TYPE_IF,
						statementIdx
					);
				} else
				if (_StringTools::regexMatch(statementCode, "^switch[\\s]*\\(.*\\)$") == true) {
					blockStack.emplace_back(
						Block::TYPE_SWITCH,
						STATEMENTIDX_NONE
					);
				} else
				if (_StringTools::regexMatch(statementCode, "^case[\\s]*\\(.*\\)$") == true) {
					blockStack.emplace_back(
						Block::TYPE_CASE,
						statementIdx
					);
				} else
				if (_StringTools::regexMatch(statementCode, "^default[\\s]*$") == true) {
					blockStack.emplace_back(
						Block::TYPE_DEFAULT,
						statementIdx
					);
				}
				scripts.back().statements.emplace_back(
					currentLineIdx,
					statementIdx,
					statementCode,
					doStatementPreProcessing(statementCode, generatedStatement),
					STATEMENTIDX_NONE
				);
			}
			statementIdx++;
		}
	}

	// check for unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end
	if (scriptValid == true && blockStack.empty() == false) {
		_Console::printLine(scriptFileName + ": unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
		//
		parseErrors.push_back("Unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
		//
		scriptValid = false;
		//
		return false;
	}

	// create syntax tree
	for (auto scriptIdx = scriptCount; scriptIdx < scripts.size(); scriptIdx++) {
		auto& script = scripts[scriptIdx];
		// create syntax tree of executable condition if we have any
		if (script.emitCondition == false && script.executableCondition.empty() == false) {
			string_view method;
			vector<string_view> arguments;
			string accessObjectMemberStatement;
			if (parseStatement(script.executableCondition, method, arguments, script.conditionStatement, accessObjectMemberStatement) == false) {
				//
				scriptValid = false;
			} else
			if (createStatementSyntaxTree(SCRIPTIDX_NONE, method, arguments, script.conditionStatement, script.conditionSyntaxTree) == false) {
				//
				scriptValid = false;
			}
		}
		// create script syntax tree
		for (auto statementIdx = STATEMENTIDX_FIRST; statementIdx < script.statements.size(); statementIdx++) {
			const auto& statement = script.statements[statementIdx];
			script.syntaxTree.emplace_back();
			auto& syntaxTree = script.syntaxTree.back();
			string_view methodName;
			vector<string_view> arguments;
			string accessObjectMemberStatement;
			if (parseStatement(statement.executableStatement, methodName, arguments, statement, accessObjectMemberStatement) == false) {
				//
				scriptValid = false;
			} else
			if (createStatementSyntaxTree(scriptIdx, methodName, arguments, statement, syntaxTree) == false) {
				//
				scriptValid = false;
			}
		}
	}

	//
	return scriptValid;
}

void MiniScript::parseScript(const string& pathName, const string& fileName) {
	//
	scriptValid = true;
	scriptPathName = pathName;
	scriptFileName = fileName;

	//
	for (const auto& [methodName, method]: methods) delete method;
	for (const auto& [stateMachineStateId, stateMachineState]: stateMachineStates) delete stateMachineState;
	methods.clear();
	stateMachineStates.clear();
	while (scriptStateStack.empty() == false) popScriptState();

	// shutdown script state
	pushScriptState();
	resetScriptExecutationState(SCRIPTIDX_NONE, STATEMACHINESTATE_WAIT_FOR_CONDITION);

	//
	string scriptCode;
	try {
		scriptCode = _FileSystem::getContentAsString(scriptPathName, scriptFileName);
	} catch (_FileSystem::FileSystemException& fse)	{
		_Console::printLine(scriptPathName + "/" + scriptFileName + ": an error occurred: " + fse.what());
		//
		scriptValid = true;
		//
		parseErrors.push_back("An error occurred: " + string(fse.what()));
		//
		return;
	}

	//
	{
		auto scriptHash = _SHA256::encode(scriptCode);
		if (native == true) {
			if (scriptHash == nativeHash) {
				scripts = nativeScripts;
				registerStateMachineStates();
				registerMethods();
				registerVariables();
				startScript();
				return;
			} else {
				_Console::printLine(scriptPathName + "/" + scriptFileName + ": Script has changed. Script will be run in interpreted mode. Retranspile and recompile your script if you want to run it natively.");
				native = false;
			}
		}
		nativeHash = scriptHash;
	}

	//
	registerStateMachineStates();
	registerMethods();
	registerVariables();

	//
	if (parseScriptInternal(scriptCode) == false) return;

	// parse deferred function script codes,
	//	which are created by parsing map initializers and possible map inline functions
	do {
		auto deferredInlineScriptCodesCopy = deferredInlineScriptCodes;
		deferredInlineScriptCodes.clear();
		for (const auto& functionScriptCode: deferredInlineScriptCodesCopy) {
			parseScriptInternal(functionScriptCode);
		}
	} while (deferredInlineScriptCodes.empty() == false);
	// set up stacklet and function indices
	for (auto scriptIdx = 0; scriptIdx < scripts.size(); scriptIdx++) {
		//
		if (setupFunctionAndStackletScriptIndices(scriptIdx) == false) {
			//
			scriptValid = false;
			//
			break;
		}
	}
	// validations
	if (scriptValid == true) {
		for (auto scriptIdx = 0; scriptIdx < scripts.size(); scriptIdx++) {
			const auto& script = scripts[scriptIdx];
			//
			if (script.type == MiniScript::Script::TYPE_STACKLET) {
				// valid: root scope
				if (script.arguments.empty()) continue;
				// invalid: more than 1 argument
				if (script.arguments.size() != 1) {
					_Console::printLine(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: only none(for root scope) or one argument is allowed, which defines a function/stacklet as stacklet scope");
					parseErrors.push_back(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: only none(for root scope) or one argument is allowed, which defines a function/stacklet as stacklet scope");
					//
					scriptValid = false;
					//
					break;
				}
				//
				auto stackletScopeScriptIdx = getFunctionScriptIdx(script.arguments[0].name);
				// invalid: scope function/stacklet not found
				if (stackletScopeScriptIdx == SCRIPTIDX_NONE) {
					_Console::printLine(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet not found: " + script.arguments[0].name);
					parseErrors.push_back(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet not found: " + script.arguments[0].name);
					//
					scriptValid = false;
					//
					break;
				} else
				// invalid: stacklet can not have itself as scope stacklet
				if (stackletScopeScriptIdx == scriptIdx) {
					_Console::printLine(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet can not be the stacklet itself");
					parseErrors.push_back(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet can not be the stacklet itself");
					//
					scriptValid = false;
					//
					break;
				}
			}
			//
			if (script.type == MiniScript::Script::TYPE_FUNCTION ||
				script.type == MiniScript::Script::TYPE_ON ||
				script.type == MiniScript::Script::TYPE_ONENABLED) {
				//
				if (validateStacklets(scriptIdx) == false) {
					//
					scriptValid = false;
					//
					break;
				}
			}
			//
			if (script.type == MiniScript::Script::TYPE_FUNCTION) {
				//
				if (script.callable == true) {
					//
					if (validateCallable(script.condition) == false) {
						//
						scriptValid = false;
						//
						break;
					}
				} else {
					//
					vector<string> functionStack;
					//
					if (validateContextFunctions(script.condition, functionStack) == false) {
						//
						scriptValid = false;
						//
						break;
					}
				}
			}
		}
	}

	// check for initialize and error condition
	auto haveErrorScript = false;
	for (const auto& script: scripts) {
		if (script.type == Script::TYPE_ONENABLED) {
			// no op
		} else
		if (script.condition == "error") {
			haveErrorScript = true;
		}
	}
	if (haveErrorScript == false) {
		_Console::printLine(scriptPathName + "/" + scriptFileName + ": script needs to define an error condition");
		//
		parseErrors.push_back("Script needs to define an error condition");
		//
		scriptValid = false;
		//
		return;

	}

	//
	startScript();
}

void MiniScript::startScript() {
	if (VERBOSE == true) _Console::printLine("MiniScript::startScript(): '" + scriptFileName + ": starting script.");
	if (scriptValid == false) {
		_Console::printLine(scriptFileName + ": script not valid: not starting");
		return;
	}
	auto& scriptState = getScriptState();
	for (const auto& [variableName, variable]: scriptState.variables) delete variable;
	scriptState.variables.clear();
	scriptState.running = true;
	registerVariables();
	//
	if (hasCondition("initialze") == true) emit("initialize");
}

int MiniScript::determineScriptIdxToStart() {
	if (VERBOSE == true) _Console::printLine("MiniScript::determineScriptIdxToStart()");
	auto nothingScriptIdx = SCRIPTIDX_NONE;
	auto scriptIdx = 0;
	for (const auto& script: scripts) {
		if (script.type != Script::TYPE_ON) {
			// no op
		} else
		if (script.emitCondition == true && script.condition == "nothing") {
			nothingScriptIdx = scriptIdx;
			// no op
		} else
		if (script.emitCondition == true) {
			// emit condition
		} else {
			auto conditionMet = true;
			auto returnValue = Variable();
			if (evaluateInternal(script.condition, script.executableCondition, returnValue) == true) {
				auto returnValueBoolValue = false;
				if (returnValue.getBooleanValue(returnValueBoolValue, false) == false) {
					_Console::printLine("MiniScript::determineScriptIdxToStart(): " + script.condition + ": expecting boolean return value, but got: " + returnValue.getAsString());
					conditionMet = false;
				} else
				if (returnValueBoolValue == false) {
					conditionMet = false;
				}
			} else {
				_Console::printLine("MiniScript::determineScriptIdxToStart(): " + scriptFileName + ":" + to_string(script.line) + ": " + script.condition + ": parse error");
			}
			if (conditionMet == false) {
				if (VERBOSE == true) {
					_Console::print("MiniScript::determineScriptIdxToStart(): " + script.condition + ": FAILED");
				}	
			} else {
				if (VERBOSE == true) {
					_Console::print("MiniScript::determineScriptIdxToStart(): " + script.condition + ": OK");
				}
				return scriptIdx;
			}
		}
		scriptIdx++;
	}
	return nothingScriptIdx;
}

int MiniScript::determineNamedScriptIdxToStart() {
	if (VERBOSE == true) _Console::printLine("MiniScript::determineNamedScriptIdxToStart()");
	// TODO: we could have a hash map here to speed up enabledConditionName -> script lookup
	const auto& scriptState = getScriptState();
	for (const auto& enabledConditionName: enabledNamedConditions) {
		auto scriptIdx = 0;
		for (const auto& script: scripts) {
			if (script.type != Script::TYPE_ONENABLED ||
				script.name != enabledConditionName) {
				// no op
			} else {
				auto conditionMet = true;
				Variable returnValue;
				if (evaluateInternal(script.condition, script.executableCondition, returnValue) == true) {
					auto returnValueBoolValue = false;
					if (returnValue.getBooleanValue(returnValueBoolValue, false) == false) {
						_Console::printLine("MiniScript::determineNamedScriptIdxToStart(): " + script.condition + ": expecting boolean return value, but got: " + returnValue.getAsString());
						conditionMet = false;
					} else
					if (returnValueBoolValue == false) {
						conditionMet = false;
					}
				} else {
					_Console::printLine("MiniScript::determineNamedScriptIdxToStart(): " + scriptFileName + ":" + to_string(script.line) + ": " + script.condition + ": parse error");
				}
				if (conditionMet == false) {
					if (VERBOSE == true) {
						_Console::print("MiniScript::determineNamedScriptIdxToStart(): " + script.condition + ": FAILED");
					}
				} else {
					if (VERBOSE == true) {
						_Console::print("MiniScript::determineNamedScriptIdxToStart(): " + script.condition + ": OK");
					}
					return scriptIdx;
				}
			}
			scriptIdx++;
		}
	}
	return SCRIPTIDX_NONE;
}

const string MiniScript::doStatementPreProcessing(const string& processedStatement, const Statement& statement) {
	auto preprocessedStatement = processedStatement;
	//
	struct StatementOperator {
		int idx { OPERATORIDX_NONE };
		Operator operator_;
	};
	//
	auto trimArgument = [](const string& argument) -> const string {
		auto processedArgument = _StringTools::trim(argument);
		if (_StringTools::startsWith(processedArgument, "(") == true && _StringTools::endsWith(processedArgument, ")") == true) {
			processedArgument = _StringTools::substring(processedArgument, 1, processedArgument.size() - 1);
		}
		return processedArgument;
	};
	//
	auto findLeftArgument = [&](const string& statement, int position, int& length, string& brackets) -> const string {
		//
		auto bracketCount = 0;
		auto squareBracketCount = 0;
		auto curlyBracketCount = 0;
		auto quote = '\0';
		auto lc = '\0';
		string argument;
		length = 0;
		for (int i = position; i >= 0; i--) {
			auto c = statement[i];
			if ((c == '"' || c == '\'') && lc != '\\') {
				if (quote == '\0') {
					quote = c;
				} else
				if (quote == c) {
					quote = '\0';
				}
				argument = c + argument;
			} else
			if (quote == '\0') {
				if (c == ')') {
					bracketCount++;
					argument = c + argument;
				} else
				if (c == ']') {
					squareBracketCount++;
					argument = c + argument;
				} else
				if (c == '}') {
					curlyBracketCount++;
					argument = c + argument;
				} else
				if (c == '(') {
					bracketCount--;
					if (bracketCount < 0) {
						brackets = "()";
						return trimArgument(argument);
					}
					argument = c + argument;
				} else
				if (c == '[') {
					squareBracketCount--;
					if (squareBracketCount < 0) {
						brackets = "[]";
						return trimArgument(argument);
					}
					argument = c + argument;
				} else
				if (c == '{') {
					curlyBracketCount--;
					if (curlyBracketCount < 0) {
						brackets = "{}";
						return trimArgument(argument);
					}
					argument = c + argument;
				} else
				if (squareBracketCount == 0 && curlyBracketCount == 0 && c == ',') {
					if (bracketCount == 0) return trimArgument(argument);
					argument = c + argument;
				} else {
					argument = c + argument;
				}
			} else
			if (quote != '\0') {
				argument = c + argument;
			}
			length++;
			//
			lc = c;
		}
		return trimArgument(argument);
	};
	//
	auto findRightArgument = [&](const string& statement, int position, int& length, string& brackets) -> const string {
		//
		auto bracketCount = 0;
		auto squareBracketCount = 0;
		auto curlyBracketCount = 0;
		auto quote = '\0';
		auto lc = '\0';
		string argument;
		length = 0;
		for (auto i = position; i < statement.size(); i++) {
			auto c = statement[i];
			// quote?
			if ((c == '"' || c == '\'') && lc != '\\') {
				if (quote == '\0') {
					quote = c;
				} else
				if (quote == c) {
					quote = '\0';
				}
				argument+= c;
			} else
			// no quote
			if (quote == '\0') {
				if (c == '(') {
					bracketCount++;
					argument+= c;
				} else
				if (c == '[') {
					squareBracketCount++;
					argument+= c;
				} else
				if (c == '{') {
					curlyBracketCount++;
					argument+= c;
				} else
				if (c == ')') {
					bracketCount--;
					if (bracketCount < 0) {
						brackets = "()";
						return trimArgument(argument);
					}
					argument+= c;
				} else
				if (c == ']') {
					squareBracketCount--;
					// array initializer?
					if (_StringTools::startsWith(argument, "[") == false) {
						// no
						if (squareBracketCount < 0) {
							brackets = "[]";
							return trimArgument(argument);
						}
					}
					argument+= c;
				} else
				if (c == '}') {
					curlyBracketCount--;
					if (curlyBracketCount < 0) {
						brackets = "{}";
						return trimArgument(argument);
					}
					argument+= c;
				} else
				if (squareBracketCount == 0 && curlyBracketCount == 0 && c == ',') {
					if (bracketCount == 0) return trimArgument(argument);
					//
					if (argument.empty() == true && (c == ' ' || c == '\t' || c == '\n')) {
						// no op
					} else {
						argument+= c;
					}
				} else {
					//
					if (argument.empty() == true && (c == ' ' || c == '\t' || c == '\n')) {
						// no op
					} else {
						argument+= c;
					}
				}
			} else
			if (quote != '\0') {
				argument+= c;
			}
			length++;
			//
			lc = c;
		}
		//
		return trimArgument(argument);
	};
	//
	auto viewIsLamdaFunction = [](const string_view& candidate) -> bool {
		if (candidate.size() == 0) return false;
		//
		auto i = 0;
		// (
		if (candidate[i++] != '(') return false;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++); if (i >= candidate.size()) return false;
		//
		auto argumentStartIdx = string::npos;
		auto argumentEndIdx = string::npos;
		//
		for (; i < candidate.size(); i++) {
			auto c = candidate[i];
			if (c == '&') {
				if (argumentStartIdx == string::npos) {
					argumentStartIdx = i;
				} else {
					return false;
				}
			} else
			if (c == '$') {
				if (argumentStartIdx == string::npos) {
					argumentStartIdx = i;
				} else
				if (argumentStartIdx == i - 1 && candidate[argumentStartIdx] == '&') {
					// no op
				} else {
					return false;
				}
			} else
			if (c == ',' || c == ')') {
				if (argumentEndIdx == string::npos) {
					if (argumentStartIdx != string::npos) {
						argumentEndIdx = i;
					}
					//
					argumentStartIdx = string::npos;
					argumentEndIdx = string::npos;
				} else {
					return false;
				}
				if (c == ')') {
					i++;
					break;
				}
			} else
			if (argumentStartIdx != string::npos && _Character::isAlphaNumeric(candidate[i]) == false && c != '_') {
				return false;
			}
		}
		//
		if (i >= candidate.size()) return false;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++); if (i >= candidate.size()) return false;
		// -
		if (candidate[i++] != '-') return false;
		//
		if (i >= candidate.size()) return false;
		// >
		if (candidate[i++] != '>') return false;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++); if (i >= candidate.size()) return false;
		//
		if (candidate[i++] != '{') return false;
		//
		return true;
	};
	//
	auto getNextStatementOperator = [&](const string& processedStatement, StatementOperator& nextOperator, const Statement& statement) {
		bool lamdaFunctionDeclaration = false;
		auto curlyBracketCount = 0;
		auto bracketCount = 0;
		auto quote = '\0';
		auto lc = '\0';
		for (auto i = 0; i < processedStatement.size(); i++) {
			auto c = processedStatement[i];
			if ((c == '"' || c == '\'') && lc != '\\') {
				if (quote == '\0') {
					quote = c;
				} else
				if (quote == c) {
					quote = '\0';
				}
			} else
			if (quote == '\0') {
				if (c == '(') {
					// read ahead if inline/lambda function
					string_view lamdaFunctionCandidate(&processedStatement[i], processedStatement.size() - i);
					if (viewIsLamdaFunction(lamdaFunctionCandidate) == true) lamdaFunctionDeclaration = true;
					bracketCount++;
				} else
				if (c == ')') {
					if (lamdaFunctionDeclaration == true) lamdaFunctionDeclaration = false;
					bracketCount--;
				} else
				if (c == '{') {
					curlyBracketCount++;
				} else
				if (c == '}') {
					curlyBracketCount--;
				} else
				if (curlyBracketCount == 0 && lamdaFunctionDeclaration == false) {
					for (int j = OPERATOR_NONE + 1; j < OPERATOR_MAX; j++) {
						auto priorizedOperator = static_cast<Operator>(j);
						string operatorCandidate;
						auto operatorString = getOperatorAsString(priorizedOperator);
						if (operatorString.size() == 1) operatorCandidate+= processedStatement[i];
						if (operatorString.size() == 2 && i + 1 < processedStatement.size()) {
							operatorCandidate+= processedStatement[i];
							operatorCandidate+= processedStatement[i + 1];
						}
						if (operatorString == operatorCandidate && (nextOperator.idx == OPERATORIDX_NONE || priorizedOperator > nextOperator.operator_)) {
							if (i > 0 && isOperatorChar(processedStatement[i - 1]) == true) {
								continue;
							}
							if (operatorString.size() == 2 && i + 2 < processedStatement.size() && isOperatorChar(processedStatement[i + 2]) == true) {
								continue;
							} else
							if (operatorString.size() == 1 && i + 1 < processedStatement.size() && isOperatorChar(processedStatement[i + 1]) == true) {
								continue;
							}
							if (priorizedOperator == OPERATOR_SUBTRACTION) {
								string leftArgumentBrackets;
								auto leftArgumentLeft = 0;
								auto leftArgument = findLeftArgument(processedStatement, i - 1, leftArgumentLeft, leftArgumentBrackets);
								if (leftArgument.length() == 0) continue;
							}
							nextOperator.idx = i;
							nextOperator.operator_ = priorizedOperator;
						}
					}
				}
			}
			lc = c;
		}

		//
		if (bracketCount > 0) {
			_Console::printLine(getStatementInformation(statement) + ": unbalanced bracket count: " + to_string(_Math::abs(bracketCount)) + " " + (bracketCount < 0?"too much closed":"still open"));
			//
			parseErrors.push_back(getStatementInformation(statement) + ": unbalanced bracket count: " + to_string(_Math::abs(bracketCount)) + " " + (bracketCount < 0?"too much closed":"still open"));
			//
			return false;
		}
		//
		return nextOperator.idx != OPERATORIDX_NONE;
	};
	//
	StatementOperator nextOperators;
	while (getNextStatementOperator(preprocessedStatement, nextOperators, statement) == true) {
		//
		Method* method { nullptr };
		Method* prefixOperatorMethod { nullptr };
		Method* postfixOperatorMethod { nullptr };
		//
		{
			// resolve method
			method = getOperatorMethod(nextOperators.operator_);
			if (method == nullptr) {
				_Console::printLine(getStatementInformation(statement) + ": operator @" + to_string(nextOperators.idx) + ": no operator method found");
				scriptValid = false;
				return preprocessedStatement;
			}
			// default for methods with 1 argument is post fix operator method
			postfixOperatorMethod = method;
			//
			// special case prefix or postfix operator methods, that have 1 argument only
			if (method->getOperator() == OPERATOR_POSTFIX_INCREMENT ||
				method->getOperator() == OPERATOR_PREFIX_INCREMENT) {
				prefixOperatorMethod = getOperatorMethod(OPERATOR_POSTFIX_INCREMENT);
				postfixOperatorMethod = getOperatorMethod(OPERATOR_PREFIX_INCREMENT);
			}
			//
			if (method->getOperator() == OPERATOR_POSTFIX_DECREMENT ||
				method->getOperator() == OPERATOR_PREFIX_DECREMENT) {
				prefixOperatorMethod = getOperatorMethod(OPERATOR_POSTFIX_DECREMENT);
				postfixOperatorMethod = getOperatorMethod(OPERATOR_PREFIX_DECREMENT);
			}
			//
			if (method->getOperator() == OPERATOR_POSTFIX_INCREMENT ||
				method->getOperator() == OPERATOR_PREFIX_INCREMENT ||
				method->getOperator() == OPERATOR_POSTFIX_DECREMENT ||
				method->getOperator() == OPERATOR_PREFIX_DECREMENT) {
				//
				if (prefixOperatorMethod == nullptr) {
					_Console::printLine(getStatementInformation(statement) + ": operator @" + to_string(nextOperators.idx) + ": no prefix operator method found");
					scriptValid = false;
					return preprocessedStatement;
				}
				//
				if (postfixOperatorMethod == nullptr) {
					_Console::printLine(getStatementInformation(statement) + ": operator @" + to_string(nextOperators.idx) + ": no postfix operator method found");
					scriptValid = false;
					return preprocessedStatement;
				}
			}
		}
		//
		if (method->isVariadic() == false &&
			method->getArgumentTypes().size() == 1) {
			// find the single argument right
			auto operatorString = getOperatorAsString(nextOperators.operator_);
			// find left argument
			string leftArgumentBrackets;
			int leftArgumentLength = 0;
			auto leftArgument = findLeftArgument(preprocessedStatement, nextOperators.idx - 1, leftArgumentLength, leftArgumentBrackets);
			// find argument right
			string rightArgumentBrackets;
			int rightArgumentLength = 0;
			auto rightArgument = findRightArgument(preprocessedStatement, nextOperators.idx + operatorString.size(), rightArgumentLength, rightArgumentBrackets);
			//
			if (leftArgument.empty() == false) {
				// substitute with method call
				preprocessedStatement =
					_StringTools::substring(preprocessedStatement, 0, nextOperators.idx - leftArgumentLength) +
					prefixOperatorMethod->getMethodName() + "(" + leftArgument + ")" +
					_StringTools::substring(preprocessedStatement, nextOperators.idx + operatorString.size(), preprocessedStatement.size()
				);
			} else
			if (rightArgument.empty() == false) {
				// substitute with method call
				preprocessedStatement =
					_StringTools::substring(preprocessedStatement, 0, nextOperators.idx) +
					postfixOperatorMethod->getMethodName() + "(" + rightArgument + ")" +
					_StringTools::substring(preprocessedStatement, nextOperators.idx + operatorString.size() + rightArgumentLength, preprocessedStatement.size());
			} else {
				_Console::printLine(getStatementInformation(statement) + ": operator @" + to_string(nextOperators.idx) + ": requires left or right argument");
				scriptValid = false;
				return preprocessedStatement;
			}
		} else
		if (method->isVariadic() == true ||
			method->getArgumentTypes().size() == 2) {
			//
			auto operatorString = getOperatorAsString(nextOperators.operator_);
			// find left argument
			string leftArgumentBrackets;
			int leftArgumentLength = 0;
			auto leftArgument = findLeftArgument(preprocessedStatement, nextOperators.idx - 1, leftArgumentLength, leftArgumentBrackets);
			// find right argument
			string rightArgumentBrackets;
			int rightArgumentLength = 0;
			auto rightArgument = findRightArgument(preprocessedStatement, nextOperators.idx + operatorString.size(), rightArgumentLength, rightArgumentBrackets);
			//
			if (leftArgument.empty() == true || rightArgument.empty() == true) {
				_Console::printLine(getStatementInformation(statement) + ": operator @" + to_string(nextOperators.idx) + ": requires left and right argument");
				scriptValid = false;
				return preprocessedStatement;
			}
			//
			if (nextOperators.operator_ == OPERATOR_SET) {
				leftArgument = "\"" + doStatementPreProcessing(leftArgument, statement) + "\"";
			}
			// substitute with method call
			preprocessedStatement =
				_StringTools::substring(preprocessedStatement, 0, nextOperators.idx - leftArgumentLength) +
				method->getMethodName() + "(" + leftArgument + ", " + rightArgument + ")" +
				_StringTools::substring(preprocessedStatement, nextOperators.idx + operatorString.size() + rightArgumentLength, preprocessedStatement.size()
			);
		}
		//
		nextOperators = StatementOperator();
	}
	//
	return preprocessedStatement;
}

bool MiniScript::getObjectMemberAccess(const string_view& executableStatement, string_view& object, string_view& method, int& methodStartIdx, const Statement& statement) {
	//
	auto objectMemberAccess = false;
	auto objectStartIdx = string::npos;
	auto objectEndIdx = string::npos;
	auto memberCallStartIdx = string::npos;
	auto memberCallEndIdx = string::npos;
	//
	auto quote = '\0';
	auto lc = '\0';
	auto bracketCount = 0;
	auto squareBracketCount = 0;
	auto curlyBracketCount = 0;
	for (auto i = 0; i < executableStatement.size(); i++) {
		auto c = executableStatement[i];
		// handle quotes
		if ((c == '"' || c == '\'') && lc != '\\') {
			if (quote == '\0') {
				quote = c;
			} else
			if (quote == c) {
				quote = '\0';
			}
		} else
		if (quote != '\0') {
			// no op
		} else
		if (c == '(') {
			bracketCount++;
		} else
		if (c == ')') {
			bracketCount--;
		} else
		if (c == '[') {
			squareBracketCount++;
		} else
		if (c == ']') {
			squareBracketCount--;
		} else
		if (c == '{') {
			curlyBracketCount++;
		} else
		if (c == '}') {
			curlyBracketCount--;
		} else
		if (i > 3 &&
			lc == '-' && c == '>' &&
			bracketCount == 0 &&
			squareBracketCount == 0 &&
			curlyBracketCount == 0) {
			//
			auto objectStartIdxCandidate = 0;
			auto objectEndIdxCandidate = i - 1;
			auto memberCallStartIdxCandidate = i + 1;
			auto memberCallEndIdxCandidate = executableStatement.size();
			//
			auto objectCandidate = _StringTools::viewTrim(string_view(&executableStatement[objectStartIdxCandidate], objectEndIdxCandidate - objectStartIdxCandidate));
			auto methodCandidate = _StringTools::viewTrim(string_view(&executableStatement[memberCallStartIdxCandidate], memberCallEndIdxCandidate - memberCallStartIdxCandidate));
			//
			if (objectCandidate.empty() == false && methodCandidate.empty() == false) {
				//
				objectStartIdx = objectStartIdxCandidate;
				objectEndIdx = objectEndIdxCandidate;
				memberCallStartIdx = memberCallStartIdxCandidate;
				memberCallEndIdx = memberCallEndIdxCandidate;
				//
				object = objectCandidate;
				method = methodCandidate;
				//
				methodStartIdx = memberCallStartIdx;
				objectMemberAccess = true;
				// dont break here, we can have multiple member access operators here, but we want the last one in this step
			}
		}
		//
		lc = c;
	}

	//
	return objectMemberAccess;
}

void MiniScript::dumpScriptState(ScriptState& scriptState, const string& message) {
	_Console::printLine("MiniScript::dumpScriptState(): " + message + ": " + to_string(scriptStateStack.size()) + " on stack");
	_Console::printLine(string("\t") + "state: " + to_string(scriptState.state));
	_Console::printLine(string("\t") + "lastState: " + to_string(scriptState.lastState));
	_Console::printLine(string("\t") + "running: " + (scriptState.running == true?"true":"false"));
	_Console::printLine(string("\t") + "scriptIdx: " + to_string(scriptState.scriptIdx));
	_Console::printLine(string("\t") + "statementIdx: " + to_string(scriptState.statementIdx));
	_Console::printLine(string("\t") + "gotoStatementIdx: " + to_string(scriptState.gotoStatementIdx));
	_Console::printLine(string("\t") + "variable count: " + to_string(scriptState.variables.size()));
	_Console::printLine(string("\t") + "block stack count: " + to_string(scriptState.blockStack.size()));
	array<string, 8> blockStackTypes {
		"BLOCKTYPE_NONE",
		"BLOCKTYPE_GLOBAL",
		"BLOCKTYPE_FUNCTION",
		"BLOCKTYPE_FOR",
		"BLOCKTYPE_FORTIME",
		"BLOCKTYPE_IF",
		"BLOCKTYPE_SWITCH",
		"BLOCKTYPE_CASE"
	};
	for (const auto& block: scriptState.blockStack) {
		_Console::printLine(string("\t\t") + blockStackTypes[block.type]);
	}
	_Console::printLine(string("\t") + "returnValue: " + scriptState.returnValue.getValueAsString());
}

bool MiniScript::call(int scriptIdx, span<Variable>& arguments, Variable& returnValue, bool pushScriptState) {
	if (scriptIdx < 0 || scriptIdx >= scripts.size()) {
		_Console::printLine("MiniScript::call(): Invalid script index: " + to_string(scriptIdx));
		return false;
	}
	auto& script = scripts[scriptIdx];
	//
	if (script.type != Script::TYPE_FUNCTION &&
			script.type != Script::TYPE_STACKLET) {
		_Console::printLine("MiniScript::call(): " + (script.name.empty() == false?script.name:script.condition) + ": Script is not a function/callable/stacklet.");
		return false;
	}
	// copy script state
	ScriptState currentScriptState = getScriptState();
	//
	if (pushScriptState == true) {
		if (script.type == Script::TYPE_STACKLET) {
			_Console::printLine("MiniScript::call(): " + script.condition + ": Stacklets can not be called with a stack.");
			return false;
		}
		this->pushScriptState();
		// script state vector could get modified, so
		auto& scriptState = getScriptState();
		// also put named arguments into state context variables
		auto argumentIdx = 0;
		for (const auto& argument: script.arguments) {
			if (argumentIdx == arguments.size()) {
				break;
			}
			setVariable(argument.name, arguments[argumentIdx], nullptr, argument.reference);
			argumentIdx++;
		}
		//
		resetScriptExecutationState(scriptIdx, STATEMACHINESTATE_NEXT_STATEMENT);
	} else {
		//
		if (script.type != Script::TYPE_STACKLET) {
			_Console::printLine("MiniScript::call(): " + script.condition + ": Function/Callable can not be called with no stack.");
			return false;
		}
		//
		resetStackletScriptExecutationState(scriptIdx, STATEMACHINESTATE_NEXT_STATEMENT);
	}
	// script state vector could get modified, so
	{
		auto& scriptState = getScriptState();
		// run this function dude
		scriptState.running = true;
	}
	// execute
	for (;true;) {
		execute();
		// run this function dude
		if (getScriptState().running == false) break;
	}
	// get return value
	{
		const auto& scriptState = getScriptState();
		// run this function dude
		returnValue = scriptState.returnValue;
	}
	// done, pop the function script state
	if (pushScriptState == true) {
		popScriptState();
	} else {
		auto& scriptState = getScriptState();
		scriptState.state = currentScriptState.state;
		scriptState.lastState = currentScriptState.lastState;
		scriptState.lastStateMachineState = currentScriptState.lastStateMachineState;
		scriptState.running = currentScriptState.running;
		scriptState.scriptIdx = currentScriptState.scriptIdx;
		scriptState.statementIdx = currentScriptState.statementIdx;
		scriptState.gotoStatementIdx = currentScriptState.gotoStatementIdx;
		scriptState.returnValue = currentScriptState.returnValue;
	}
	// try garbage collection
	tryGarbageCollection();
	//
	return true;
}

const vector<MiniScript::Method*> MiniScript::getMethods() {
	vector<Method*> methods;
	for (const auto& [methodName, method]: this->methods) {
		if (method->isPrivate() == true) continue;
		methods.push_back(method);
	}
	//
	struct {
		bool operator()(Method* a, Method* b) const {
			auto aPrefix = _StringTools::substring(a->getMethodName(), 0, _StringTools::lastIndexOf(a->getMethodName(), ".") + 1);
			auto aName = _StringTools::substring(a->getMethodName(), _StringTools::lastIndexOf(a->getMethodName(), ".") + 1);
			auto bPrefix = _StringTools::substring(b->getMethodName(), 0, _StringTools::lastIndexOf(b->getMethodName(), ".") + 1);
			auto bName = _StringTools::substring(b->getMethodName(), _StringTools::lastIndexOf(b->getMethodName(), ".") + 1);
			array<string, 6> prefixes {
				"is",
				"has",
				"get",
				"set",
				"unset",
				"compute"
			};
			array<string, 6> sortPrefixes {
				"0",
				"1",
				"2",
				"3",
				"4",
				"5"
			};
			int aPrefixIdx = 0;
			for (const auto& prefix: prefixes) {
				if ((aName != prefix || aPrefix.empty() == false) && _StringTools::startsWith(aName, prefix) == true) {
					aName = _StringTools::substring(aName, prefix.size());
					break;
				}
				aPrefixIdx++;
			}
			int bPrefixIdx = 0;
			for (const auto& prefix: prefixes) {
				if ((bName != prefix || bPrefix.empty() == false) && _StringTools::startsWith(bName, prefix) == true) {
					bName = _StringTools::substring(bName, prefix.size());
					break;
				}
				bPrefixIdx++;
			}
			if (aName == bName) {
				return aPrefix + (aPrefixIdx < 6?sortPrefixes[aPrefixIdx]:"") + aName < bPrefix + (bPrefixIdx < 6?sortPrefixes[bPrefixIdx]:"") + bName;
			} else {
				return aPrefix + aName < bPrefix + bName;
			}
		}
	} sortFunction;
	//
	sort(methods.begin(), methods.end(), sortFunction);
	//
	return methods;
}

const vector<MiniScript::Method*> MiniScript::getOperatorMethods() {
	vector<Method*> methods;
	for (const auto& [operatorId, method]: operators) {
		methods.push_back(method);
	}
	return methods;
}

const string MiniScript::getScriptInformation(int scriptIdx, bool includeStatements) {
	if (scriptIdx < 0 || scriptIdx >= scripts.size()) {
		_Console::printLine("MiniScript::getScriptInformation(): invalid script index: " + to_string(scriptIdx));
		return string();
	}
	const auto& script = scripts[scriptIdx];
	string result;
	string argumentsString;
	switch(script.type) {
		case Script::TYPE_FUNCTION:
		case Script::TYPE_STACKLET: {
			for (const auto& argument: script.arguments) {
				if (argumentsString.empty() == false) argumentsString+= ", ";
				if (argument.reference == true) argumentsString+= "&";
				argumentsString+= argument.name;
			}
			argumentsString = "(" + argumentsString + ")";
			if (script.type == Script::TYPE_FUNCTION) result+= "function: "; else result+= "stacklet: ";
			break;
		}
		case Script::TYPE_ON: result+= "on: "; break;
		case Script::TYPE_ONENABLED: result+= "on-enabled: "; break;
	}
	if (script.condition.empty() == false)
		result+= script.condition + argumentsString + "; ";
	if (script.name.empty() == false) {
		result+= "name = '" + script.name + argumentsString + "';\n";
	} else {
		result+= "\n";
	}
	if (includeStatements == true) {
		for (const auto& statement: script.statements) {
			result+= "\t" + to_string(statement.statementIdx) + ": " + _StringTools::replace(statement.executableStatement, "\n", "\n\t\t") + (statement.gotoStatementIdx != STATEMENTIDX_NONE?" (gotoStatement " + to_string(statement.gotoStatementIdx) + ")":"") + "\n";
		}
		result+= "\n";
	}
	//
	return result;
}

const string MiniScript::getInformation() {
	string result;
	result+= "Script: " + scriptPathName + "/" + scriptFileName + " (runs " + (native == true?"natively":"interpreted") + ")" + "\n\n";
	auto scriptIdx = 0;
	for (const auto& script: scripts) {
		result+= getScriptInformation(scriptIdx);
		scriptIdx++;
	}

	//
	result+="State Machine States:\n";
	{
		vector<string> states;
		for (const auto& [stateMachineStateId, stateMachineState]: stateMachineStates) {
			string state;
			state = stateMachineState->getName() + "(" + to_string(stateMachineState->getId()) + ")";
			states.push_back(state);
		}
		sort(states.begin(), states.end());
		for (const auto& state: states) result+= state+ "\n";
	}
	result+= "\n";

	//
	result+= "Methods:\n";
	{
		vector<string> methods;
		for (const auto& [methodName, method]: this->methods) {
			if (method->isPrivate() == true) continue;
			string methodDescription;
			methodDescription+= method->getMethodName();
			methodDescription+= "(";
			methodDescription+= method->getArgumentsInformation();
			methodDescription+= "): ";
			methodDescription+= Variable::getReturnTypeAsString(method->getReturnValueType(), method->isReturnValueNullable());
			methods.push_back(methodDescription);
		}
		sort(methods.begin(), methods.end());
		for (const auto& method: methods) result+= method + "\n";
	}
	result+= "\n";

	//
	result+= "Operators:\n";
	{
		vector<string> operators;
		for (const auto& [operatorId, method]: this->operators) {
			string operatorString;
			operatorString+= getOperatorAsString(method->getOperator());
			operatorString+= " --> ";
			operatorString+= method->getMethodName();
			operatorString+= "(";
			operatorString+= method->getArgumentsInformation();
			operatorString+= "): ";
			operatorString+= Variable::getReturnTypeAsString(method->getReturnValueType(), method->isReturnValueNullable());
			operators.push_back(operatorString);
		}
		sort(operators.begin(), operators.end());
		for (const auto& method: operators) result+= method + "\n";
	}
	result+= "\n";

	//
	result+= "Variables:\n";
	{
		const auto& scriptState = getScriptState();
		vector<string> variables;
		for (const auto& [scriptVariableName, scriptVariableValue]: scriptState.variables) {
			string variable;
			variable+= scriptVariableName + " = " + scriptVariableValue->getAsString();
			variables.push_back(variable);
		}
		sort(variables.begin(), variables.end());
		for (const auto& variable: variables) result+= variable + "\n";
	}

	//
	return result;
}

void MiniScript::registerStateMachineStates() {
	// base
	if (native == false) {
		//
		class ScriptStateNextStatement: public StateMachineState {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptStateNextStatement(MiniScript* miniScript): StateMachineState(), miniScript(miniScript) {}
			virtual const string getName() override {
				return "STATEMACHINESTATE_NEXT_STATEMENT";
			}
			virtual int getId() override {
				return STATEMACHINESTATE_NEXT_STATEMENT;
			}
			virtual void execute() override {
				if (miniScript->getScriptState().statementIdx == STATEMENTIDX_NONE) {
					miniScript->enabledNamedConditions.clear();
					miniScript->timeEnabledConditionsCheckLast = TIME_NONE;
					miniScript->setScriptStateState(STATEMACHINESTATE_WAIT_FOR_CONDITION);
					return;
				}
				if (miniScript->native == false) miniScript->executeNextStatement();
			}
		};
		registerStateMachineState(new ScriptStateNextStatement(this));
	}
	{
		//
		class ScriptStateWait: public StateMachineState {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptStateWait(MiniScript* miniScript): StateMachineState(), miniScript(miniScript) {}
			virtual const string getName() override {
				return "STATEMACHINESTATE_WAIT";
			}
			virtual int getId() override {
				return STATEMACHINESTATE_WAIT;
			}
			virtual void execute() override {
				auto now = _Time::getCurrentMillis();
				if (now > miniScript->getScriptState().timeWaitStarted + miniScript->getScriptState().timeWaitTime) {
					miniScript->setScriptStateState(STATEMACHINESTATE_NEXT_STATEMENT);
				} else {
					#if !defined(MINISCRIPT_NO_SLEEP)
						_Thread::sleep(10);
					#endif
				}
			}
		};
		registerStateMachineState(new ScriptStateWait(this));
	}
	{
		//
		class ScriptStateWaitForCondition: public StateMachineState {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptStateWaitForCondition(MiniScript* miniScript): StateMachineState(), miniScript(miniScript) {}
			virtual const string getName() override {
				return "STATEMACHINESTATE_WAIT_FOR_CONDITION";
			}
			virtual int getId() override {
				return STATEMACHINESTATE_WAIT_FOR_CONDITION;
			}
			virtual void execute() override {
				auto now = _Time::getCurrentMillis();
				if (now < miniScript->getScriptState().timeWaitStarted + miniScript->getScriptState().timeWaitTime) {
					return;
				}
				auto scriptIdxToStart = miniScript->determineScriptIdxToStart();
				if (scriptIdxToStart == SCRIPTIDX_NONE) {
					miniScript->getScriptState().timeWaitStarted = now;
					miniScript->getScriptState().timeWaitTime = 100LL;
					return;
				}
				miniScript->resetScriptExecutationState(scriptIdxToStart, STATEMACHINESTATE_NEXT_STATEMENT);
			}
		};
		registerStateMachineState(new ScriptStateWaitForCondition(this));
	}
}

void MiniScript::registerMethods() {
	// unregister old registered methods
	for (const auto& [scriptMethodId, scriptMethod]: methods) delete scriptMethod;
	methods.clear();

	// register math methods
	miniScriptMath = make_unique<MathMethods>(this);
	miniScriptMath->registerMethods();

	// base script methods
	// 	register base methods
	BaseMethods::registerMethods(this);

	// 	register string methods
	StringMethods::registerMethods(this);

	// 	register byte array methods
	ByteArrayMethods::registerMethods(this);

	// 	register array methods
	ArrayMethods::registerMethods(this);

	// 	register map methods
	MapMethods::registerMethods(this);

	// 	register set methods
	SetMethods::registerMethods(this);

	// 	register script methods
	ScriptMethods::registerMethods(this);

	// additional script methods
	// register application methods
	ApplicationMethods::registerMethods(this);

	// register console methods
	ConsoleMethods::registerMethods(this);

	// register context methods
	ContextMethods::registerMethods(this);

	// register cryptography methods
	CryptographyMethods::registerMethods(this);

	// register file system methods
	FileSystemMethods::registerMethods(this);

	// register JSON methods
	JSONMethods::registerMethods(this);

	// register network methods
	NetworkMethods::registerMethods(this);

	// register time methods
	TimeMethods::registerMethods(this);

	// register XML methods
	XMLMethods::registerMethods(this);

	//
	for (const auto dataType: dataTypes) {
		if (dataType->isMathDataType() == true) miniScriptMath->registerDataType(dataType);
		dataType->registerMethods(this);
	}

	// determine operators
	for (const auto& [scriptMethodName, scriptMethod]: methods) {
		auto methodOperator = scriptMethod->getOperator();
		if (methodOperator != OPERATOR_NONE) {
			auto methodOperatorString = getOperatorAsString(methodOperator);
			auto scriptOperatorIt = operators.find(static_cast<uint8_t>(methodOperator));
			if (scriptOperatorIt != operators.end()) {
				_Console::printLine("MiniScript::registerMethods(): operator '" + methodOperatorString + "' already registered for method " + scriptMethod->getMethodName() + "");
				continue;
			}
			operators[static_cast<uint8_t>(methodOperator)] = scriptMethod;
		}
	}
}

void MiniScript::registerVariables() {
	//
	for (const auto& [variableName, variable]: getRootScriptState().variables) delete variable;
	getRootScriptState().variables.clear();

	//
	miniScriptMath->registerConstants();

	// base script constants
	// 	register base constants
	BaseMethods::registerConstants(this);

	// 	register string constants
	StringMethods::registerConstants(this);

	// 	register byte array constants
	ByteArrayMethods::registerConstants(this);

	// 	register array constants
	ArrayMethods::registerConstants(this);

	// 	register map constants
	MapMethods::registerConstants(this);

	// 	register set constants
	SetMethods::registerConstants(this);

	// 	register script constants
	ScriptMethods::registerConstants(this);

	// additional script constants
	// register application constants
	ApplicationMethods::registerConstants(this);

	// register console constants
	ConsoleMethods::registerConstants(this);

	// register context constants
	ContextMethods::registerConstants(this);

	// register cryptography constants
	CryptographyMethods::registerConstants(this);

	// register file system constants
	FileSystemMethods::registerConstants(this);

	// register JSON constants
	JSONMethods::registerConstants(this);

	// register network constants
	NetworkMethods::registerConstants(this);

	// register time constants
	TimeMethods::registerConstants(this);

	// register XML constants
	XMLMethods::registerConstants(this);

	//
	for (const auto dataType: dataTypes) dataType->registerConstants(this);
}

void MiniScript::createLamdaFunction(Variable& variable, const vector<string_view>& arguments, const string_view& functionScriptCode, bool populateThis, const Statement& statement) {
	// function declaration
	auto functionName = string() + "lamda_function_" + to_string(inlineFunctionIdx++);
	auto inlineFunctionScriptCode = "function: " + functionName + "(";
	if (populateThis == true) inlineFunctionScriptCode+= "&$this";
	auto argumentIdx = 0;
	for (const auto& argument: arguments) {
		if (argumentIdx > 0 || populateThis == true) inlineFunctionScriptCode+= ",";
		inlineFunctionScriptCode+= argument;
		argumentIdx++;
	}
	inlineFunctionScriptCode+= string() + ")" + "\n";
	// function definition
	auto scriptCode = string(functionScriptCode);
	auto lineIdx = MiniScript::LINE_FIRST;
	auto currentLineIdx = MiniScript::LINE_FIRST;
	for (auto i = 0; i < scriptCode.size(); i++) {
		//
		currentLineIdx = lineIdx;

		// try to get next statement code
		auto statementCode = getNextStatement(scriptCode, i, lineIdx);
		//
		inlineFunctionScriptCode+= doStatementPreProcessing(statementCode, statement) + "\n";
	}
	//
	inlineFunctionScriptCode+= "\n";
	inlineFunctionScriptCode+= string() + "end" + "\n";
	// store it to be parsed later
	deferredInlineScriptCodes.push_back(inlineFunctionScriptCode);
	//
	variable.setFunctionAssignment(functionName);
}

void MiniScript::createStacklet(Variable& variable, const string& scopeName, const vector<string_view>& arguments, const string_view& stackletScriptCode, const Statement& statement) {
	// stacklet declaration
	auto stackletName = string() + "stacklet_" + to_string(inlineStackletIdx++);
	auto inlineStackletScriptCode = "stacklet: " + stackletName + "(" + scopeName + ")" + "\n";
	// stacklet definition
	auto scriptCode = string(stackletScriptCode);
	auto lineIdx = MiniScript::LINE_FIRST;
	auto currentLineIdx = MiniScript::LINE_FIRST;
	for (auto i = 0; i < scriptCode.size(); i++) {
		//
		currentLineIdx = lineIdx;

		// try to get next statement code
		auto statementCode = getNextStatement(scriptCode, i, lineIdx);
		//
		inlineStackletScriptCode+= doStatementPreProcessing(statementCode, statement) + "\n";
	}
	//
	inlineStackletScriptCode+= "\n";
	inlineStackletScriptCode+= string() + "end" + "\n";
	// store it to be parsed later
	deferredInlineScriptCodes.push_back(inlineStackletScriptCode);
	//
	variable.setStackletAssignment(stackletName);
}

const MiniScript::Variable MiniScript::initializeArray(const string_view& initializerString, MiniScript* miniScript, int scriptIdx, const Statement& statement) {
	Variable variable;
	variable.setType(TYPE_ARRAY);
	//
	auto bracketCount = 0;
	auto squareBracketCount = 0;
	auto curlyBracketCount = 0;
	auto quote = '\0';
	auto arrayValueStart = string::npos;
	auto arrayValueEnd = string::npos;
	auto quotedArrayValueStart = string::npos;
	auto quotedArrayValueEnd = string::npos;
	auto lc = '\0';
	auto i = 0;
	//
	auto pushToArray = [&]() -> void {
		// quoted value
		if (quotedArrayValueStart != string::npos) {
			quotedArrayValueStart++;
			auto arrayValueLength = quotedArrayValueEnd - quotedArrayValueStart;
			if (arrayValueLength > 0) {
				auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[quotedArrayValueStart], arrayValueLength));
				if (arrayValueStringView.empty() == false) {
					variable.pushArrayEntry(string(arrayValueStringView));
				}
			}
		} else
		// unquoted value
		if (arrayValueStart != string::npos) {
			arrayValueEnd = i - 1;
			auto arrayValueLength = arrayValueEnd - arrayValueStart + 1;
			if (arrayValueLength > 0) {
				auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[arrayValueStart], arrayValueLength));
				if (arrayValueStringView.empty() == false) {
					Variable arrayValue;
					arrayValue.setImplicitTypedValueFromStringView(arrayValueStringView, miniScript, scriptIdx, statement);
					variable.pushArrayEntry(arrayValue);
				}
			}
		}
		//
		quotedArrayValueStart = string::npos;
		quotedArrayValueEnd = string::npos;
		arrayValueStart = string::npos;
		arrayValueEnd = string::npos;
	};
	//
	for (; i < initializerString.size(); i++) {
		auto c = initializerString[i];
		// quotes
		if (squareBracketCount == 1 && curlyBracketCount == 0 && (c == '"' || c == '\'') && lc != '\\') {
			if (quote == '\0') {
				quote = c;
				quotedArrayValueStart = i;
			} else
			if (quote == c) {
				quote = '\0';
				quotedArrayValueEnd = i;
			}
		} else
		// no quote
		if (quote == '\0') {
			// , -> push to array
			if (squareBracketCount == 1 && curlyBracketCount == 0 && bracketCount == 0 && c == ',') {
				// push to array
				pushToArray();
			} else
			// possible function call
			if (c == '(') {
				bracketCount++;
			} else
			if (c == ')') {
				bracketCount--;
			} else
			// array initializer
			if (c == '[' && curlyBracketCount == 0 && bracketCount == 0) {
				// we have a inner array initializer, mark it
				if (squareBracketCount == 1) arrayValueStart = i;
				// increase square bracket count
				squareBracketCount++;
			} else
			// end of array initializer
			if (c == ']' && curlyBracketCount == 0 && bracketCount == 0) {
				squareBracketCount--;
				// done? push to array
				if (squareBracketCount == 0) {
					// push to array
					pushToArray();
				} else
				// otherwise push inner array initializer
				if (squareBracketCount == 1) {
					// parse and push
					if (arrayValueStart != string::npos) {
						arrayValueEnd = i;
						auto arrayValueLength = arrayValueEnd - arrayValueStart + 1;
						if (arrayValueLength > 0) {
							auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[arrayValueStart], arrayValueLength));
							if (arrayValueStringView.empty() == false) {
								auto arrayValue = initializeArray(arrayValueStringView, miniScript, scriptIdx, statement);
								variable.pushArrayEntry(arrayValue);
							}
						}
						//
						arrayValueStart = string::npos;
						arrayValueEnd = string::npos;
					}
				}
			} else
			// map/set initializer
			if (c == '{' && squareBracketCount == 1 && bracketCount == 0) {
				// we have a inner map/set initializer, mark it
				if (curlyBracketCount == 0) arrayValueStart = i;
				// increase curly bracket count
				curlyBracketCount++;
			} else
			// end of map/set initializer
			if (c == '}' && squareBracketCount == 1 && bracketCount == 0) {
				curlyBracketCount--;
				// otherwise push inner array initializer
				if (curlyBracketCount == 0) {
					// parse and push
					if (arrayValueStart != string::npos) {
						arrayValueEnd = i;
						auto arrayValueLength = arrayValueEnd - arrayValueStart + 1;
						if (arrayValueLength > 0) {
							auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[arrayValueStart], arrayValueLength));
							if (arrayValueStringView.empty() == false) {
								auto arrayValue = initializeMapSet(arrayValueStringView, miniScript, scriptIdx, statement);
								variable.pushArrayEntry(arrayValue);
							}
						}
						//
						arrayValueStart = string::npos;
						arrayValueEnd = string::npos;
					}
				}
			} else
			// set up argument start
			if (squareBracketCount == 1 && curlyBracketCount == 0 && bracketCount == 0 && arrayValueStart == string::npos && c != ' ' && c != '\t' && c != '\n') {
				arrayValueStart = i;
			}
		}
		//
		lc = c;
	}
	//
	auto initalizer = make_unique<MiniScript::Variable::Initializer>(string(initializerString), statement, nullptr);
	variable.ir.initializer->copy(initalizer.get());
	//
	return variable;
}

const MiniScript::Variable MiniScript::initializeMapSet(const string_view& initializerString, MiniScript* miniScript, int scriptIdx, const Statement& statement) {
	Variable variable;
	variable.setType(TYPE_MAP);
	//
	auto bracketCount = 0;
	auto curlyBracketCount = 0;
	auto squareBracketCount = 0;
	auto quote = '\0';
	auto mapKeyStart = string::npos;
	auto mapKeyEnd = string::npos;
	auto mapValueStart = string::npos;
	auto mapValueEnd = string::npos;
	auto quotedMapKeyStart = string::npos;
	auto quotedMapKeyEnd = string::npos;
	auto quotedMapValueStart = string::npos;
	auto quotedMapValueEnd = string::npos;
	auto i = 0;
	enum ParseMode { PARSEMODE_KEY, PARSEMODE_VALUE };
	auto parseMode = PARSEMODE_KEY;
	auto hasValues = false;
	auto inlineFunctionSignatureStart = string::npos;

	//
	auto insertMapKeyValuePair = [&]() -> void {
		//
		string_view mapKey;
		// quoted map key
		if (quotedMapKeyStart != string::npos && quotedMapKeyEnd != string::npos) {
			quotedMapKeyStart++;
			auto mapKeyLength = quotedMapKeyEnd - quotedMapKeyStart;
			if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[quotedMapKeyStart], mapKeyLength));
		} else
		// unquoted map key
		if (mapKeyStart != string::npos && mapKeyEnd != string::npos) {
			//
			auto mapKeyLength = mapKeyEnd - mapKeyStart + 1;
			if (mapKeyLength > 0) {
				mapKey = _StringTools::viewTrim(string_view(&initializerString[mapKeyStart], mapKeyLength));
				if (mapKey.empty() == true) mapKey = string_view();
			}
		}
		//
		quotedMapKeyStart = string::npos;
		quotedMapKeyEnd = string::npos;
		mapKeyStart = string::npos;
		mapKeyEnd = string::npos;
		// validate map key
		if (mapKey.empty() == true) {
			// no op
		} else
		if (viewIsKey(mapKey) == false) {
			_Console::printLine(miniScript->getStatementInformation(statement) + ": invalid key name, ignoring map entry: " + string(mapKey));
		} else {
			// quoted map value
			if (quotedMapValueStart != string::npos && quotedMapValueEnd != string::npos) {
				quotedMapValueStart++;
				auto mapValueLength = quotedMapValueEnd - quotedMapValueStart;
				if (mapValueLength > 0) {
					auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[quotedMapValueStart], mapValueLength));
					if (mapValueStringView.empty() == false) {
						//
						variable.setMapEntry(string(mapKey), string(mapValueStringView));
						//
						hasValues = true;
					}
				}
			} else
			// unquoted map value
			if (mapValueStart != string::npos && mapValueEnd != string::npos) {
				auto mapValueLength = mapValueEnd - mapValueStart + 1;
				if (mapValueLength > 0) {
					auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[mapValueStart], mapValueLength));
					if (mapValueStringView.empty() == false) {
						Variable mapValue;
						mapValue.setImplicitTypedValueFromStringView(mapValueStringView, miniScript, scriptIdx, statement);
						//
						variable.setMapEntry(string(mapKey), mapValue);
						//
						hasValues = true;
					}
				}
			} else {
				//
				variable.setMapEntry(string(mapKey), Variable());
			}
		}
		//
		quotedMapValueStart = string::npos;
		quotedMapValueEnd = string::npos;
		mapValueStart = string::npos;
		mapValueEnd = string::npos;
		inlineFunctionSignatureStart = string::npos;

		//
		parseMode = PARSEMODE_KEY;
	};
	//
	auto lc = '\0';
	for (; i < initializerString.size(); i++) {
		auto c = initializerString[i];
		auto nc = i < initializerString.size() - 1?initializerString[i + 1]:'\0';
		// quotes
		if (curlyBracketCount == 1 && squareBracketCount == 0 && (c == '"' || c == '\'') && lc != '\\') {
			// we have a new quote here
			if (quote == '\0') {
				quote = c;
				// key?
				if (parseMode == PARSEMODE_KEY) {
					quotedMapKeyStart = i;
					mapKeyStart = string::npos;
				} else
				// value
				if (parseMode == PARSEMODE_VALUE) {
					quotedMapValueStart = i;
					mapValueStart = string::npos;
				}
			} else
			// finish the quote
			if (quote == c) {
				quote = '\0';
				// key?
				if (parseMode == PARSEMODE_KEY) {
					quotedMapKeyEnd = i;
				} else
				// value
				if (parseMode == PARSEMODE_VALUE) {
					quotedMapValueEnd = i;
				}
			}
		} else
		// no quote
		if (quote == '\0') {
			if (curlyBracketCount == 1 && squareBracketCount == 0 && bracketCount == 0 && c == ':' && nc != ':' && lc != ':' && lc != '\\') {
				//
				if (quotedMapKeyStart != string::npos) {
					quotedMapKeyEnd = i - 1;
				} else
				if (mapKeyStart != string::npos) {
					mapKeyEnd = i - 1;
				}
				//
				parseMode = PARSEMODE_VALUE;
				//
			} else
			// , -> insert map
			if (curlyBracketCount == 1 && squareBracketCount == 0 && bracketCount == 0 && c == ',') {
				if (mapValueStart != string::npos) {
					mapValueEnd = i - 1;
				} else
				if (mapKeyStart != string::npos && mapValueStart == string::npos && quotedMapValueStart == string::npos) {
					mapKeyEnd = i - 1;
				}
				// insert map key value pair
				insertMapKeyValuePair();
				// nada
			} else
			// possible function call
			if (c == '(') {
				//
				bracketCount++;
				//
				if (bracketCount == 1) inlineFunctionSignatureStart = i;
			} else
			if (c == ')') {
				bracketCount--;
				// function assignment
				if (inlineFunctionSignatureStart != string::npos && bracketCount == 0 && mapValueStart == string::npos) mapValueStart = inlineFunctionSignatureStart;
				//
				inlineFunctionSignatureStart = string::npos;
			} else
			// map/set initializer
			if (c == '{' && squareBracketCount == 0 && bracketCount == 0) {
				// increase square bracket count
				curlyBracketCount++;
				// we have a inner map/set initializer, mark it
				if (curlyBracketCount == 1) {
					mapKeyStart = i + 1;
				} else
				if (curlyBracketCount == 2) {
					if (mapValueStart == string::npos) mapValueStart = i;
				}
			} else
			// end of map/set initializer
			if (c == '}' && squareBracketCount == 0 && bracketCount == 0) {
				//
				curlyBracketCount--;
				// done? insert into map
				if (curlyBracketCount == 0) {
					//
					if (mapValueStart != string::npos) {
						mapValueEnd = i - 1;
					} else
					if (mapKeyStart != string::npos) {
						mapKeyEnd = i - 1;
					}
					// insert map key value pair
					insertMapKeyValuePair();
				} else
				// otherwise push inner array initializer
				if (curlyBracketCount == 1) {
					// parse and insert into map
					string_view mapKey;
					// quoted map key
					if (quotedMapKeyStart != string::npos) {
						quotedMapKeyStart++;
						auto mapKeyLength = quotedMapKeyEnd - quotedMapKeyStart;
						if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[quotedMapKeyStart], mapKeyLength));
					} else
					// unquoted map key
					if (mapKeyStart != string::npos) {
						if (mapKeyEnd == string::npos) mapKeyEnd = i;
						auto mapKeyLength = mapKeyEnd - mapKeyStart + 1;
						if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[mapKeyStart], mapKeyLength));
					}
					// validate map key
					if (mapKey.empty() == true || viewIsKey(mapKey) == false) {
						_Console::printLine(miniScript->getStatementInformation(statement) + ": invalid key name, ignoring map entry: " + string(mapKey));
					} else {
						//
						quotedMapKeyStart = string::npos;
						quotedMapKeyEnd = string::npos;
						mapKeyStart = string::npos;
						mapKeyEnd = string::npos;
						inlineFunctionSignatureStart = string::npos;

						// map value
						if (mapValueStart != string::npos) {
							mapValueEnd = i;
							auto mapValueLength = mapValueEnd - mapValueStart + 1;
							if (mapValueLength > 0) {
								auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[mapValueStart], mapValueLength));
								if (mapValueStringView.empty() == false) {
									//
									vector<string_view> arguments;
									string_view functionScriptCodeStringView;
									if (viewIsLamdaFunction(mapValueStringView, arguments, functionScriptCodeStringView) == true) {
										Variable mapValue;
										miniScript->createLamdaFunction(mapValue, arguments, functionScriptCodeStringView, true, statement);
										variable.setMapEntry(string(mapKey), mapValue);
									} else {
										// map/set
										auto mapValue = initializeMapSet(mapValueStringView, miniScript, scriptIdx, statement);
										variable.setMapEntry(string(mapKey), mapValue);
									}
								}
							}
							//
							mapValueStart = string::npos;
							mapValueEnd = string::npos;
						}
					}
					//
					parseMode = PARSEMODE_KEY;
				}
			} else
			// array initializer
			if (c == '[' && curlyBracketCount == 1 && bracketCount == 0) {
				// we have a inner array initializer, mark it
				if (squareBracketCount == 0) mapValueStart = i;
				// increase square bracket count
				squareBracketCount++;
			} else
			// end of array initializer
			if (c == ']' && curlyBracketCount == 1 && bracketCount == 0) {
				squareBracketCount--;
				// otherwise push inner array initializer
				if (squareBracketCount == 0) {
					// parse and insert into map
					string_view mapKey;

					// quoted map key
					if (quotedMapKeyStart != string::npos) {
						quotedMapKeyStart++;
						auto mapKeyLength = quotedMapKeyEnd - quotedMapKeyStart;
						if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[quotedMapKeyStart], mapKeyLength));
					} else
					// unquoted map key
					if (mapKeyStart != string::npos) {
						auto mapKeyLength = mapKeyEnd - mapKeyStart + 1;
						if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[mapKeyStart], mapKeyLength));
					}
					// validate map key
					if (mapKey.empty() == true || viewIsKey(mapKey) == false) {
						_Console::printLine(miniScript->getStatementInformation(statement) + ": invalid key name, ignoring map entry: " + string(mapKey));
					} else {
						//
						quotedMapKeyStart = string::npos;
						quotedMapKeyEnd = string::npos;
						mapKeyStart = string::npos;
						mapKeyEnd = string::npos;
						inlineFunctionSignatureStart = string::npos;

						// map value
						if (mapValueStart != string::npos) {
							mapValueEnd = i;
							auto mapValueLength = mapValueEnd - mapValueStart + 1;
							if (mapValueLength > 0) {
								auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[mapValueStart], mapValueLength));
								if (mapValueStringView.empty() == false) {
									auto mapValue = initializeArray(mapValueStringView, miniScript, scriptIdx, statement);
									variable.setMapEntry(string(mapKey), mapValue);
								}
							}
							//
							mapValueStart = string::npos;
							mapValueEnd = string::npos;
						}
					}
					//
					parseMode = PARSEMODE_KEY;
				}
			} else
			// set up map key  start
			if (curlyBracketCount == 1 && squareBracketCount == 0 && bracketCount == 0 && c != ' ' && c != '\t' && c != '\n') {
				if (parseMode == PARSEMODE_KEY && mapKeyStart == string::npos && quotedMapKeyStart == string::npos) {
					mapKeyStart = i;
				} else
				if (parseMode == PARSEMODE_VALUE && mapValueStart == string::npos && quotedMapValueStart == string::npos) {
					mapValueStart = i;
				}
			}
		}
		//
		lc = c;
	}
	// convert to set if no values given
	if (hasValues == false) {
		Variable setVariable;
		setVariable.setType(TYPE_SET);
		const auto& mapValueReference = variable.getMapValueReference();
		for (const auto& [mapVariableKey, mapVariableValue]: mapValueReference) {
			setVariable.insertSetKey(mapVariableKey);
		}
		variable = setVariable;
	}
	//
	auto initalizer = make_unique<MiniScript::Variable::Initializer>(string(initializerString), statement, nullptr);
	variable.ir.initializer->copy(initalizer.get());
	//
	return variable;
}

void MiniScript::Variable::setFunctionCallStatement(const string& initializerStatement, MiniScript* miniScript, int scriptIdx, const Statement& statement) {
	setType(TYPE_FUNCTION_CALL);
	getStringValueReference().setValue(initializerStatement);
	//
	Statement initializerScriptStatement(
		statement.line,
		statement.statementIdx,
		initializerStatement,
		initializerStatement,
		MiniScript::STATEMENTIDX_NONE
	);
	//
	string_view methodName;
	vector<string_view> arguments;
	string accessObjectMemberStatement;
	SyntaxTreeNode* evaluateSyntaxTree = new SyntaxTreeNode();
	if (miniScript->parseStatement(initializerStatement, methodName, arguments, initializerScriptStatement, accessObjectMemberStatement) == false) {
		//
	} else
	if (miniScript->createStatementSyntaxTree(scriptIdx, methodName, arguments, initializerScriptStatement, *evaluateSyntaxTree) == false) {
		//
	} else {
		getInitializerReference() = new Initializer(initializerStatement, statement, evaluateSyntaxTree);
	}
}

inline MiniScript::Variable* MiniScript::evaluateVariableAccessIntern(MiniScript::Variable* variablePtr, const string& variableStatement, const string& callerMethod, Variable*& parentVariable, int64_t& arrayIdx, string& key, int& setAccessBool, const Statement* statement, bool expectVariable) {
	// get root variable
	key.clear();
	// no array idx by default
	arrayIdx = ARRAYIDX_NONE;
	// determine left and right access operator position if there are any
	auto accessOperatorLeftIdx = string::npos;
	auto accessOperatorRightIdx = string::npos;
	if (getVariableAccessOperatorLeftRightIndices(variableStatement, callerMethod, accessOperatorLeftIdx, accessOperatorRightIdx, statement) == false) {
		return nullptr;
	}
	// access operator, if we have any, evaluate the array index
	auto haveAccessOperator = accessOperatorLeftIdx != string::npos && accessOperatorRightIdx != string::npos;
	if (haveAccessOperator == true &&
		evaluateAccess(variableStatement, callerMethod, accessOperatorLeftIdx, accessOperatorRightIdx, arrayIdx, key, statement) == false) {
		return nullptr;
	}
	//
	// get pointer to children variable
	if (haveAccessOperator == false) {
		//
		return variablePtr;
	} else {
		// resolve first parsed access pattern and repeat until resolved
		while (haveAccessOperator == true) {
			// map key access
			if (key.empty() == false) {
				if (variablePtr->getType() == TYPE_MAP) {
					//
					auto& mapValueReference = variablePtr->getMapValueReference();
					// key
					auto mapIt = mapValueReference.find(key);
					if (mapIt != mapValueReference.end()) {
						//
						parentVariable = variablePtr;
						//
						variablePtr = mapIt->second;
					} else {
						if (expectVariable == true) {
							_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": key not found: '" + key + "'");
						}
						// we have our parent
						parentVariable = variablePtr;
						//
						return nullptr;
					}
				} else
				if (variablePtr->getType() == TYPE_SET) {
					//
					auto& setValueReference = variablePtr->getSetValueReference();
					// key
					auto setIt = setValueReference.find(key);
					if (setIt != setValueReference.end()) {
						//
						setAccessBool = SETACCESSBOOL_TRUE;
						//
						parentVariable = variablePtr;
					} else {
						//
						setAccessBool = SETACCESSBOOL_FALSE;
						// we have our parent
						parentVariable = variablePtr;
						//
						return nullptr;
					}
				} else {
					_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": map/set access operator, but variable is not of type map/set");
					return nullptr;
				}
			} else
			if (variablePtr->getType() == TYPE_ARRAY) {
				// otherwise array
				if (arrayIdx == ARRAYIDX_ADD) {
					// we have our parent
					parentVariable = variablePtr;
					//
					return nullptr;
				} else
				if (arrayIdx >= ARRAYIDX_FIRST && arrayIdx < variablePtr->getArrayValueReference().size()) {
					//
					parentVariable = variablePtr;
					//
					variablePtr = variablePtr->getArrayValueReference()[arrayIdx];
				} else {
					_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": index out of bounds: 0 <= " + to_string(arrayIdx) + " < " + to_string(variablePtr->getArrayValueReference().size()));
					return nullptr;
				}
			} else {
				_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": access operator, expected array, but got: " + variablePtr->getValueAsString());
				return nullptr;
			}

			//
			auto accessOperatorStartIdx = accessOperatorRightIdx;
			accessOperatorLeftIdx = string::npos;
			accessOperatorRightIdx = string::npos;
			if (getVariableAccessOperatorLeftRightIndices(variableStatement, callerMethod, accessOperatorLeftIdx, accessOperatorRightIdx, statement, accessOperatorStartIdx) == false) {
				// fail
				return nullptr;
			}

			// do we have a next array access next to previous one?
			haveAccessOperator = accessOperatorLeftIdx != string::npos && accessOperatorRightIdx != string::npos;
			if (haveAccessOperator == false) {
				return variablePtr;
			} else {
				// yep, evaluate it
				if (evaluateAccess(variableStatement, callerMethod, accessOperatorLeftIdx, accessOperatorRightIdx, arrayIdx, key, statement) == false) {
					return nullptr;
				}
			}
		}
		//
		return variablePtr;
	}
}

inline MiniScript::Variable* MiniScript::getVariableIntern(const string& variableStatement, const string& callerMethod, string& variableName, Variable*& parentVariable, int64_t& arrayIdx, string& key, int& setAccessBool, const Statement* statement, bool expectVariable, bool global) {
	// determine variable name
	{
		auto dotIdx = _StringTools::indexOf(variableStatement, ".");
		if (dotIdx == string::npos) dotIdx = variableStatement.size();
		auto squareBracketIdx = _StringTools::indexOf(variableStatement, "[");
		if (squareBracketIdx == string::npos) squareBracketIdx = variableStatement.size();
		variableName = _StringTools::substring(variableStatement, 0, dotIdx < squareBracketIdx?dotIdx:squareBracketIdx);
	}
	// retrieve variable from script state
	Variable* variablePtr = nullptr;
	const auto& scriptState = global == false?getScriptState():getRootScriptState();
	auto variableIt = scriptState.variables.find(variableName);
	if (variableIt == scriptState.variables.end()) {
		if (expectVariable == true) {
			_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + " does not exist");
		}
		return nullptr;
	} else {
		variablePtr = variableIt->second;
	}
	//
	return evaluateVariableAccessIntern(variablePtr, variableStatement, callerMethod, parentVariable, arrayIdx, key, setAccessBool, statement, expectVariable);
}

inline void MiniScript::setVariableInternal(const string& variableStatement, Variable* parentVariable, Variable* variablePtr, int64_t arrayIdx, const string& key, const Variable& variable, const Statement* statement, bool createReference) {
	// common case
	if (variablePtr != nullptr) {
		if (variablePtr->isConstant() == false) {
			if (createReference == true) {
				variablePtr->setReference(&variable);
			} else {
				variablePtr->setValue(variable);
			}
		} else {
			_Console::printLine(getStatementInformation(*statement) + ": constant: " + variableStatement + ": Assignment of constant is not allowed");
		}
		return;
	} else
	// array add operator
	if (key.empty() == false) {
		if (parentVariable == nullptr) {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": map access operator without map: '" + key + "'");
		} else
		// all checks passed, push to map
		if (parentVariable->getType() == MiniScript::TYPE_MAP) {
			// check if our parent is not a const variable
			if (parentVariable->isConstant() == false) {
				parentVariable->setMapEntry(key, createReference == false?Variable::createNonReferenceVariable(&variable):Variable::createReferenceVariable(&variable));
			} else {
				_Console::printLine(getStatementInformation(*statement) + ": constant: " + variableStatement + ": Assignment of constant is not allowed");
			}
		} else
		if (parentVariable->getType() == MiniScript::TYPE_SET) {
			// check if our parent is not a const variable
			if (parentVariable->isConstant() == false) {
				bool booleanValue;
				if (variable.getBooleanValue(booleanValue, false) == true) {
					if (booleanValue == true) {
						parentVariable->insertSetKey(key);
					} else {
						parentVariable->removeSetKey(key);
					}
				} else {
					string callerMethod = __FUNCTION__;
					_Console::printLine("MiniScript::" + callerMethod + "(): '" + scriptFileName + "': variable: " + variableStatement + ": set access operator: expected boolean variable to remove/insert key in set, but got " + variable.getTypeAsString());
				}
			} else {
				_Console::printLine(getStatementInformation(*statement) + ": constant: " + variableStatement + ": Assignment of constant is not allowed");
			}
		} else {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": map/set access operator: expected map/set, but got " + parentVariable->getTypeAsString() + ": '" + key + "'");
		}
		//
		return;
	} else
	if (arrayIdx == ARRAYIDX_ADD) {
		if (parentVariable == nullptr) {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": [] array push operator without array");
		} else
		if (parentVariable->getType() == MiniScript::TYPE_ARRAY) {
			// check if our parent is not a const variable
			if (parentVariable->isConstant() == false) {
				// all checks passed, push variable to array
				parentVariable->pushArrayEntry(createReference == false?Variable::createNonReferenceVariable(&variable):Variable::createReferenceVariable(&variable));
			} else {
				_Console::printLine(getStatementInformation(*statement) + ": constant: " + variableStatement + ": Assignment of constant is not allowed");
			}
		} else {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": [] array push operator: expected array, but got " + parentVariable->getTypeAsString());
		}
		//
		return;
	}
}

inline bool MiniScript::evaluateInternal(const string& statement, const string& executableStatement, Variable& returnValue, bool pushScriptState) {
	Statement evaluateStatement(
		LINE_NONE,
		0,
		"internal.script.evaluate(" + statement + ")",
		"internal.script.evaluate(" + executableStatement + ")",
		STATEMENTIDX_NONE
	);
	auto scriptEvaluateStatement = "internal.script.evaluate(" + executableStatement + ")";
	//
	string_view methodName;
	vector<string_view> arguments;
	string accessObjectMemberStatement;
	SyntaxTreeNode evaluateSyntaxTree;
	if (parseStatement(scriptEvaluateStatement, methodName, arguments, evaluateStatement, accessObjectMemberStatement) == false) {
		return false;
	} else
	if (createStatementSyntaxTree(SCRIPTIDX_NONE, methodName, arguments, evaluateStatement, evaluateSyntaxTree) == false) {
		return false;
	} else {
		//
		if (pushScriptState == true) {
			this->pushScriptState();
			resetScriptExecutationState(SCRIPTIDX_NONE, STATEMACHINESTATE_NEXT_STATEMENT);
		}
		getScriptState().running = true;
		//
		returnValue = executeStatement(
			evaluateSyntaxTree,
			evaluateStatement
		);
		//
		if (pushScriptState == true) popScriptState();
		//
		return true;
	}
}

inline const MiniScript::Variable MiniScript::initializeVariable(const Variable& variable) {
	switch (variable.getType()) {
		case TYPE_ARRAY:
			{
				Variable arrayVariable;
				//
				arrayVariable.setType(TYPE_ARRAY);
				auto arrayPointer = variable.getArrayPointer();
				if (arrayPointer == nullptr) break;
				for (const auto arrayEntry: *arrayPointer) {
					arrayVariable.pushArrayEntry(initializeVariable(*arrayEntry));
				}
				//
				return arrayVariable;
			}
		case TYPE_MAP:
			{
				Variable mapVariable;
				//
				auto mapPointer = variable.getMapPointer();
				if (mapPointer == nullptr) break;
				for (const auto& [mapKey, mapValue]: *mapPointer) {
					mapVariable.setMapEntry(mapKey, initializeVariable(*mapValue));
				}
				//
				return mapVariable;
			}
		case TYPE_FUNCTION_CALL:
			{
				return executeStatement(
					*variable.getInitializer()->getSyntaxTree(),
					variable.getInitializer()->getStatement()
				);
			}
		default: break;
	}
	//
	return variable;
}

inline bool MiniScript::viewIsKey(const string_view& candidate) {
	if (candidate.size() == 0) return false;
	if (candidate[0] == '$') return false;
	for (auto i = 0; i < candidate.size(); i++) {
		auto c = candidate[i];
		if (_Character::isAlphaNumeric(c) == false && c != '_') return false;
	}
	return true;
}

inline bool MiniScript::getVariableAccessOperatorLeftRightIndices(const string& variableStatement, const string& callerMethod, string::size_type& accessOperatorLeftIdx, string::size_type& accessOperatorRightIdx, const Statement* statement, int startIdx) {
	accessOperatorLeftIdx = string::npos;
	accessOperatorRightIdx = string::npos;
	auto haveKey = false;
	auto squareBracketsCount = 0;
	// improve me!
	if (startIdx > 0) {
		haveKey = variableStatement[startIdx - 1] == '.';
		if (haveKey == true) accessOperatorLeftIdx = startIdx - 1;
	}
	for (auto i = startIdx; i < variableStatement.length(); i++) {
		auto c = variableStatement[i];
		if (haveKey == true) {
			if (c == '.') {
				//
				accessOperatorRightIdx = i;
				//
				return true;
			} else
			if (c == '[') {
				//
				accessOperatorRightIdx = i;
				//
				return true;
			}
			if (c == ']') {
				_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": unexpected char: ']'");
				return false;
			}
		} else
		if (c == '.' && squareBracketsCount == 0) {
			haveKey = true;
			accessOperatorLeftIdx = i;
		} else
		if (c == '[') {
			if (squareBracketsCount == 0) accessOperatorLeftIdx = i;
			squareBracketsCount++;
		} else
		if (c == ']') {
			squareBracketsCount--;
			if (squareBracketsCount == 0) {
				//
				accessOperatorRightIdx = i + 1;
				//
				return true;
			} else
			if (squareBracketsCount < 0) {
				_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": unexpected char: ']'");
				return false;
			}
		}
	}
	//
	if (haveKey == true) accessOperatorRightIdx = variableStatement.size();
	//
	return true;
}

inline bool MiniScript::evaluateAccess(const string& variableStatement, const string& callerMethod, string::size_type& arrayAccessOperatorLeftIdx, string::size_type& arrayAccessOperatorRightIdx, int64_t& arrayIdx, string& key, const Statement* statement) {
	key.clear();
	arrayIdx = ARRAYIDX_NONE;
	// check for dot access
	if (variableStatement.data()[arrayAccessOperatorLeftIdx] == '.') {
		key = string(_StringTools::viewTrim(string_view(&variableStatement.data()[arrayAccessOperatorLeftIdx + 1], arrayAccessOperatorRightIdx - arrayAccessOperatorLeftIdx - 1)));
		return true;
	}
	// evaluate array index
	auto arrayIdxExpressionStringView = _StringTools::viewTrim(string_view(&variableStatement.data()[arrayAccessOperatorLeftIdx + 1], arrayAccessOperatorRightIdx - arrayAccessOperatorLeftIdx - 2));
	if (arrayIdxExpressionStringView.empty() == false) {
		// integer first for performance
		if (_Integer::viewIs(arrayIdxExpressionStringView) == true) {
			arrayIdx = _Integer::viewParse(arrayIdxExpressionStringView);
		} else {
			// TODO: as evaluate statement we also might need the expression that had not yet a preprocessor run for error messages and such
			Variable statementReturnValue;
			auto evaluateStatement = string(arrayIdxExpressionStringView);
			if (evaluateInternal(evaluateStatement, evaluateStatement, statementReturnValue, false) == false || statementReturnValue.getIntegerValue(arrayIdx, false) == false) {
				_Console::printLine("MiniScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": variable: " + variableStatement + ": failed to evaluate expression: '" + string(arrayIdxExpressionStringView) + "'");
				return false;
			}
		}
	} else {
		arrayIdx = ARRAYIDX_ADD;
	}
	//
	return true;
}

void MiniScript::setConstantInternal(Variable& variable) {
	variable.setConstant();
	switch (variable.getType()) {
		case TYPE_ARRAY:
			{
				auto arrayPointer = variable.getArrayPointer();
				if (arrayPointer == nullptr) break;
				for (const auto arrayEntry: *arrayPointer) {
					setConstant(*arrayEntry);
				}
				//
				break;
			}
		case TYPE_MAP:
			{
				auto mapPointer = variable.getMapPointer();
				if (mapPointer == nullptr) break;
				for (const auto& [mapKey, mapValue]: *mapPointer) {
					setConstant(*mapValue);
				}
				//
				break;
			}
		default:
			break;
	}
}

void MiniScript::garbageCollection() {
	auto garbageCollectionDataTypesIndicesCopy = garbageCollectionDataTypesIndices;
	for (auto index: garbageCollectionDataTypesIndicesCopy) {
		auto& garbageCollectionDataType = garbageCollectionDataTypes[index];
		garbageCollectionDataType.dataType->garbageCollection(garbageCollectionDataType.context);
	}
}
