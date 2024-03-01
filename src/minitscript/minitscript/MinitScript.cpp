#include <minitscript/minitscript/MinitScript.h>

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

#include <minitscript/minitscript/MinitScript.h>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/ApplicationMethods.h>
#include <minitscript/minitscript/ArrayMethods.h>
#include <minitscript/minitscript/BaseMethods.h>
#include <minitscript/minitscript/ByteArrayMethods.h>
#include <minitscript/minitscript/CryptographyMethods.h>
#include <minitscript/minitscript/ConsoleMethods.h>
#include <minitscript/minitscript/ContextMethods.h>
#include <minitscript/minitscript/FileSystemMethods.h>
#include <minitscript/minitscript/HTTPDownloadClientClass.h>
#include <minitscript/minitscript/JSONMethods.h>
#include <minitscript/minitscript/MapMethods.h>
#include <minitscript/minitscript/MathMethods.h>
#include <minitscript/minitscript/NetworkMethods.h>
#include <minitscript/minitscript/ScriptMethods.h>
#include <minitscript/minitscript/SetMethods.h>
#include <minitscript/minitscript/StringMethods.h>
#include <minitscript/minitscript/TimeMethods.h>
#include <minitscript/minitscript/XMLMethods.h>

#include <minitscript/math/Math.h>
#include <minitscript/os/filesystem/FileSystem.h>
#include <minitscript/os/threading/Thread.h>
#include <minitscript/utilities/Character.h>
#include <minitscript/utilities/Console.h>
#include <minitscript/utilities/Integer.h>
#include <minitscript/utilities/StringTokenizer.h>
#include <minitscript/utilities/StringTools.h>
#include <minitscript/utilities/SHA256.h>
#include <minitscript/utilities/Time.h>

using std::find;
using std::ifstream;
using std::initializer_list;
using std::make_pair;
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

using minitscript::minitscript::MinitScript;

using minitscript::minitscript::ApplicationMethods;
using minitscript::minitscript::ArrayMethods;
using minitscript::minitscript::BaseMethods;
using minitscript::minitscript::ByteArrayMethods;
using minitscript::minitscript::CryptographyMethods;
using minitscript::minitscript::ConsoleMethods;
using minitscript::minitscript::ContextMethods;
using minitscript::minitscript::FileSystemMethods;
using minitscript::minitscript::HTTPDownloadClientClass;
using minitscript::minitscript::JSONMethods;
using minitscript::minitscript::MapMethods;
using minitscript::minitscript::MathMethods;
using minitscript::minitscript::NetworkMethods;
using minitscript::minitscript::ScriptMethods;
using minitscript::minitscript::SetMethods;
using minitscript::minitscript::StringMethods;
using minitscript::minitscript::TimeMethods;
using minitscript::minitscript::XMLMethods;

// TODO: we can remove the _ here again, as MinitScript.cpp is not a transpilation unit anymore
using _Math = minitscript::math::Math;
using _FileSystem = minitscript::os::filesystem::FileSystem;
using _Character = minitscript::utilities::Character;
using _Console = minitscript::utilities::Console;
using _Integer = minitscript::utilities::Integer;
using _StringTokenizer = minitscript::utilities::StringTokenizer;
using _StringTools = minitscript::utilities::StringTools;
using _SHA256 = minitscript::utilities::SHA256;
using _Time = minitscript::utilities::Time;
using _Thread = minitscript::os::threading::Thread;

const string MinitScript::OPERATOR_CHARS = "+-!~/%<>=&^|";

vector<MinitScript::DataType*> MinitScript::dataTypes;
MinitScript::ShutdownRAII MinitScript::shutdownRAII(MinitScript::dataTypes);

const string MinitScript::METHOD_SCRIPTCALL = "script.call";
const string MinitScript::METHOD_SCRIPTCALLSTACKLET = "script.callStacklet";
const string MinitScript::METHOD_SCRIPTCALLBYINDEX = "script.callByIndex";
const string MinitScript::METHOD_SCRIPTCALLSTACKLETBYINDEX = "script.callStackletByIndex";

const string MinitScript::METHOD_ENABLENAMEDCONDITION = "script.enableNamedCondition";
const string MinitScript::METHOD_DISABLENAMEDCONDITION = "script.disableNamedCondition";

const string MinitScript::Variable::TYPENAME_NONE = "";
const string MinitScript::Variable::TYPENAME_NULL = "Null";
const string MinitScript::Variable::TYPENAME_BOOLEAN = "Boolean";
const string MinitScript::Variable::TYPENAME_INTEGER = "Integer";
const string MinitScript::Variable::TYPENAME_FLOAT = "Float";
const string MinitScript::Variable::TYPENAME_FUNCTION = "Function";
const string MinitScript::Variable::TYPENAME_STACKLET = "Stacklet";
const string MinitScript::Variable::TYPENAME_NUMBER = "Number";
const string MinitScript::Variable::TYPENAME_MIXED = "Mixed";
const string MinitScript::Variable::TYPENAME_STRING = "String";
const string MinitScript::Variable::TYPENAME_BYTEARRAY = "ByteArray";
const string MinitScript::Variable::TYPENAME_ARRAY = "Array";
const string MinitScript::Variable::TYPENAME_MAP = "Map";
const string MinitScript::Variable::TYPENAME_SET = "Set";

const vector<string> MinitScript::Method::CONTEXTFUNCTIONS_ALL = {};

void MinitScript::initialize() {
	//
	_Console::initialize();
	//
	registerDataType(new HTTPDownloadClientClass());
	//
	HTTPDownloadClientClass::initialize();
}

const string MinitScript::getBaseClassHeader() {
	return "minitscript/minitscript/MinitScript.h";
}

const string MinitScript::getBaseClass() {
	return "minitscript::minitscript::MinitScript";
}

const vector<string> MinitScript::getTranspilationUnits() {
	return {
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/ApplicationMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/ArrayMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/BaseMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/ByteArrayMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/ConsoleMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/ContextMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/CryptographyMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/FileSystemMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/HTTPDownloadClientClass.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/JSONMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/MapMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/MathMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/NetworkMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/ScriptMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/SetMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/StringMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/TimeMethods.cpp",
		MINITSCRIPT_DATA + "/src/minitscript/minitscript/XMLMethods.cpp"
	};
}

MinitScript::MinitScript():
	errorStatement(
		LINE_NONE,
		STATEMENTIDX_NONE,
		string(),
		string(),
		STATEMENTIDX_NONE
	) {
	//
	for (auto dataType: dataTypes) {
		if (dataType->isRequiringGarbageCollection() == false) continue;
		// create script context
		auto scriptContext = dataType->createScriptContext();
		scriptContext->setMinitScript(this);
		scriptContext->setIndex(garbageCollectionDataTypes.size());
		//
		garbageCollectionDataTypes.emplace_back(dataType,scriptContext);
		garbageCollectionScriptContextsByDataType[dataType->getType()] = scriptContext;
	}
	setNative(false);
	pushScriptState();
}

MinitScript::~MinitScript() {
	for (const auto& [methodName, method]: this->methods) delete method;
	for (const auto& [stateMachineStateId, stateMachineState]: this->stateMachineStates) delete stateMachineState;
	while (scriptStateStack.empty() == false) popScriptState();
	garbageCollection();
	for (auto& garbageCollectionDataType: garbageCollectionDataTypes) garbageCollectionDataType.dataType->deleteScriptContext(garbageCollectionDataType.context);
}

void MinitScript::registerStateMachineState(StateMachineState* state) {
	auto stateMachineStateIt = stateMachineStates.find(state->getId());
	if (stateMachineStateIt != stateMachineStates.end()) {
		_Console::printLine("MinitScript::registerStateMachineState(): " + scriptFileName + ": State with id + " + to_string(state->getId()) + ", name " + state->getName() + " already registered.");
		return;
	}
	stateMachineStates[state->getId()] = state;
}

void MinitScript::initializeNative() {
}

void MinitScript::complain(const string& methodName, const Statement& statement) {
	auto argumentsInformation = getArgumentsInformation(methodName);
	if (argumentsInformation.empty() == true) argumentsInformation = "None";
	_Console::printLine(methodName + "(): " + getStatementInformation(statement) + ": Argument mismatch: expected arguments: " + argumentsInformation);
	//
	errorMessage = "An method usage complain has occurred";
	errorStatement = statement;
}

void MinitScript::complain(const string& methodName, const Statement& statement, const string& message) {
	_Console::printLine(methodName + "(): " + getStatementInformation(statement) + ": " + message);
	//
	errorMessage = "An method usage complain with message has occurred: " + message;
	errorStatement = statement;
}

void MinitScript::registerMethod(Method* method) {
	auto methodsIt = methods.find(method->getMethodName());
	if (methodsIt != methods.end()) {
		_Console::printLine("MinitScript::registerMethod(): " + scriptFileName + ": Method with name " + method->getMethodName() + " already registered.");
		return;
	}
	methods[method->getMethodName()] = method;
}

void MinitScript::registerDataType(DataType* dataType) {
	dataType->setType(static_cast<VariableType>(TYPE_PSEUDO_DATATYPES + dataTypes.size()));
	dataTypes.push_back(dataType);
}

void MinitScript::executeNextStatement() {
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
	//
	if (scriptState.statementIdx == STATEMENTIDX_FIRST) emitted = false;
	//
	executeStatement(syntaxTree, statement);
	//
	if (emitted == true) return;
	//
	scriptState.statementIdx++;
	if (scriptState.statementIdx >= script.statements.size()) {
		scriptState.scriptIdx = SCRIPTIDX_NONE;
		scriptState.statementIdx = STATEMENTIDX_NONE;
		setScriptStateState(STATEMACHINESTATE_WAIT_FOR_CONDITION);
	}
}

bool MinitScript::parseStatement(const string_view& executableStatement, string_view& methodName, vector<ParserArgument>& arguments, const Statement& statement, string& accessObjectMemberStatement) {
	if (VERBOSE == true) _Console::printLine("MinitScript::parseStatement(): " + getStatementInformation(statement) + ": '" + string(executableStatement) + "'");
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
	auto argumentSubLineIdx = -1;
	auto lc  = '\0';
	//
	auto viewIsStringMethodAccess = [](const string_view& candidate) -> bool {
		if (candidate.empty() == true) return false;
		//
		auto i = 0;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++);
		if (i >= candidate.size()) return false;
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
	auto subLineIdx = 0;
	//
	for (auto i = executableStatementStartIdx; i < executableStatement.size(); i++) {
		auto c = executableStatement[i];
		// quotes
		if (squareBracketCount == 0 && curlyBracketCount == 0 && ((c == '"' || c == '\'') && lc != '\\')) {
			if (bracketCount == 1) {
				if (quote == '\0') {
					quotedArgumentStart = i;
					argumentSubLineIdx = subLineIdx;
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
						argumentSubLineIdx = subLineIdx;
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
					argumentSubLineIdx = subLineIdx;
				} else {
					argumentEnd = i;
				}
			}
		} else {
			// no quotes, handle \n
			if (c == '\n') {
				subLineIdx++;
			} else
			// (
			if (c == '(') {
				bracketCount++;
				if (bracketCount > 1) {
					if (argumentStart == string::npos) {
						argumentStart = i;
						argumentSubLineIdx = subLineIdx;
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
						if (argumentLength > 0) arguments.emplace_back(_StringTools::viewTrim(string_view(&executableStatement[quotedArgumentStart], argumentLength)), argumentSubLineIdx);
						quotedArgumentStart = string::npos;
						quotedArgumentEnd = string::npos;
					} else
					if (argumentStart != string::npos) {
						if (argumentEnd == string::npos) argumentEnd = i - 1;
						auto argumentLength = argumentEnd - argumentStart + 1;
						if (argumentLength > 0) arguments.emplace_back(_StringTools::viewTrim(string_view(&executableStatement[argumentStart], argumentLength)), argumentSubLineIdx);
						argumentStart = string::npos;
						argumentEnd = string::npos;
						argumentSubLineIdx = -1;
					}
				} else {
					if (argumentStart == string::npos) {
						argumentStart = i;
						argumentSubLineIdx = subLineIdx;
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
						argumentSubLineIdx = subLineIdx;
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
						argumentSubLineIdx = subLineIdx;
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
						argumentSubLineIdx = subLineIdx;
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
						argumentSubLineIdx = subLineIdx;
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
							if (argumentLength > 0) arguments.emplace_back(_StringTools::viewTrim(string_view(&executableStatement[quotedArgumentStart], argumentLength)), argumentSubLineIdx);
							quotedArgumentStart = string::npos;
							quotedArgumentEnd = string::npos;
							argumentSubLineIdx = -1;
						} else
						if (argumentStart != string::npos) {
							if (argumentEnd == string::npos) argumentEnd = i - 1;
							auto argumentLength = argumentEnd - argumentStart + 1;
							if (argumentLength > 0) arguments.emplace_back(_StringTools::viewTrim(string_view(&executableStatement[argumentStart], argumentEnd - argumentStart + 1)), argumentSubLineIdx);
							argumentStart = string::npos;
							argumentEnd = string::npos;
							argumentSubLineIdx = -1;
						}
					} else {
						if (argumentStart == string::npos) {
							argumentStart = i + 1;
							argumentSubLineIdx = subLineIdx;
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
							argumentSubLineIdx = subLineIdx;
						}
					} else {
						argumentEnd = i;
					}
				}
			}
		}
		//
		lc = lc == '\\' && c == '\\'?'\0':c;
	}

	// extract method name
	if (methodStart != string::npos && methodEnd != string::npos) {
		methodName = _StringTools::viewTrim(string_view(&executableStatement[methodStart], methodEnd - methodStart + 1));
	}

	// handle object member access and generate internal.script.evaluateMemberAccess call
	if (objectMemberAccess == true) {
		// construct executable statement and arguments
		string_view evaluateMemberAccessMethodName;
		vector<ParserArgument> evaluateMemberAccessArguments;

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
		evaluateMemberAccessArguments.emplace_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx), 0);
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= ", ";
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= objectMemberAccessObjectVariable == true?"null":string(objectMemberAccessObject);
		evaluateMemberAccessArguments.emplace_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx), 0);
		accessObjectMemberStatement+= ", ";
		idx = accessObjectMemberStatement.size();
		accessObjectMemberStatement+= "\"" + string(methodName) + "\"";
		evaluateMemberAccessArguments.emplace_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx), 0);
		for (const auto& argument: arguments) {
			auto argumentVariable = viewIsVariableAccess(argument.argument);
			accessObjectMemberStatement+= ", ";
			idx = accessObjectMemberStatement.size();
			accessObjectMemberStatement+= argumentVariable == true?"\"" + string(argument.argument) + "\"":"null";
			evaluateMemberAccessArguments.emplace_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx), 0);
			accessObjectMemberStatement+= ", ";
			idx = accessObjectMemberStatement.size();
			accessObjectMemberStatement+= argumentVariable == true?"null":string(argument.argument);
			evaluateMemberAccessArguments.emplace_back(string_view(&accessObjectMemberStatement.data()[idx], accessObjectMemberStatement.size() - idx), 0);
		}
		accessObjectMemberStatement+= ")";
		// set up new results
		methodName = evaluateMemberAccessMethodName;
		arguments = evaluateMemberAccessArguments;
	}

	//
	if (VERBOSE == true) {
		_Console::printLine("MinitScript::parseStatement(): " + _StringTools::replace(getStatementInformation(statement), "\n", "\\n"));
		_Console::printLine(string("\t") + ": Method: '" + string(methodName) + "'");
		_Console::printLine(string("\t") + ": Arguments");
		int variableIdx = 0;
		for (const auto& argument: arguments) {
			_Console::printLine(string("\t\t") + "@" + to_string(argument.subLineIdx) + "'" + _StringTools::replace(string(argument.argument), "\n", "\\n") + "'");
			variableIdx++;
		}
		_Console::printLine();
	}

	// complain about bracket count
	if (bracketCount != 0) {
		// TODO: sub line index
		_Console::printLine(getStatementInformation(statement) + ": " + string(executableStatement) + "': Unbalanced bracket count: " + to_string(_Math::abs(bracketCount)) + " " + (bracketCount < 0?"too much closed":"still open"));
		//
		parseErrors.push_back(string(executableStatement) + ": Unbalanced bracket count: " + to_string(_Math::abs(bracketCount)) + " " + (bracketCount < 0?"too much closed":"still open"));
		//
		return false;
	}
	// complain about square bracket count
	if (squareBracketCount != 0) {
		// TODO: sub line index
		_Console::printLine(getStatementInformation(statement) + ": " + string(executableStatement) + "': Unbalanced square bracket count: " + to_string(_Math::abs(squareBracketCount)) + " " + (squareBracketCount < 0?"too much closed":"still open"));
		//
		parseErrors.push_back(string(executableStatement) + ": Unbalanced square bracket count: " + to_string(_Math::abs(squareBracketCount)) + " " + (squareBracketCount < 0?"too much closed":"still open"));
		//
		return false;
	}
	// complain about curly bracket count
	if (curlyBracketCount != 0) {
		// TODO: sub line index
		_Console::printLine(getStatementInformation(statement) + ": " + string(executableStatement) + "': Unbalanced curly bracket count: " + to_string(_Math::abs(curlyBracketCount)) + " " + (curlyBracketCount < 0?"too much closed":"still open"));
		//
		parseErrors.push_back(string(executableStatement) + ": Unbalanced curly bracket count: " + to_string(_Math::abs(curlyBracketCount)) + " " + (curlyBracketCount < 0?"too much closed":"still open"));
		//
		return false;
	}

	//
	return true;
}

MinitScript::Variable MinitScript::executeStatement(const SyntaxTreeNode& syntaxTree, const Statement& statement) {
	if (VERBOSE == true) _Console::printLine("MinitScript::executeStatement(): " + getStatementInformation(statement) + "': " + syntaxTree.value.getValueAsString() + "(" + getArgumentsAsString(syntaxTree.arguments) + ")");
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
		_Console::printLine("MinitScript::executeStatement(): '" + getStatementInformation(statement) + ": " + syntaxTree.value.getValueAsString() + "(" + getArgumentsAsString(syntaxTree.arguments) + ")");
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
						": Method '" + string(syntaxTree.value.getValueAsString()) + "'" +
						": argument value @ " + to_string(argumentIdx) + ": expected " + Variable::getTypeAsString(argumentType.type) + ", but got: " + (argumentIdx < arguments.size()?arguments[argumentIdx].getAsString():"nothing"));
				}
				argumentIdx++;
			}
			if (method->isVariadic() == false && arguments.size() > method->getArgumentTypes().size()) {
				_Console::printLine(
					getStatementInformation(statement) +
					": Method '" + string(syntaxTree.value.getValueAsString()) + "'" +
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
		if (MinitScript::Variable::isExpectedType(returnValue.getType(), method->getReturnValueType()) == false) {
			_Console::printLine(
				getStatementInformation(statement) +
				": Method '" + string(syntaxTree.value.getValueAsString()) + "'" +
				": return value: expected " + Variable::getReturnTypeAsString(method->getReturnValueType(), method->isReturnValueNullable()) + ", but got: " + Variable::getReturnTypeAsString(returnValue.getType(), false));
		}
		//
		return returnValue;
	}
	//
	return returnValue;
}

bool MinitScript::createStatementSyntaxTree(int scriptIdx, const string_view& methodName, const vector<ParserArgument>& arguments, const Statement& statement, SyntaxTreeNode& syntaxTree, int subLineIdx) {
	//
	if (VERBOSE == true) {
		//
		auto getArgumentsAsString = [](const vector<ParserArgument>& arguments) -> const string {
			string argumentsString;
			for (const auto& argument: arguments) argumentsString+= (argumentsString.empty() == false?", ":"") + string("@") + to_string(argument.subLineIdx) + string("'") + string(argument.argument) + string("'");
			return argumentsString;
		};
		//
		_Console::printLine("MinitScript::createScriptStatementSyntaxTree(): " + getStatementInformation(statement) + ": " + string(methodName) + "(" + getArgumentsAsString(arguments) + ")");
	}
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
		int lamdaFunctionStackletLineIdx = statement.line + subLineIdx + argument.subLineIdx;
		if (viewIsLamdaFunction(argument.argument, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode, lamdaFunctionStackletLineIdx) == true) {
			Variable variable;
			createLamdaFunction(variable, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode, lamdaFunctionStackletLineIdx, false, statement);
			SyntaxTreeNode subSyntaxTree(SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL, variable, nullptr, {}, subLineIdx + argument.subLineIdx);
			syntaxTree.arguments.push_back(subSyntaxTree);
		} else
		if (viewIsStacklet(argument.argument, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode, lamdaFunctionStackletLineIdx) == true) {
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
			createStacklet(variable, scopeName, lamdaFunctionStackletArguments, lamdaFunctionStackletScriptCode, lamdaFunctionStackletLineIdx, statement);
			SyntaxTreeNode subSyntaxTree(SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL, variable, nullptr, {}, subLineIdx + argument.subLineIdx);
			syntaxTree.arguments.push_back(subSyntaxTree);
		} else
		if (getObjectMemberAccess(argument.argument, accessObjectMemberObject, accessObjectMemberMethod, accessObjectMemberStartIdx, statement) == true) {
			// method call
			string_view subMethodName;
			vector<ParserArgument> subArguments;
			string accessObjectMemberStatement;
			if (parseStatement(argument.argument, subMethodName, subArguments, statement, accessObjectMemberStatement) == true) {
				SyntaxTreeNode subSyntaxTree;
				if (createStatementSyntaxTree(scriptIdx, subMethodName, subArguments, statement, subSyntaxTree, subLineIdx + argument.subLineIdx) == false) {
					return false;
				}
				syntaxTree.arguments.push_back(subSyntaxTree);
			} else {
				return false;
			}
		} else
		// plain variable
		if (viewIsVariableAccess(argument.argument) == true) {
			//
			Variable value;
			value.setValue(deescape(argument.argument, statement));
			// look up getVariable method
			string methodName =
				argumentIdx >= argumentReferences.size() || argumentReferences[argumentIdx] == false?
					(method != nullptr?"getMethodArgumentVariable":"getVariable"):
					"getVariableReference";
			//
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
			//
			syntaxTree.arguments.emplace_back(
				SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD,
				MinitScript::Variable(deescape(methodName, statement)),
				method,
				initializer_list<SyntaxTreeNode>
					{
						{
							SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL,
							value,
							nullptr,
							{},
							subLineIdx + argument.subLineIdx
						}
					},
				subLineIdx + argument.subLineIdx
			);
		} else
		// method call
		if (argument.argument.empty() == false &&
			viewIsStringLiteral(argument.argument) == false &&
			viewIsInitializer(argument.argument) == false &&
			viewIsCall(argument.argument) == true) {
			// method call
			string_view subMethodName;
			vector<ParserArgument> subArguments;
			string accessObjectMemberStatement;
			if (parseStatement(argument.argument, subMethodName, subArguments, statement, accessObjectMemberStatement) == true) {
				SyntaxTreeNode subSyntaxTree;
				if (createStatementSyntaxTree(scriptIdx, subMethodName, subArguments, statement, subSyntaxTree, subLineIdx + argument.subLineIdx) == false) {
					return false;
				}
				syntaxTree.arguments.push_back(subSyntaxTree);
			} else {
				//
				return false;
			}
		} else {
			// implicitely literal
			Variable value;
			value.setImplicitTypedValueFromStringView(argument.argument, this, scriptIdx, statement);
			//
			syntaxTree.arguments.emplace_back(
				SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL,
				value,
				nullptr,
				initializer_list<SyntaxTreeNode>{},
				subLineIdx + argument.subLineIdx
			);
		}
		//
		argumentIdx++;
	}
	// try first user functions
	if (functionScriptIdx != SCRIPTIDX_NONE) {
		syntaxTree.type = scripts[functionScriptIdx].type == Script::TYPE_FUNCTION?SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_FUNCTION:SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_STACKLET;
		syntaxTree.value.setValue(deescape(methodName, statement));
		syntaxTree.subLineIdx = subLineIdx;
		syntaxTree.setScriptIdx(functionScriptIdx);
		//
		return true;
	} else
	// try methods next
	if (method != nullptr) {
		syntaxTree.type = SyntaxTreeNode::SCRIPTSYNTAXTREENODE_EXECUTE_METHOD;
		syntaxTree.value.setValue(deescape(methodName, statement));
		syntaxTree.subLineIdx = subLineIdx;
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

bool MinitScript::setupFunctionAndStackletScriptIndices(int scriptIdx) {
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

bool MinitScript::setupFunctionAndStackletScriptIndices(SyntaxTreeNode& syntaxTreeNode, const Statement& statement) {
	switch (syntaxTreeNode.type) {
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
			{
				switch(syntaxTreeNode.value.getType()) {
					case(MinitScript::TYPE_ARRAY):
					case(MinitScript::TYPE_MAP):
						{
							if (setupFunctionAndStackletScriptIndices(syntaxTreeNode.value, statement) == false) return false;
							//
							break;
						}
					case(MinitScript::TYPE_FUNCTION_ASSIGNMENT):
						{
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
							//
							break;
						}
					case(MinitScript::TYPE_STACKLET_ASSIGNMENT):
						{
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
							//
							break;
						}
					default:
						break;
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

bool MinitScript::setupFunctionAndStackletScriptIndices(Variable& variable, const Statement& statement) {
	switch (variable.getType()) {
		case TYPE_ARRAY:
			{
				auto arrayPointer = variable.getArrayPointer();
				if (arrayPointer == nullptr) break;
				for (auto arrayEntry: *arrayPointer) {
					if (setupFunctionAndStackletScriptIndices(*arrayEntry, statement) == false) return false;
				}
				//
				break;
			}
		case TYPE_MAP:
			{
				//
				auto mapPointer = variable.getMapPointer();
				if (mapPointer == nullptr) break;
				for (auto& [mapKey, mapValue]: *mapPointer) {
					if (setupFunctionAndStackletScriptIndices(*mapValue, statement) == false) return false;
				}
				//
				break;
			}
		case TYPE_FUNCTION_ASSIGNMENT:
			{
				string function;
				auto functionScriptIdx = SCRIPTIDX_NONE;
				if (variable.getFunctionValue(function, functionScriptIdx) == false ||
					(functionScriptIdx = getFunctionScriptIdx(function)) == SCRIPTIDX_NONE) {
					//
					_Console::printLine(
						getStatementInformation(statement) +
						": Function not found: " +
						variable.getValueAsString()
					);
					//
					parseErrors.push_back(
						getStatementInformation(statement) +
						": Function not found: " +
						variable.getValueAsString()
					);
					//
					return false;
				}
				//
				variable.setFunctionAssignment(function, functionScriptIdx);
				//
				break;
			}
		case TYPE_STACKLET_ASSIGNMENT:
			{
				string stacklet;
				auto stackletScriptIdx = SCRIPTIDX_NONE;
				if (variable.getStackletValue(stacklet, stackletScriptIdx) == false ||
					(stackletScriptIdx = getFunctionScriptIdx(stacklet)) == SCRIPTIDX_NONE) {
					//
					_Console::printLine(
						getStatementInformation(statement) +
						": Stacklet not found" +
						variable.getValueAsString()
					);
					//
					parseErrors.push_back(
						getStatementInformation(statement) +
						": Stacklet not found: " +
						variable.getValueAsString()
					);
					//
					return false;
				}
				//
				variable.setStackletAssignment(stacklet, stackletScriptIdx);
				//
				break;
			}
		default: break;
	}
	//
	return true;
}

int MinitScript::getStackletScopeScriptIdx(int scriptIdx) {
	if (scriptIdx < 0 || scriptIdx >= scripts.size() ||
		scripts[scriptIdx].type != MinitScript::Script::TYPE_STACKLET) {
		return MinitScript::SCRIPTIDX_NONE;
	}
	//
	const auto& stackletScript = scripts[scriptIdx];
	const auto& stackletScopeName = stackletScript.arguments.size() == 1?stackletScript.arguments[0].name:string();
	if (stackletScopeName.empty() == true) {
		return MinitScript::SCRIPTIDX_NONE;
	}
	//
	for (auto i = 0; i < scripts.size(); i++) {
		if (i == scriptIdx) continue;
		const auto& scriptCandidate = scripts[i];
		if (scriptCandidate.type != MinitScript::Script::TYPE_FUNCTION && scriptCandidate.type != MinitScript::Script::TYPE_STACKLET) continue;
		if (scriptCandidate.condition == stackletScopeName) {
			if (scriptCandidate.type == MinitScript::Script::TYPE_STACKLET) return getStackletScopeScriptIdx(i); else return i;
		}
	}
	//
	return MinitScript::SCRIPTIDX_NONE;
}

bool MinitScript::validateStacklets(int scriptIdx) {
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

bool MinitScript::validateStacklets(const string& function, int scopeScriptIdx) {
	auto functionScriptIdx = getFunctionScriptIdx(function);
	if (functionScriptIdx == SCRIPTIDX_NONE) {
		_Console::printLine("MinitScript::validateStacklet(): function not found: " + function);
		return false;
	}
	//
	const auto& script = scripts[functionScriptIdx];
	auto statementIdx = STATEMENTIDX_FIRST;
	//
	for (const auto& syntaxTreeNode: script.syntaxTree) {
		const auto& statement = script.statements[statementIdx++];
		//
		if (validateStacklets(scopeScriptIdx == MinitScript::SCRIPTIDX_NONE?functionScriptIdx:scopeScriptIdx, syntaxTreeNode, statement) == false) {
			//
			return false;
		}
	}
	//
	return true;
}

bool MinitScript::validateStacklets(int scopeScriptIdx, const SyntaxTreeNode& syntaxTreeNode, const Statement& statement) {
	switch (syntaxTreeNode.type) {
		case SyntaxTreeNode::SCRIPTSYNTAXTREENODE_LITERAL:
			{
				// TODO: improve me! This is actually litaral only, which can be also set as variable and be reused later
				//	basically we forbid here to create a stacklet assignment variable with wrong scope in a given scope
				if (syntaxTreeNode.value.getType() == MinitScript::TYPE_STACKLET_ASSIGNMENT) {
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

bool MinitScript::validateCallable(const string& function) {
	auto functionScriptIdx = getFunctionScriptIdx(function);
	if (functionScriptIdx == SCRIPTIDX_NONE) {
		_Console::printLine("MinitScript::validateCallable(): function not found: " + function);
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

bool MinitScript::validateCallable(const SyntaxTreeNode& syntaxTreeNode, const Statement& statement) {
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
						": Method " +
						syntaxTreeNode.getMethod()->getMethodName() + "() can not be called within a callable function"
					);
					//
					parseErrors.push_back(
						getStatementInformation(statement) +
						": Method " +
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

bool MinitScript::validateContextFunctions(const string& function, vector<string>& functionStack) {
	auto functionScriptIdx = getFunctionScriptIdx(function);
	if (functionScriptIdx == SCRIPTIDX_NONE) {
		_Console::printLine("MinitScript::validateContextFunctions(): Function not found: " + function);
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

bool MinitScript::validateContextFunctions(const SyntaxTreeNode& syntaxTreeNode, vector<string>& functionStack, const Statement& statement) {
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
							": Method " +
							syntaxTreeNode.getMethod()->getMethodName() + "() can only be called within the following functions: " +
							contextFunctionsString +
							", but was called from " +
							functionStackFunction + "()"
						);
						//
						parseErrors.push_back(
							getStatementInformation(statement) +
							": Method " +
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

void MinitScript::emit(const string& condition) {
	// defer emit if a function/stacklet is still running
	if (isFunctionRunning() == true) {
		deferredEmit = condition;
		return;
	}
	//
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
	//
	emitted = true;
}

void MinitScript::executeStateMachine() {
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

void MinitScript::execute() {
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

bool MinitScript::getNextStatement(const string& scriptCode, int& i, int& line, string& statement) {
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
	auto expectRightArgument = false;
	auto lc = '\0';
	auto llc = '\0';
	for (; i < scriptCode.size(); i++) {
		auto c = scriptCode[i];
		auto nc = i + 1 < scriptCode.size()?scriptCode[i + 1]:'\0';
		// this is some sort of hack, but it works, we need a more sophisticated parser later
		if (c != '-' && c != '>' &&
			c != ' ' && c != '\t' && c != '\n' && c != '\r') canExpectStacklet = c == ',' || c == '(';
		// handle quotes
		if (hash == true && c != '\n') {
			// no op
		} else
		if ((c == '"' || c == '\'') && lc != '\\') {
			if (quote == '\0') {
				quote = c;
			} else
			if (quote == c) {
				expectRightArgument = false;
				quote = '\0';
			}
			// add char to script line
			statementCodeLines.back() += c;
			//
			inlineFunctionArguments = false;
		} else
		if (quote != '\0') {
			// no op
			if (c == '\n') {
				_Console::printLine(scriptFileName + ":" + to_string(line) + ": Newline within string literal is not allowed");
				parseErrors.push_back(scriptFileName + ":" + to_string(line) + ": Newline within string literal is not allowed");
				//
				return false;
			} else {
				statementCodeLines.back() += c;
			}
		} else
		// brackets
		if (c == '(') {
			inlineFunctionArguments = false;
			expectRightArgument = false;
			//
			bracketCount++;
			expectBracket = false;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (c == ')') {
			//
			inlineFunctionArguments = false;
			expectRightArgument = false;
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
			expectRightArgument = false;
			//
			squareBracketCount++;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (c == ']') {
			//
			inlineFunctionArguments = false;
			expectRightArgument = false;
			//
			squareBracketCount--;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		// curly brackets
		if (c == '{') {
			//
			inlineFunctionArguments = false;
			expectRightArgument = false;
			//
			curlyBracketCount++;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (c == '}') {
			//
			inlineFunctionArguments = false;
			expectRightArgument = false;
			//
			curlyBracketCount--;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		// hash
		if (c == '#' && curlyBracketCount == 0 && squareBracketCount == 0) {
			// hash
			hash = true;
		} else
		// new line
		if (c == '\r') {
			// ignore
		} else
		// assigment operator
		if (// TODO: implement those prefix/postfix operators properly, for now they get ignored
			//(lc == '+' && c == '+') ||
			//(lc == '-' && c == '-') ||
			//(lc == '+' && c == '+') ||
			//(lc == '-' && c == '-') ||
			(isOperatorChar(lc) == false && c == '!' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '~' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '*' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '/' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '%' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '+' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '-' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '<' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '>' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '&' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '^' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '|' && isOperatorChar(nc) == false) ||
			(isOperatorChar(lc) == false && c == '=' && isOperatorChar(nc) == false) ||
			(isOperatorChar(llc) == false && lc == '<' && c == '=' && isOperatorChar(nc) == false) ||
			(isOperatorChar(llc) == false && lc == '>' && c == '=' && isOperatorChar(nc) == false) ||
			(isOperatorChar(llc) == false && lc == '=' && c == '=' && isOperatorChar(nc) == false) ||
			(isOperatorChar(llc) == false && lc == '!' && c == '=' && isOperatorChar(nc) == false) ||
			(isOperatorChar(llc) == false && lc == '&' && c == '&' && isOperatorChar(nc) == false) ||
			(isOperatorChar(llc) == false && lc == '|' && c == '|' && isOperatorChar(nc) == false)
		) {
			if (expectBracket == false && expectRightArgument == false && bracketCount == 0 && squareBracketCount == 0 && curlyBracketCount == 0) {
				expectRightArgument = true;
			}
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if (lc == '-' && c == '>') {
			// we expect a bracket now for object->xyz() member method call, if we have a possible identifier
			if (inlineFunctionArguments == false && canExpectStacklet == false) expectBracket = true;
			//
			expectRightArgument = false;
			// add char to script line
			statementCodeLines.back() += c;
		} else
		if ((c == '\n' && ++line) || (hash == false && c == ';')) {
			// break here and process script line
			if (expectBracket == false && expectRightArgument == false && bracketCount == 0 && squareBracketCount == 0 && curlyBracketCount == 0) break;
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
			//
			if (_Character::isSpace(c) == false) expectRightArgument = false;
		}
		//
		lc = lc == '\\' && c == '\\'?'\0':c;
		llc = lc;
	}

	//
	auto lineIdx = 0;
	for (const auto& line: statementCodeLines) {
		auto trimmedLine = _StringTools::trim(line);
		statementCode+= trimmedLine;
		if (statementCode.empty() == false && lineIdx != statementCodeLines.size() - 1) statementCode+= "\n";
		lineIdx++;
	}

	// add last line index
	if (i == scriptCode.size() && scriptCode.back() != '\n') ++line;

	// done
	statement = statementCode;

	//
	return true;
}

bool MinitScript::parseScriptInternal(const string& scriptCode, int lineIdxOffset) {
	//
	auto scriptCount = scripts.size();
	auto haveScript = false;
	auto lineIdx = LINE_FIRST;
	auto currentLineIdx = LINE_NONE;
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
		string statementCode;
		if (getNextStatement(scriptCode, i, lineIdx, statementCode) == false) {
			//
			scriptValid = false;
			return false;
		}

		// add last line index
		if (i == scriptCode.size() && scriptCode.back() != '\n') ++lineIdx;
		//
		if (statementCode.empty() == true) continue;

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
					string nextStatementCode;
					if (getNextStatement(scriptCode, i, lineIdx, nextStatementCode) == false) {
						//
						scriptValid = false;
						return false;
					}
					//
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
						string nextStatementCode;
						if (getNextStatement(scriptCode, i, lineIdx, nextStatementCode) == false) {
							//
							scriptValid = false;
							return false;
						}
						//
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
								lc = lc == '\\' && c == '\\'?'\0':c;
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
						string nextStatementCode;
						if (getNextStatement(scriptCode, i, lineIdx, nextStatementCode) == false) {
							//
							scriptValid = false;
							return false;
						}
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
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Unbalanced bracket count");
							parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Unbalanced bracket count");
							scriptValid = false;
							return false;
						} else
						if (rightBracketIdx == string::npos) {
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Unbalanced bracket count");
							parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Unbalanced bracket count");
							scriptValid = false;
							return false;
						} else {
							auto argumentNamesString = _StringTools::trim(_StringTools::substring(statement, leftBracketIdx + 1, rightBracketIdx));
							auto argumentNamesTokenized = _StringTools::tokenize(argumentNamesString, ",");
							statement = _StringTools::substring(statement, 0, leftBracketIdx);
							for (const auto& argumentName: argumentNamesTokenized) {
								auto argumentNameTrimmed = _StringTools::trim(argumentName);
								auto reference = false;
								auto privateScope = false;
								if (_StringTools::startsWith(argumentNameTrimmed, "&&") == true) {
									reference = true;
									privateScope = true;
									argumentNameTrimmed = _StringTools::trim(_StringTools::substring(argumentNameTrimmed, 2));
								} else
								if (_StringTools::startsWith(argumentNameTrimmed, "&") == true) {
									reference = true;
									argumentNameTrimmed = _StringTools::trim(_StringTools::substring(argumentNameTrimmed, 1));
								}
								if (scriptType == Script::TYPE_FUNCTION) {
									if (_StringTools::regexMatch(argumentNameTrimmed, "\\$[a-zA-Z0-9_]+") == true) {
										arguments.emplace_back(
											argumentNameTrimmed,
											reference,
											privateScope
										);
									} else {
										_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Invalid argument name: '" + argumentNameTrimmed + "'");
										parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Invalid argument name: '" + argumentNameTrimmed + "'");
										scriptValid = false;
										return false;

									}
								} else
								if (scriptType == Script::TYPE_STACKLET) {
									if (_StringTools::regexMatch(argumentNameTrimmed, "[a-zA-Z0-9_]+") == true) {
										arguments.emplace_back(
											argumentNameTrimmed,
											reference,
											false
										);
									} else {
										_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Invalid stacklet parent stacklet/function: '" + argumentNameTrimmed + "'");
										parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": " + (scriptType == Script::TYPE_FUNCTION?"function":"stacklet") + ": Invalid stacklet parent stacklet/function: '" + argumentNameTrimmed + "'");
										scriptValid = false;
										return false;
									}
								}
							}
						}

					}
				}
				auto trimmedStatement = _StringTools::trim(statement);
				Statement evaluateStatement(
					currentLineIdx + lineIdxOffset,
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
					currentLineIdx + lineIdxOffset,
					conditionOrName,
					conditionOrNameExecutable,
					Statement(currentLineIdx + lineIdxOffset, statementIdx, conditionOrName, conditionOrNameExecutable, STATEMENTIDX_NONE),
					SyntaxTreeNode(),
					name,
					emitCondition,
					initializer_list<Statement>{},
					initializer_list<SyntaxTreeNode>{},
					callable,
					arguments
				);
			} else {
				_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": expecting 'on:', 'on-enabled:', 'stacklet:', 'function:', 'callable:'");
				parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": expecting 'on:', 'on-enabled:', 'stacklet:', 'function:', 'callable:'");
				scriptValid = false;
				return false;
			}
		} else {
			//
			if (_StringTools::startsWith(statementCode, "function:") == true ||
				_StringTools::startsWith(statementCode, "stacklet:") == true ||
				_StringTools::startsWith(statementCode, "on:") == true ||
				_StringTools::startsWith(statementCode, "on-enabled:") == true ||
				_StringTools::startsWith(statementCode, "callable:") == true
			) {
				_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": Unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
				parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": Unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
				scriptValid = false;
				return false;
			} else {
				//
				auto regexStatementCode = _StringTools::replace(statementCode, "\n", " ");
				//
				if (statementCode == "end") {
					if (blockStack.empty() == false) {
						auto block = blockStack.back();
						blockStack.erase(blockStack.begin() + blockStack.size() - 1);
						switch(block.type) {
							case Block::TYPE_FOR:
							case Block::TYPE_FOREACH:
								{
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, block.statementIdx);
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
								}
								break;
							case Block::TYPE_IF:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_ELSE:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_ELSEIF:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_SWITCH:
								{
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_CASE:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size() + 1;
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_DEFAULT:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size() + 1;
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
						}
					} else{
						scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
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
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_ELSEIF:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, statementCode, STATEMENTIDX_NONE);
								}
								break;
							default:
								_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": else without if/elseif");
								parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": else without if/elseif");
								scriptValid = false;
								return false;
						}
						blockStack.emplace_back(
							Block::TYPE_ELSE,
							statementIdx
						);
					} else {
						_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": else without if");
						parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": else without if");
						scriptValid = false;
						return false;
					}
				} else
				if (_StringTools::regexMatch(regexStatementCode, "^elseif[\\s]*\\(.*\\)$") == true) {
					Statement elseIfStatement(
						currentLineIdx + lineIdxOffset,
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
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, executableStatement, STATEMENTIDX_NONE);
								}
								break;
							case Block::TYPE_ELSEIF:
								{
									scripts.back().statements[block.statementIdx].gotoStatementIdx = scripts.back().statements.size();
									scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx, statementCode, executableStatement, STATEMENTIDX_NONE);
								}
								break;
							default:
								_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": elseif without if");
								parseErrors.push_back(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": elseif without if");
								scriptValid = false;
								return false;
						}
						blockStack.emplace_back(
							Block::TYPE_ELSEIF,
							statementIdx
						);
					} else {
						_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": elseif without if");
						parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": elseif without if");
						scriptValid = false;
						return false;
					}
				} else {
					smatch matches;
					Statement generatedStatement(
						currentLineIdx + lineIdxOffset,
						STATEMENTIDX_FIRST,
						statementCode,
						statementCode,
						STATEMENTIDX_NONE
					);
					if (_StringTools::regexMatch(regexStatementCode, "^for[\\s]*\\(.*\\)$") == true) {
						// parse for statement
						string_view forMethodName;
						vector<ParserArgument> forArguments;
						string accessObjectMemberStatement;
						string executableStatement = doStatementPreProcessing(statementCode, generatedStatement);
						// success?
						if (parseStatement(executableStatement, forMethodName, forArguments, generatedStatement, accessObjectMemberStatement) == true &&
							forArguments.size() == 3) {
							// create initialize statement
							string initializeStatement = string(forArguments[0].argument);
							scripts.back().statements.emplace_back(currentLineIdx + lineIdxOffset, statementIdx++, statementCode, initializeStatement, STATEMENTIDX_NONE);
							//
							blockStack.emplace_back(
								Block::TYPE_FOR,
								statementIdx
							);
							//
							statementCode = "forCondition(" + string(forArguments[1].argument) + ", -> { " + string(forArguments[2].argument) + " })";
						} else {
							_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": Invalid for statement");
							parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": Invalid for statement");
							scriptValid = false;
							return false;
						}
					} else
					// array/set forEach
					if (_StringTools::regexMatch(regexStatementCode, "^forEach[\\s]*\\([\\s]*(&?\\$[a-zA-Z0-9_]+)[\\s]*\\in[\\s]*((\\$[a-zA-Z0-9_]+)|(\\[.*\\])|(\\{.*\\}))[\\s]*\\)$", &matches) == true) {
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
								"if (script.isNative() == true); " +
									"setVariableReference(\"" + containerArrayVariableBackup + "\", " + containerArrayVariable + "); " +
									"setVariableReference(\"" + entryVariableBackup + "\", " + entryVariable + "); " +
								"end; " +
								containerVariableType + " = getType(" + containerVariable + "); " +
								"if (" + containerVariableType + " == \"Array\"); " +
									"setVariableReference(\"" + containerArrayVariable + "\", " + containerVariable + "); " +
								"elseif (" + containerVariableType + " == \"Set\"); " +
									containerArrayVariable + " = Set::getKeys(" + containerVariable + "); " +
								"else; " +
									"console.printLine(\"forEach() expects array or set as container, but got \" + String::toLowerCase(getType(" + containerVariable + "))); " +
									"script.emit(\"error\"); " +
								"end; " +
								iterationVariable + " = 0; " +
								"if (" + iterationVariable + " < Array::getSize(" + containerArrayVariable + ")); " +
									iterationUpdate + "; " +
								"end;" +
							"}";
						//
						if (containerByInitializer == true) {
							scripts.back().statements.emplace_back(
								currentLineIdx + lineIdxOffset,
								statementIdx++,
								statementCode,
								doStatementPreProcessing(containerVariable + " = " + containerInitializer, generatedStatement),
								STATEMENTIDX_NONE
							);
						}
						scripts.back().statements.emplace_back(
							currentLineIdx + lineIdxOffset,
							statementIdx++,
							statementCode,
							doStatementPreProcessing(initialization, generatedStatement),
							STATEMENTIDX_NONE
						);
						scripts.back().statements.emplace_back(
							currentLineIdx + lineIdxOffset,
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
							"forCondition(" + iterationVariable + " < Array::getSize(" + containerArrayVariable + "), " +
							"-> { " +
								iterationVariable + "++" + "; " +
								"if (" + iterationVariable + " < Array::getSize(" + containerArrayVariable + ")); " +
									iterationUpdate + "; " +
								"else; " +
									"if (script.isNative() == true); " +
										"setVariableReference(\"" + containerArrayVariable + "\", " + containerArrayVariableBackup + "); " +
										"setVariableReference(\"" + entryVariable + "\", " + entryVariableBackup + "); " +
									"else; " +
										"unsetVariableReference(\"" + containerArrayVariable + "\"); " +
										"unsetVariableReference(\"" + entryVariable + "\"); " +
									"end; " +
									"setVariable(\"" + containerArrayVariable + "\", $$.___ARRAY); " +
									"setVariable(\"" + entryVariable + "\", $$.___NULL); " +
									(containerByInitializer == true?"setVariable(\"" + containerVariable + "\", $$.___ARRAY); ":"") +
								"end; " +
							"})";
					} else
					// map forEach
					if (_StringTools::regexMatch(regexStatementCode, "^forEach[\\s]*\\([\\s]*(\\$[a-zA-Z0-9_]+)[\\s]*,[\\s]*(&?\\$[a-zA-Z0-9_]+)[\\s]*\\in[\\s]*((\\$[a-zA-Z0-9_]+)|(\\[.*\\])|(\\{.*\\}))[\\s]*\\)$", &matches) == true) {
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
								"setVariableReference(\"" + entryValueVariable + "\", Map::getReference(" + containerVariable + ", " + entryKeyVariable + "))":
								entryValueVariable + " = Map::get(" + containerVariable + ", " + entryKeyVariable + ")"
							);
						//
						string initialization =
							initializationStackletVariable + " = -> { " +
								"if (script.isNative() == true); " +
									"setVariableReference(\"" + containerArrayVariable + "\", " + containerArrayVariableBackup + "); " +
									"setVariableReference(\"" + entryValueVariable + "\", " + entryValueVariableBackup + "); " +
								"end; " +
								containerVariableType + " = getType(" + containerVariable + "); " +
								"if (" + containerVariableType + " == \"Map\"); " +
									containerArrayVariable + " = Map::getKeys(" + containerVariable + "); " +
								"else; " +
									"console.printLine(\"forEach() expects map as container, but got \" + String::toLowerCase(getType(" + containerVariable + "))); " +
									"script.emit(\"error\"); " +
								"end; " +
								iterationVariable + " = 0; " +
								"if (" + iterationVariable + " < Array::getSize(" + containerArrayVariable + ")); " +
									iterationUpdate + "; " +
								"end; " +
							"}";
						//
						if (containerByInitializer == true) {
							scripts.back().statements.emplace_back(
								currentLineIdx + lineIdxOffset,
								statementIdx++,
								statementCode,
								doStatementPreProcessing(containerVariable + " = " + containerInitializer, generatedStatement),
								STATEMENTIDX_NONE
							);
						}
						scripts.back().statements.emplace_back(
							currentLineIdx + lineIdxOffset,
							statementIdx++,
							statementCode,
							doStatementPreProcessing(initialization, generatedStatement),
							STATEMENTIDX_NONE
						);
						scripts.back().statements.emplace_back(
							currentLineIdx + lineIdxOffset,
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
							"forCondition(" + iterationVariable + " < Array::getSize(" + containerArrayVariable + "), " +
							"-> { " +
								iterationVariable + "++" + "; " +
								"if (" + iterationVariable + " < Array::getSize(" + containerArrayVariable + ")); " +
									iterationUpdate + "; " +
								"else; " +
									"if (script.isNative() == true); " +
										"setVariableReference(\"" + containerArrayVariable + "\", " + containerArrayVariableBackup + "); " +
										"setVariableReference(\"" + entryValueVariable + "\", " + entryValueVariableBackup + "); " +
									"else; "
										"unsetVariableReference(\"" + containerArrayVariable + "\"); " +
										"unsetVariableReference(\"" + entryValueVariable + "\"); " +
									"end; " +
									"setVariable(\"" + containerArrayVariable + "\", $$.___ARRAY); " +
									"setVariable(\"" + entryKeyVariable + "\", $$.___NULL); "
									"setVariable(\"" + entryValueVariable + "\", $$.___NULL); " +
									(containerByInitializer == true?"setVariable(\"" + containerVariable + "\", $$.___ARRAY); ":"") +
								"end;"
							"})";
					} else
					if (_StringTools::regexMatch(regexStatementCode, "^forEach[\\s]*\\(.*\\)$", &matches) == true) {
						_Console::printLine(scriptFileName + ":" + to_string(currentLineIdx + lineIdxOffset) + ": Invalid forEach statement");
						parseErrors.push_back(to_string(currentLineIdx + lineIdxOffset) + ": Invalid forEach statement");
						scriptValid = false;
						return false;
					} else
					if (_StringTools::regexMatch(regexStatementCode, "^forTime[\\s]*\\(.*\\)$") == true ||
						_StringTools::regexMatch(regexStatementCode, "^forCondition[\\s]*\\(.*\\)$") == true) {
						blockStack.emplace_back(
							Block::TYPE_FOR,
							statementIdx
						);
					} else
					if (_StringTools::regexMatch(regexStatementCode, "^if[\\s]*\\(.*\\)$") == true) {
						blockStack.emplace_back(
							Block::TYPE_IF,
							statementIdx
						);
					} else
					if (_StringTools::regexMatch(regexStatementCode, "^switch[\\s]*\\(.*\\)$") == true) {
						blockStack.emplace_back(
							Block::TYPE_SWITCH,
							STATEMENTIDX_NONE
						);
					} else
					if (_StringTools::regexMatch(regexStatementCode, "^case[\\s]*\\(.*\\)$") == true) {
						blockStack.emplace_back(
							Block::TYPE_CASE,
							statementIdx
						);
					} else
					if (_StringTools::regexMatch(regexStatementCode, "^default[\\s]*$") == true) {
						blockStack.emplace_back(
							Block::TYPE_DEFAULT,
							statementIdx
						);
					}
					scripts.back().statements.emplace_back(
						currentLineIdx + lineIdxOffset,
						statementIdx,
						statementCode,
						doStatementPreProcessing(statementCode, generatedStatement),
						STATEMENTIDX_NONE
					);
				}
				statementIdx++;
			}
		}
	}

	// check for unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end
	if (scriptValid == true && blockStack.empty() == false) {
		_Console::printLine(scriptFileName + ": Unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
		parseErrors.push_back("Unbalanced if/elseif/else/switch/case/default/forCondition/forTime/end");
		scriptValid = false;
		return false;
	}

	// create syntax tree
	for (auto scriptIdx = scriptCount; scriptIdx < scripts.size(); scriptIdx++) {
		auto& script = scripts[scriptIdx];
		// create syntax tree of executable condition if we have any
		if (script.emitCondition == false && script.executableCondition.empty() == false) {
			string_view method;
			vector<ParserArgument> arguments;
			string accessObjectMemberStatement;
			if (parseStatement(script.executableCondition, method, arguments, script.conditionStatement, accessObjectMemberStatement) == false) {
				scriptValid = false;
				return false;
			} else
			if (createStatementSyntaxTree(SCRIPTIDX_NONE, method, arguments, script.conditionStatement, script.conditionSyntaxTree) == false) {
				scriptValid = false;
				return false;
			}
		}
		// create script syntax tree
		for (auto statementIdx = STATEMENTIDX_FIRST; statementIdx < script.statements.size(); statementIdx++) {
			const auto& statement = script.statements[statementIdx];
			script.syntaxTree.emplace_back();
			auto& syntaxTree = script.syntaxTree.back();
			string_view methodName;
			vector<ParserArgument> arguments;
			string accessObjectMemberStatement;
			if (parseStatement(statement.executableStatement, methodName, arguments, statement, accessObjectMemberStatement) == false) {
				scriptValid = false;
				return false;
			} else
			if (createStatementSyntaxTree(scriptIdx, methodName, arguments, statement, syntaxTree) == false) {
				scriptValid = false;
				return false;
			}
		}
	}
	//
	return scriptValid;
}

void MinitScript::parseScript(const string& pathName, const string& fileName) {
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
		_Console::printLine(scriptPathName + "/" + scriptFileName + ": An error occurred: " + fse.what());
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
		for (const auto& functionScriptCodePair: deferredInlineScriptCodesCopy) {
			parseScriptInternal(functionScriptCodePair.second, functionScriptCodePair.first);
		}
	} while (deferredInlineScriptCodes.empty() == false);
	// set up stacklet and function indices
	for (auto scriptIdx = 0; scriptIdx < scripts.size(); scriptIdx++) {
		//
		if (setupFunctionAndStackletScriptIndices(scriptIdx) == false) {
			scriptValid = false;
			return;
		}
	}
	// validations
	if (scriptValid == true) {
		for (auto scriptIdx = 0; scriptIdx < scripts.size(); scriptIdx++) {
			const auto& script = scripts[scriptIdx];
			//
			if (script.type == MinitScript::Script::TYPE_STACKLET) {
				// valid: root scope
				if (script.arguments.empty()) continue;
				// invalid: more than 1 argument
				if (script.arguments.size() != 1) {
					_Console::printLine(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: only none(for root scope) or one argument is allowed, which defines a function/stacklet as stacklet scope");
					parseErrors.push_back(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: only none(for root scope) or one argument is allowed, which defines a function/stacklet as stacklet scope");
					scriptValid = false;
					return;
				}
				//
				auto stackletScopeScriptIdx = getFunctionScriptIdx(script.arguments[0].name);
				// invalid: scope function/stacklet not found
				if (stackletScopeScriptIdx == SCRIPTIDX_NONE) {
					_Console::printLine(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet not found: " + script.arguments[0].name);
					parseErrors.push_back(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet not found: " + script.arguments[0].name);
					scriptValid = false;
					return;
				} else
				// invalid: stacklet can not have itself as scope stacklet
				if (stackletScopeScriptIdx == scriptIdx) {
					_Console::printLine(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet can not be the stacklet itself");
					parseErrors.push_back(scriptFileName + ": Stacklet: " + script.condition + ": invalid arguments: scope function/stacklet can not be the stacklet itself");
					scriptValid = false;
					return;
				}
			}
			//
			if (script.type == MinitScript::Script::TYPE_FUNCTION ||
				script.type == MinitScript::Script::TYPE_ON ||
				script.type == MinitScript::Script::TYPE_ONENABLED) {
				//
				if (validateStacklets(scriptIdx) == false) {
					scriptValid = false;
					return;
				}
			}
			//
			if (script.type == MinitScript::Script::TYPE_FUNCTION) {
				//
				if (script.callable == true) {
					//
					if (validateCallable(script.condition) == false) {
						scriptValid = false;
						return;
					}
				} else {
					//
					vector<string> functionStack;
					//
					if (validateContextFunctions(script.condition, functionStack) == false) {
						scriptValid = false;
						return;
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
		_Console::printLine(scriptPathName + "/" + scriptFileName + ": Script needs to define an error condition");
		parseErrors.push_back("Script needs to define an error condition");
		scriptValid = false;
		return;
	}

	//
	startScript();
}

void MinitScript::startScript() {
	if (VERBOSE == true) _Console::printLine("MinitScript::startScript(): '" + scriptFileName + ": Starting script.");
	if (scriptValid == false) {
		_Console::printLine(scriptFileName + ": Script not valid: not starting");
		return;
	}
	auto& scriptState = getScriptState();
	for (const auto& [variableName, variable]: scriptState.variables) delete variable;
	scriptState.variables.clear();
	scriptState.running = true;
	registerVariables();
	//
	if (hasCondition("initialize") == true) emit("initialize");
}

int MinitScript::determineScriptIdxToStart() {
	if (VERBOSE == true) _Console::printLine("MinitScript::determineScriptIdxToStart()");
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
			Variable returnValue;
			if (evaluateInternal(script.condition, script.executableCondition, returnValue) == true) {
				auto returnValueBoolValue = false;
				if (returnValue.getBooleanValue(returnValueBoolValue, false) == false) {
					_Console::printLine("MinitScript::determineScriptIdxToStart(): " + script.condition + ": Expecting boolean return value, but got: " + returnValue.getAsString());
					conditionMet = false;
				} else
				if (returnValueBoolValue == false) {
					conditionMet = false;
				}
			} else {
				_Console::printLine("MinitScript::determineScriptIdxToStart(): " + scriptFileName + ":" + to_string(script.line) + ": " + script.condition + ": Parse error");
			}
			if (conditionMet == false) {
				if (VERBOSE == true) {
					_Console::print("MinitScript::determineScriptIdxToStart(): " + script.condition + ": FAILED");
				}	
			} else {
				if (VERBOSE == true) {
					_Console::print("MinitScript::determineScriptIdxToStart(): " + script.condition + ": OK");
				}
				return scriptIdx;
			}
		}
		scriptIdx++;
	}
	return nothingScriptIdx;
}

int MinitScript::determineNamedScriptIdxToStart() {
	if (VERBOSE == true) _Console::printLine("MinitScript::determineNamedScriptIdxToStart()");
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
						_Console::printLine("MinitScript::determineNamedScriptIdxToStart(): " + script.condition + ": Expecting boolean return value, but got: " + returnValue.getAsString());
						conditionMet = false;
					} else
					if (returnValueBoolValue == false) {
						conditionMet = false;
					}
				} else {
					_Console::printLine("MinitScript::determineNamedScriptIdxToStart(): " + scriptFileName + ":" + to_string(script.line) + ": " + script.condition + ": Parse error");
				}
				if (conditionMet == false) {
					if (VERBOSE == true) {
						_Console::print("MinitScript::determineNamedScriptIdxToStart(): " + script.condition + ": FAILED");
					}
				} else {
					if (VERBOSE == true) {
						_Console::print("MinitScript::determineNamedScriptIdxToStart(): " + script.condition + ": OK");
					}
					return scriptIdx;
				}
			}
			scriptIdx++;
		}
	}
	return SCRIPTIDX_NONE;
}

const string MinitScript::doStatementPreProcessing(const string& processedStatement, const Statement& statement) {
	auto preprocessedStatement = processedStatement;
	//
	struct StatementOperator {
		int idx { OPERATORIDX_NONE };
		Operator operator_;
		string invalidOperator;
	};
	//
	auto trimArgument = [](const string& argument) -> const string {
		auto leftNewLineCount = 0;
		for (auto i = 0; i < argument.size(); i++) {
			auto c = argument[i];
			if (c == '\n') {
				leftNewLineCount++;
			} else
			if (_Character::isSpace(c) == false) {
				break;
			}
		}
		auto rightNewLineCount = 0;
		for (int i = argument.size() - 1; i >= 0; i--) {
			auto c = argument[i];
			if (c == '\n') {
				rightNewLineCount++;
			} else
			if (_Character::isSpace(c) == false) {
				break;
			}
		}
		auto processedArgument = _StringTools::trim(argument);
		if (_StringTools::startsWith(processedArgument, "(") == true && _StringTools::endsWith(processedArgument, ")") == true) {
			processedArgument = _StringTools::substring(processedArgument, 1, processedArgument.size() - 1);
		}
		return processedArgument.empty() == true?string():_StringTools::generate("\n", leftNewLineCount) + processedArgument + _StringTools::generate("\n", rightNewLineCount);
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
			auto lc = i > 0?statement[i - 1]:'\0';
			if (lc == '\\' && c == '\\') lc = '\0';
			//
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
					if (argument.empty() == true && (c == ' ' || c == '\t')) {
						// no op
					} else {
						argument+= c;
					}
				} else {
					//
					if (argument.empty() == true && (c == ' ' || c == '\t')) {
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
			lc = lc == '\\' && c == '\\'?'\0':c;
		}
		//
		return trimArgument(argument);
	};
	//
	auto viewIsLamdaFunction = [](const string_view& candidate) -> bool {
		if (candidate.empty() == true) return false;
		//
		auto i = 0;
		// (
		if (candidate[i++] != '(') return false;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++);
		if (i >= candidate.size()) return false;
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
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++);
		if (i >= candidate.size()) return false;
		// -
		if (candidate[i++] != '-') return false;
		//
		if (i >= candidate.size()) return false;
		// >
		if (candidate[i++] != '>') return false;
		// spaces
		for (; i < candidate.size() && _Character::isSpace(candidate[i]) == true; i++);
		if (i >= candidate.size()) return false;
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
					{
						// find operator from current statement character index
						string operatorCandidate;
						for (int j = i - 1; j >= 0; j--) {
							if (isOperatorChar(processedStatement[j]) == false) break;
							operatorCandidate = processedStatement[j] + operatorCandidate;
						}
						for (auto j = i; j < processedStatement.size(); j++) {
							if (isOperatorChar(processedStatement[j]) == false) break;
							operatorCandidate = operatorCandidate + processedStatement[j];
						}
						// skip on empty operator or -> object member call
						if (operatorCandidate.empty() == false && operatorCandidate != "->") {
							// invalid operator
							if (isOperator(operatorCandidate) == false) {
								nextOperator.idx = i;
								nextOperator.operator_ = OPERATOR_NONE;
								nextOperator.invalidOperator = operatorCandidate;
							}
						}
					}
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
							auto isMemberAccessOrAssignmentOperator = [](const string& candidate) { return candidate == "->"; };
							if (operatorString.size() == 1 &&
								i > 0 &&
								isOperatorChar(processedStatement[i - 1]) == true &&
								(isMemberAccessOrAssignmentOperator(processedStatement[i - 1] + operatorString) == true ||
								isOperator(processedStatement[i - 1] + operatorString) == true)) {
								continue;
							}
							if (operatorString.size() == 1 &&
								i + 1 < processedStatement.size() &&
								isOperatorChar(processedStatement[i + 1]) == true &&
								(isMemberAccessOrAssignmentOperator(operatorString + processedStatement[i + 1]) == true ||
								isOperator(operatorString + processedStatement[i + 1]) == true)) {
								continue;
							}
							if (operatorString.size() == 2 &&
								i > 0 &&
								isOperatorChar(processedStatement[i - 1]) == true) {
								continue;
							}
							if (operatorString.size() == 2 &&
								i + 2 < processedStatement.size() &&
								isOperatorChar(processedStatement[i + 2]) == true) {
								continue;
							} else
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
			lc = lc == '\\' && c == '\\'?'\0':c;
		}

		//
		if (bracketCount > 0) {
			//
			return false;
		}
		//
		return nextOperator.idx != OPERATORIDX_NONE;
	};
	//
	StatementOperator nextOperator;
	while (getNextStatementOperator(preprocessedStatement, nextOperator, statement) == true) {
		if (nextOperator.invalidOperator.empty() == false) {
			_Console::printLine(getStatementInformation(statement, getStatementSubLineIdx(preprocessedStatement, nextOperator.idx)) + ": Invalid operator: " + nextOperator.invalidOperator);
			scriptValid = false;
			return preprocessedStatement;
		}
		//
		Method* method { nullptr };
		Method* prefixOperatorMethod { nullptr };
		Method* postfixOperatorMethod { nullptr };
		//
		{
			// resolve method
			method = getOperatorMethod(nextOperator.operator_);
			if (method == nullptr) {
				_Console::printLine(getStatementInformation(statement, getStatementSubLineIdx(preprocessedStatement, nextOperator.idx)) + ": No operator method found");
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
					_Console::printLine(getStatementInformation(statement, getStatementSubLineIdx(preprocessedStatement, nextOperator.idx)) + ": No prefix operator method found");
					scriptValid = false;
					return preprocessedStatement;
				}
				//
				if (postfixOperatorMethod == nullptr) {
					_Console::printLine(getStatementInformation(statement, getStatementSubLineIdx(preprocessedStatement, nextOperator.idx)) + ": No postfix operator method found");
					scriptValid = false;
					return preprocessedStatement;
				}
			}
		}
		//
		if (method->isVariadic() == false &&
			method->getArgumentTypes().size() == 1) {
			// find the single argument right
			auto operatorString = getOperatorAsString(nextOperator.operator_);
			// find left argument
			string leftArgumentBrackets;
			int leftArgumentLength = 0;
			auto leftArgument = findLeftArgument(preprocessedStatement, nextOperator.idx - 1, leftArgumentLength, leftArgumentBrackets);
			// find argument right
			string rightArgumentBrackets;
			int rightArgumentLength = 0;
			auto rightArgument = findRightArgument(preprocessedStatement, nextOperator.idx + operatorString.size(), rightArgumentLength, rightArgumentBrackets);
			//
			if (leftArgument.empty() == false) {
				//
				auto leftArgumentNewLines = 0;
				for (auto i = 0; i < leftArgument.size(); i++) {
					auto c = leftArgument[i];
					if (c == '\n') leftArgumentNewLines++; else break;
				}
				// substitute with method call
				preprocessedStatement =
					_StringTools::substring(preprocessedStatement, 0, nextOperator.idx - leftArgumentLength) +
					_StringTools::generate("\n", leftArgumentNewLines) +
					prefixOperatorMethod->getMethodName() + "(" + _StringTools::substring(leftArgument, leftArgumentNewLines) + ")" +
					_StringTools::substring(preprocessedStatement, nextOperator.idx + operatorString.size(), preprocessedStatement.size()
				);
			} else
			if (rightArgument.empty() == false) {
				// substitute with method call
				preprocessedStatement =
					_StringTools::substring(preprocessedStatement, 0, nextOperator.idx) +
					postfixOperatorMethod->getMethodName() + "(" + rightArgument + ")" +
					_StringTools::substring(preprocessedStatement, nextOperator.idx + operatorString.size() + rightArgumentLength, preprocessedStatement.size());
			} else {
				_Console::printLine(getStatementInformation(statement, getStatementSubLineIdx(preprocessedStatement, nextOperator.idx)) + ": Requires left or right argument");
				scriptValid = false;
				return preprocessedStatement;
			}
		} else
		if (method->isVariadic() == true ||
			method->getArgumentTypes().size() == 2) {
			//
			auto operatorString = getOperatorAsString(nextOperator.operator_);
			// find left argument
			string leftArgumentBrackets;
			int leftArgumentLength = 0;
			auto leftArgument = findLeftArgument(preprocessedStatement, nextOperator.idx - 1, leftArgumentLength, leftArgumentBrackets);
			// find right argument
			string rightArgumentBrackets;
			int rightArgumentLength = 0;
			auto rightArgument = findRightArgument(preprocessedStatement, nextOperator.idx + operatorString.size(), rightArgumentLength, rightArgumentBrackets);
			//
			if (leftArgument.empty() == true || rightArgument.empty() == true) {
				_Console::printLine(getStatementInformation(statement, getStatementSubLineIdx(preprocessedStatement, nextOperator.idx)) + ": Requires left and right argument");
				scriptValid = false;
				return preprocessedStatement;
			}
			//
			if (nextOperator.operator_ == OPERATOR_SET) {
				leftArgument = "\"" + doStatementPreProcessing(leftArgument, statement) + "\"";
			}
			//
			auto leftArgumentNewLines = 0;
			for (auto i = 0; i < leftArgument.size(); i++) {
				auto c = leftArgument[i];
				if (c == '\n') leftArgumentNewLines++; else break;
			}
			// substitute with method call
			preprocessedStatement =
				_StringTools::substring(preprocessedStatement, 0, nextOperator.idx - leftArgumentLength) +
				_StringTools::generate("\n", leftArgumentNewLines) +
				method->getMethodName() + "(" + _StringTools::substring(leftArgument, leftArgumentNewLines) + ", " + rightArgument + ")" +
				_StringTools::substring(preprocessedStatement, nextOperator.idx + operatorString.size() + rightArgumentLength, preprocessedStatement.size()
			);
			//
		}
		//
		nextOperator = StatementOperator();
	}
	//
	return preprocessedStatement;
}

bool MinitScript::getObjectMemberAccess(const string_view& executableStatement, string_view& object, string_view& method, int& methodStartIdx, const Statement& statement) {
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
			// we need to check the serious candidates here
			if (objectCandidate.empty() == false &&
				methodCandidate.empty() == false &&
				(viewIsStringLiteral(objectCandidate) == true || viewIsVariableAccess(objectCandidate) == true || viewIsCall(objectCandidate) == true)) {
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
		lc = lc == '\\' && c == '\\'?'\0':c;
	}
	//
	return objectMemberAccess;
}

void MinitScript::dumpScriptState(ScriptState& scriptState, const string& message) {
	_Console::printLine("MinitScript::dumpScriptState(): " + (message.empty() == false?message + ": ":"") + to_string(scriptStateStack.size()) + " on stack");
	_Console::printLine(string("\t") + "state: " + to_string(scriptState.state));
	_Console::printLine(string("\t") + "lastState: " + to_string(scriptState.lastState));
	_Console::printLine(string("\t") + "running: " + (scriptState.running == true?"true":"false"));
	_Console::printLine(string("\t") + "scriptIdx: " + to_string(scriptState.scriptIdx));
	_Console::printLine(string("\t") + "statementIdx: " + to_string(scriptState.statementIdx));
	_Console::printLine(string("\t") + "gotoStatementIdx: " + to_string(scriptState.gotoStatementIdx));
	_Console::printLine(string("\t") + "variable count: " + to_string(scriptState.variables.size()));
	_Console::printLine(string("\t") + "block stack count: " + to_string(scriptState.blockStack.size()));
	array<string, 9> blockStackTypes {
		"TYPE_NONE",
		"TYPE_GLOBAL",
		"TYPE_STACKLET",
		"TYPE_FUNCTION",
		"TYPE_FOR",
		"TYPE_FORTIME",
		"TYPE_IF",
		"TYPE_SWITCH",
		"TYPE_CASE"
	};
	for (const auto& block: scriptState.blockStack) {
		_Console::printLine(string("\t\t") + blockStackTypes[block.type]);
	}
	_Console::printLine(string("\t") + "returnValue: " + scriptState.returnValue.getValueAsString());
}

bool MinitScript::call(int scriptIdx, span<Variable>& arguments, Variable& returnValue, bool pushScriptState) {
	//
	if (scriptIdx < 0 || scriptIdx >= scripts.size()) {
		_Console::printLine("MinitScript::call(): Invalid script index: " + to_string(scriptIdx));
		return false;
	}
	auto& script = scripts[scriptIdx];
	//
	if (script.type != Script::TYPE_FUNCTION &&
		script.type != Script::TYPE_STACKLET) {
		_Console::printLine("MinitScript::call(): " + (script.name.empty() == false?script.name:script.condition) + ": Script is not a function/callable/stacklet.");
		return false;
	}
	// copy script state
	ScriptState currentScriptState = getScriptState();
	//
	if (pushScriptState == true) {
		if (script.type == Script::TYPE_STACKLET) {
			_Console::printLine("MinitScript::call(): " + script.condition + ": Stacklets can not be called with a stack.");
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
			// private scope
			if (argument.privateScope == true) {
				arguments[argumentIdx].setPrivateScope();
			} else {
				arguments[argumentIdx].unsetPrivateScope();
			}
			//
			setVariable(argument.name, arguments[argumentIdx], nullptr, argument.reference);
			argumentIdx++;
		}
		//
		resetScriptExecutationState(scriptIdx, STATEMACHINESTATE_NEXT_STATEMENT);
	} else {
		//
		if (script.type != Script::TYPE_STACKLET) {
			_Console::printLine("MinitScript::call(): " + script.condition + ": Function/Callable can not be called with no stack.");
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
		returnValue.setValue(scriptState.returnValue);
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
	// if function calls are worked off, we can do the deferred emit
	if (isFunctionRunning() == false && deferredEmit.empty() == false) {
		auto condition = deferredEmit;
		deferredEmit.clear();
		emit(condition);
	}
	//
	return true;
}

const vector<MinitScript::Method*> MinitScript::getMethods() {
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

const vector<MinitScript::Method*> MinitScript::getOperatorMethods() {
	vector<Method*> methods;
	for (const auto& [operatorId, method]: operators) {
		methods.push_back(method);
	}
	return methods;
}

const string MinitScript::getScriptInformation(int scriptIdx, bool includeStatements) {
	if (scriptIdx < 0 || scriptIdx >= scripts.size()) {
		_Console::printLine("MinitScript::getScriptInformation(): invalid script index: " + to_string(scriptIdx));
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
				if (argument.privateScope == true) argumentsString+= "&";
				argumentsString+= argument.name;
			}
			argumentsString = "(" + argumentsString + ")";
			if (script.type == Script::TYPE_FUNCTION) result+= "function: "; else result+= "stacklet: ";
			break;
		}
		case Script::TYPE_ON: result+= "on: "; break;
		case Script::TYPE_ONENABLED: result+= "on-enabled: "; break;
		default: break;
	}
	if (script.condition.empty() == false)
		result+= script.condition + argumentsString + "; ";
	if (script.name.empty() == false) {
		result+= "name = '" + script.name + argumentsString + "';\n";
	} else {
		result+= "\n";
	}
	if (includeStatements == true) {
		auto indent = 1;
		auto formatStatement = [&](const string& statement) {
			string result;
			auto statementMethodEndIdx = _StringTools::indexOf(statement, "(");
			if (statementMethodEndIdx == string::npos) statementMethodEndIdx = statement.size();
			auto statementMethod = _StringTools::trim(_StringTools::substring(statement, 0, statementMethodEndIdx));
			if (statementMethod == "elseif") indent-= 1; else
			if (statementMethod == "else") indent-= 1; else
			if (statementMethod == "case") indent-= 1; else
			if (statementMethod == "default") indent-= 1; else
			if (statementMethod == "end") indent-= 1;
			for (auto i = 0; i < indent; i++) result+= "  ";
			result+= statement;
			if (statementMethod == "if") indent+= 1; else
			if (statementMethod == "elseif") indent+= 1; else
			if (statementMethod == "else") indent+= 1; else
			if (statementMethod == "forTime") indent+= 1; else
			if (statementMethod == "forCondition") indent+= 1; else
			if (statementMethod == "switch") indent+= 1; else
			if (statementMethod == "case") indent+= 1; else
			if (statementMethod == "default") indent+= 1;
			return result;
		};
		result+=
			string() +
			"\t" + "    " + ": start" + "\n";
		for (const auto& statement: script.statements) {
			string newLineIndent; for (auto i = 0; i < indent + 2; i++) newLineIndent+= "  ";
			result+=
				"\t" +
				/*
				_StringTools::padLeft(to_string(statement.statementIdx), "0", 4) +
				"|" +
				*/
				_StringTools::padLeft(to_string(statement.line), "0", 4) +
				": " +
				_StringTools::replace(formatStatement(statement.executableStatement), "\n", "\n\t    :" + newLineIndent)
				/*
				+
				(statement.gotoStatementIdx != STATEMENTIDX_NONE?" (gotoStatement " + to_string(statement.gotoStatementIdx) + ")":"")
				*/
				+ "\n";
		}
		result+= "\n";
	}
	//
	return result;
}

const string MinitScript::getInformation() {
	string result;
	result+= "Script: " + scriptPathName + "/" + scriptFileName + " (runs " + (native == true?"natively":"interpreted") + ")" + "\n\n";
	result+= "\n";

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
	result+= "\n";

	//
	result+="Script:\n";
	{
		auto scriptIdx = 0;
		for (const auto& script: scripts) {
			result+= getScriptInformation(scriptIdx);
			scriptIdx++;
		}
	}

	//
	return result;
}

void MinitScript::registerStateMachineStates() {
	// base
	if (native == false) {
		//
		class ScriptStateNextStatement: public StateMachineState {
		private:
			MinitScript* minitScript { nullptr };
		public:
			ScriptStateNextStatement(MinitScript* minitScript): StateMachineState(), minitScript(minitScript) {}
			virtual const string getName() override {
				return "STATEMACHINESTATE_NEXT_STATEMENT";
			}
			virtual int getId() override {
				return STATEMACHINESTATE_NEXT_STATEMENT;
			}
			virtual void execute() override {
				if (minitScript->getScriptState().statementIdx == STATEMENTIDX_NONE) {
					minitScript->enabledNamedConditions.clear();
					minitScript->timeEnabledConditionsCheckLast = TIME_NONE;
					minitScript->setScriptStateState(STATEMACHINESTATE_WAIT_FOR_CONDITION);
					return;
				}
				if (minitScript->native == false) minitScript->executeNextStatement();
			}
		};
		registerStateMachineState(new ScriptStateNextStatement(this));
	}
	{
		//
		class ScriptStateWait: public StateMachineState {
		private:
			MinitScript* minitScript { nullptr };
		public:
			ScriptStateWait(MinitScript* minitScript): StateMachineState(), minitScript(minitScript) {}
			virtual const string getName() override {
				return "STATEMACHINESTATE_WAIT";
			}
			virtual int getId() override {
				return STATEMACHINESTATE_WAIT;
			}
			virtual void execute() override {
				auto now = _Time::getCurrentMillis();
				if (now > minitScript->getScriptState().timeWaitStarted + minitScript->getScriptState().timeWaitTime) {
					minitScript->setScriptStateState(STATEMACHINESTATE_NEXT_STATEMENT);
				} else {
					#if !defined(MINITSCRIPT_NO_SLEEP)
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
			MinitScript* minitScript { nullptr };
		public:
			ScriptStateWaitForCondition(MinitScript* minitScript): StateMachineState(), minitScript(minitScript) {}
			virtual const string getName() override {
				return "STATEMACHINESTATE_WAIT_FOR_CONDITION";
			}
			virtual int getId() override {
				return STATEMACHINESTATE_WAIT_FOR_CONDITION;
			}
			virtual void execute() override {
				auto now = _Time::getCurrentMillis();
				if (now < minitScript->getScriptState().timeWaitStarted + minitScript->getScriptState().timeWaitTime) {
					return;
				}
				auto scriptIdxToStart = minitScript->determineScriptIdxToStart();
				if (scriptIdxToStart == SCRIPTIDX_NONE) {
					minitScript->getScriptState().timeWaitStarted = now;
					minitScript->getScriptState().timeWaitTime = 100LL;
					return;
				}
				minitScript->resetScriptExecutationState(scriptIdxToStart, STATEMACHINESTATE_NEXT_STATEMENT);
			}
		};
		registerStateMachineState(new ScriptStateWaitForCondition(this));
	}
}

void MinitScript::registerMethods() {
	// unregister old registered methods
	for (const auto& [scriptMethodId, scriptMethod]: methods) delete scriptMethod;
	methods.clear();

	// register math methods
	minitScriptMath = make_unique<MathMethods>(this);
	minitScriptMath->registerMethods();

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
		if (dataType->isMathDataType() == true) minitScriptMath->registerDataType(dataType);
		dataType->registerMethods(this);
	}

	// determine operators
	for (const auto& [scriptMethodName, scriptMethod]: methods) {
		auto methodOperator = scriptMethod->getOperator();
		if (methodOperator != OPERATOR_NONE) {
			auto methodOperatorString = getOperatorAsString(methodOperator);
			auto scriptOperatorIt = operators.find(static_cast<uint8_t>(methodOperator));
			if (scriptOperatorIt != operators.end()) {
				_Console::printLine("MinitScript::registerMethods(): Operator '" + methodOperatorString + "' already registered for method " + scriptMethod->getMethodName() + "");
				continue;
			}
			operators[static_cast<uint8_t>(methodOperator)] = scriptMethod;
		}
	}
}

void MinitScript::registerVariables() {
	//
	for (const auto& [variableName, variable]: getRootScriptState().variables) delete variable;
	getRootScriptState().variables.clear();

	//
	minitScriptMath->registerConstants();

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

void MinitScript::createLamdaFunction(Variable& variable, const vector<string_view>& arguments, const string_view& functionScriptCode, int lineIdx, bool populateThis, const Statement& statement, const string& nameHint) {
	// function declaration
	auto functionName = string() + "lamda_function_" + (nameHint.empty() == true?"":_StringTools::toLowerCase(nameHint) + "_") + to_string(inlineFunctionIdx++);
	auto inlineFunctionScriptCode = "function: " + functionName + "(";
	if (populateThis == true) inlineFunctionScriptCode+= "&&$this";
	auto argumentIdx = 0;
	for (const auto& argument: arguments) {
		if (argumentIdx > 0 || populateThis == true) inlineFunctionScriptCode+= ",";
		inlineFunctionScriptCode+= argument;
		argumentIdx++;
	}
	inlineFunctionScriptCode+= string() + ")" + "\n";
	// function definition
	inlineFunctionScriptCode+= string(functionScriptCode);
	inlineFunctionScriptCode+= "\n";
	inlineFunctionScriptCode+= string() + "end" + "\n";
	// store it to be parsed later
	//	we can reduce line index by function head
	//	our line counting does not start at 1 here, but at zero
	deferredInlineScriptCodes.push_back(make_pair(lineIdx - 2, inlineFunctionScriptCode));
	//
	variable.setFunctionAssignment(functionName);
}

void MinitScript::createStacklet(Variable& variable, const string& scopeName, const vector<string_view>& arguments, const string_view& stackletScriptCode, int lineIdx, const Statement& statement) {
	// stacklet declaration
	auto stackletName = string() + "stacklet_" + to_string(inlineStackletIdx++);
	auto inlineStackletScriptCode = "stacklet: " + stackletName + "(" + scopeName + ")" + "\n";
	// stacklet definition
	inlineStackletScriptCode+= string(stackletScriptCode);
	inlineStackletScriptCode+= "\n";
	inlineStackletScriptCode+= string() + "end" + "\n";
	// store it to be parsed later
	//	we can reduce line index by function head
	//	our line counting does not start at 1 here, but at zero
	deferredInlineScriptCodes.push_back(make_pair(lineIdx - 2, inlineStackletScriptCode));
	//
	variable.setStackletAssignment(stackletName);
}

const MinitScript::Variable MinitScript::initializeArray(const string_view& initializerString, MinitScript* minitScript, int scriptIdx, const Statement& statement) {
	Variable variable;
	variable.setType(TYPE_ARRAY);
	//
	auto lineIdx = statement.line;
	auto bracketCount = 0;
	auto squareBracketCount = 0;
	auto curlyBracketCount = 0;
	auto quote = '\0';
	auto arrayValueStart = string::npos;
	auto arrayValueEnd = string::npos;
	auto inlineFunctionSignatureStartCandidate = string::npos;
	auto inlineFunctionLineIdxCandidate = LINE_NONE;
	auto inlineFunctionLineIdx = LINE_NONE;
	//
	auto lc = '\0';
	auto i = 0;
	//
	auto pushToArray = [&]() -> void {
		// array value
		if (arrayValueStart != string::npos) {
			arrayValueEnd = i - 1;
			auto arrayValueLength = arrayValueEnd - arrayValueStart + 1;
			if (arrayValueLength > 0) {
				auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[arrayValueStart], arrayValueLength));
				if (arrayValueStringView.empty() == false) {
					Variable arrayValue;
					arrayValue.setImplicitTypedValueFromStringView(arrayValueStringView, minitScript, scriptIdx, statement);
					variable.pushArrayEntry(arrayValue);
				}
			}
		}
		//
		arrayValueStart = string::npos;
		arrayValueEnd = string::npos;
		//
		inlineFunctionSignatureStartCandidate = string::npos;
		inlineFunctionLineIdxCandidate = LINE_NONE;
		inlineFunctionLineIdx = LINE_NONE;
	};
	//
	for (; i < initializerString.size(); i++) {
		auto c = initializerString[i];
		// newline/line index
		if (c == '\n') {
			lineIdx++;
			// check for comment line
			auto comment = false;
			for (auto j = i + 1; j < initializerString.size(); j++) {
				auto _c = initializerString[j];
				// space after newline
				if (_Character::isSpace(_c) == true) {
					// no op
				} else
				// comment start
				if (_c == '#') {
					comment = true;
					// iterate until next new line
					for (j++; j < initializerString.size(); j++) {
						if (initializerString[j] == '\n') break;
					}
					//
					i = j - 1;
					break;
				} else {
					// non hash as first character after new line
					break;
				}
			}
			//
			if (comment == true) continue;
		}
		// quotes
		if (squareBracketCount == 1 && curlyBracketCount == 0 && (c == '"' || c == '\'') && lc != '\\') {
			if (quote == '\0') {
				quote = c;
				if (arrayValueStart == string::npos) arrayValueStart = i;
			} else
			if (quote == c) {
				quote = '\0';
				if (arrayValueEnd == string::npos) arrayValueEnd = i;
			}
		} else
		// no quote
		if (quote == '\0') {
			// , -> push to array
			if (squareBracketCount == 1 && curlyBracketCount == 0 && bracketCount == 0 && c == ',') {
				pushToArray();
			} else
			// possible function call
			if (c == '(') {
				//
				bracketCount++;
				//
				if (bracketCount == 1) {
					inlineFunctionSignatureStartCandidate = i;
					inlineFunctionLineIdxCandidate = lineIdx;
				}
			} else
			if (c == ')') {
				bracketCount--;
				// function assignment
				if (inlineFunctionSignatureStartCandidate != string::npos && bracketCount == 0 && arrayValueStart == string::npos) {
					arrayValueStart = inlineFunctionSignatureStartCandidate;
					inlineFunctionLineIdx = inlineFunctionLineIdxCandidate;
				}
				//
				inlineFunctionSignatureStartCandidate = string::npos;
				inlineFunctionLineIdxCandidate = LINE_NONE;
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
					inlineFunctionSignatureStartCandidate = string::npos;
					inlineFunctionLineIdxCandidate = LINE_NONE;
					//
					if (arrayValueStart != string::npos) {
						arrayValueEnd = i;
						auto arrayValueLength = arrayValueEnd - arrayValueStart + 1;
						if (arrayValueLength > 0) {
							auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[arrayValueStart], arrayValueLength));
							if (arrayValueStringView.empty() == false) {
								auto arrayValue = initializeArray(arrayValueStringView, minitScript, scriptIdx, statement);
								variable.pushArrayEntry(arrayValue);
							}
						}
						//
						arrayValueStart = string::npos;
						arrayValueEnd = string::npos;
					}
					//
					inlineFunctionLineIdx = LINE_NONE;
				}
			} else
			// map/set initializer
			if (c == '{' && squareBracketCount == 1 && bracketCount == 0) {
				// we have a inner map/set initializer, mark it
				if (curlyBracketCount == 0) {
					if (arrayValueStart == string::npos) arrayValueStart = i;
				}
				// increase curly bracket count
				curlyBracketCount++;
			} else
			// end of map/set initializer or inline lamda function
			if (c == '}' && squareBracketCount == 1 && bracketCount == 0) {
				curlyBracketCount--;
				// otherwise push inner array initializer
				if (curlyBracketCount == 0) {
					// parse and push
					inlineFunctionSignatureStartCandidate = string::npos;
					inlineFunctionLineIdxCandidate = LINE_NONE;
					//
					if (arrayValueStart != string::npos) {
						arrayValueEnd = i;
						auto arrayValueLength = arrayValueEnd - arrayValueStart + 1;
						if (arrayValueLength > 0) {
							auto arrayValueStringView = _StringTools::viewTrim(string_view(&initializerString[arrayValueStart], arrayValueLength));
							if (arrayValueStringView.empty() == false) {
								vector<string_view> lamdaFunctionArguments;
								string_view lamdaFunctionScriptCode;
								int lamdaFunctionLineIdx = inlineFunctionLineIdx;
								if (viewIsLamdaFunction(arrayValueStringView, lamdaFunctionArguments, lamdaFunctionScriptCode, lamdaFunctionLineIdx) == true) {
									Variable arrayValue;
									minitScript->createLamdaFunction(arrayValue, lamdaFunctionArguments, lamdaFunctionScriptCode, lamdaFunctionLineIdx, false, statement);
									variable.pushArrayEntry(arrayValue);
								} else {
									auto arrayValue = initializeMapSet(arrayValueStringView, minitScript, scriptIdx, statement);
									variable.pushArrayEntry(arrayValue);
								}
							}
						}
						//
						arrayValueStart = string::npos;
						arrayValueEnd = string::npos;
					}
					//
					inlineFunctionLineIdx = LINE_NONE;
				}
			} else
			// set up argument start
			if (squareBracketCount == 1 && curlyBracketCount == 0 && bracketCount == 0 && arrayValueStart == string::npos && c != ' ' && c != '\t' && c != '\n') {
				arrayValueStart = i;
			}
		}
		//
		lc = lc == '\\' && c == '\\'?'\0':c;
	}
	//
	auto initalizer = make_unique<MinitScript::Variable::Initializer>(string(initializerString), statement, nullptr);
	variable.ir.initializer->copy(initalizer.get());
	//
	return variable;
}

const MinitScript::Variable MinitScript::initializeMapSet(const string_view& initializerString, MinitScript* minitScript, int scriptIdx, const Statement& statement) {
	//
	Variable variable;
	variable.setType(TYPE_MAP);
	//
	auto subLineIdx = 0;
	auto bracketCount = 0;
	auto curlyBracketCount = 0;
	auto squareBracketCount = 0;
	auto quote = '\0';
	auto mapKeyStart = string::npos;
	auto mapKeyEnd = string::npos;
	auto mapValueStart = string::npos;
	auto mapValueEnd = string::npos;
	enum ParseMode { PARSEMODE_KEY, PARSEMODE_VALUE };
	auto parseMode = PARSEMODE_KEY;
	auto inlineFunctionSignatureStartCandidate = string::npos;
	auto inlineFunctionLineIdxCandidate = LINE_NONE;
	auto inlineFunctionLineIdx = LINE_NONE;
	auto mapKeyLineIdx = LINE_NONE;
	auto hasValues = false;
	//
	auto i = 0;

	//
	auto insertMapKeyValuePair = [&]() -> void {
		//
		string_view mapKey;
		auto dequotedMapKey = false;
		// map key
		if (mapKeyStart != string::npos && mapKeyEnd != string::npos) {
			//
			auto mapKeyLength = mapKeyEnd - mapKeyStart + 1;
			if (mapKeyLength > 0) {
				mapKey = _StringTools::viewTrim(string_view(&initializerString[mapKeyStart], mapKeyLength));
				if (viewIsStringLiteral(mapKey) == true) {
					mapKey = dequote(mapKey);
					dequotedMapKey = true;
				}
				if (mapKey.empty() == true) mapKey = string_view();
			}
		}
		//
		mapKeyStart = string::npos;
		mapKeyEnd = string::npos;
		// validate map key
		if (mapKey.empty() == true) {
			// no op
		} else
		if (viewIsKey(mapKey) == false) {
			_Console::printLine(minitScript->getStatementInformation(statement, mapKeyLineIdx) + ": Invalid key name, ignoring map entry: " + string(mapKey));
		} else {
			auto _private = dequotedMapKey == true?false:viewIsKeyPrivate(mapKey);
			if (_private == true) mapKey = viewGetPrivateKey(mapKey);
			// map value
			if (mapValueStart != string::npos && mapValueEnd != string::npos) {
				auto mapValueLength = mapValueEnd - mapValueStart + 1;
				//
				if (mapValueLength > 0) {
					auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[mapValueStart], mapValueLength));
					if (mapValueStringView.empty() == false) {
						//
						Variable mapValue;
						mapValue.setImplicitTypedValueFromStringView(mapValueStringView, minitScript, scriptIdx, statement);
						//
						variable.setMapEntry(string(mapKey), mapValue, _private);
						//
						hasValues = true;
					}
				}
			} else {
				//
				variable.setMapEntry(string(mapKey), Variable(), _private);
			}
		}
		//
		mapKeyLineIdx = LINE_NONE;
		//
		mapValueStart = string::npos;
		mapValueEnd = string::npos;
		//
		inlineFunctionSignatureStartCandidate = string::npos;
		inlineFunctionLineIdxCandidate = LINE_NONE;
		inlineFunctionLineIdx = LINE_NONE;
		//
		parseMode = PARSEMODE_KEY;
	};
	//
	auto lc = '\0';
	for (; i < initializerString.size(); i++) {
		//
		auto c = initializerString[i];
		auto nc = i < initializerString.size() - 1?initializerString[i + 1]:'\0';
		// newline/line index
		if (c == '\n') {
			//
			subLineIdx++;
			// check for comment line
			auto comment = false;
			for (auto j = i + 1; j < initializerString.size(); j++) {
				auto _c = initializerString[j];
				// space after newline
				if (_Character::isSpace(_c) == true) {
					// no op
				} else
				// comment start
				if (_c == '#') {
					comment = true;
					// iterate until next new line
					for (j++; j < initializerString.size(); j++) {
						if (initializerString[j] == '\n') break;
					}
					//
					i = j - 1;
					break;
				} else {
					// non hash as first character after new line
					break;
				}
			}
			//
			if (comment == true) continue;
		}
		// quotes
		if (curlyBracketCount == 1 && squareBracketCount == 0 && (c == '"' || c == '\'') && lc != '\\') {
			// we have a new quote here
			if (quote == '\0') {
				quote = c;
				// key?
				if (parseMode == PARSEMODE_KEY) {
					if (mapKeyStart == string::npos) {
						mapKeyStart = i;
						mapKeyLineIdx = subLineIdx;
					}
				} else
				// value
				if (parseMode == PARSEMODE_VALUE) {
					if (mapValueStart == string::npos) mapValueStart = i;
				}
			} else
			// finish the quote
			if (quote == c) {
				quote = '\0';
			}
		} else
		// no quote
		if (quote == '\0') {
			// : -> map key separator
			if (curlyBracketCount == 1 && squareBracketCount == 0 && bracketCount == 0 && c == ':' && nc != ':' && lc != ':' && lc != '\\') {
				//
				if (mapKeyStart != string::npos) {
					mapKeyEnd = i - 1;
				}
				//
				parseMode = PARSEMODE_VALUE;
				//
			} else
			// , -> insert map
			if (curlyBracketCount == 1 && squareBracketCount == 0 && bracketCount == 0 && c == ',') {
				// TODO: use parse mode here
				if (mapValueStart != string::npos) {
					mapValueEnd = i - 1;
				} else
				if (mapKeyStart != string::npos && mapValueStart == string::npos) {
					mapKeyEnd = i - 1;
				}
				// insert map key value pair
				insertMapKeyValuePair();
				// nada
			} else
			// possible function call or inline function as value
			if (c == '(') {
				//
				bracketCount++;
				//
				if (bracketCount == 1) {
					inlineFunctionSignatureStartCandidate = i;
					inlineFunctionLineIdxCandidate = statement.line + subLineIdx;
				}
			} else
			if (c == ')') {
				bracketCount--;
				// function assignment
				if (inlineFunctionSignatureStartCandidate != string::npos && bracketCount == 0 && mapValueStart == string::npos) {
					mapValueStart = inlineFunctionSignatureStartCandidate;
					inlineFunctionLineIdx = inlineFunctionLineIdxCandidate;
				}
				//
				inlineFunctionSignatureStartCandidate = string::npos;
				inlineFunctionLineIdxCandidate = LINE_NONE;
			} else
			// map/set initializer
			if (c == '{' && squareBracketCount == 0 && bracketCount == 0) {
				// TODO: unexpected character if beeing in key parse mode
				// increase square bracket count
				curlyBracketCount++;
				// we have a inner map/set initializer, mark it
				if (curlyBracketCount == 2) {
					if (mapValueStart == string::npos) mapValueStart = i;
				}
			} else
			// end of map/set initializer or inline lamda function
			if (c == '}' && squareBracketCount == 0 && bracketCount == 0) {
				// TODO: unexpected character if beeing in key parse mode
				curlyBracketCount--;
				// done? insert into map
				if (curlyBracketCount == 0) {
					// TODO: use parse mode here
					// first guess, we have a value to finish
					if (mapValueStart != string::npos) {
						mapValueEnd = i - 1;
					} else
					// or a key if parsing a set
					if (mapKeyStart != string::npos) {
						mapKeyEnd = i - 1;
					}
					// insert map key value pair
					insertMapKeyValuePair();
				} else
				// otherwise push inner map initializer
				if (curlyBracketCount == 1) {
					// parse and insert into map
					string_view mapKey;
					auto dequotedMapKey = false;
					// map key
					if (mapKeyStart != string::npos) {
						if (mapKeyEnd == string::npos) mapKeyEnd = i;
						auto mapKeyLength = mapKeyEnd - mapKeyStart + 1;
						if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[mapKeyStart], mapKeyLength));
						if (viewIsStringLiteral(mapKey) == true) {
							mapKey = dequote(mapKey);
							dequotedMapKey = true;
						}
					}
					//
					mapKeyStart = string::npos;
					mapKeyEnd = string::npos;
					// validate map key
					if (mapKey.empty() == true || viewIsKey(mapKey) == false) {
						_Console::printLine(minitScript->getStatementInformation(statement, mapKeyLineIdx) + ": Invalid key name, ignoring map entry: " + string(mapKey));
					} else {
						auto _private = dequotedMapKey == true?false:viewIsKeyPrivate(mapKey);
						if (_private == true) mapKey = viewGetPrivateKey(mapKey);
						//
						inlineFunctionSignatureStartCandidate = string::npos;
						inlineFunctionLineIdxCandidate = LINE_NONE;
						// map value
						if (mapValueStart != string::npos) {
							mapValueEnd = i;
							auto mapValueLength = mapValueEnd - mapValueStart + 1;
							if (mapValueLength > 0) {
								auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[mapValueStart], mapValueLength));
								if (mapValueStringView.empty() == false) {
									vector<string_view> lamdaFunctionArguments;
									string_view lamdaFunctionScriptCode;
									int lamdaFunctionLineIdx = inlineFunctionLineIdx;
									if (viewIsLamdaFunction(mapValueStringView, lamdaFunctionArguments, lamdaFunctionScriptCode, lamdaFunctionLineIdx) == true) {
										Variable mapValue;
										minitScript->createLamdaFunction(mapValue, lamdaFunctionArguments, lamdaFunctionScriptCode, lamdaFunctionLineIdx, true, statement, string(mapKey));
										variable.setMapEntry(string(mapKey), mapValue, _private);
										//
										hasValues = true;
									} else {
										// map/set
										auto mapValue = initializeMapSet(mapValueStringView, minitScript, scriptIdx, statement);
										variable.setMapEntry(string(mapKey), mapValue, _private);
										//
										hasValues = true;
									}

								}
							}
							//
							mapValueStart = string::npos;
							mapValueEnd = string::npos;
						}
						//
						inlineFunctionLineIdx = LINE_NONE;
					}
					//
					parseMode = PARSEMODE_KEY;
					mapKeyLineIdx = LINE_NONE;
				}
			} else
			// array initializer
			if (c == '[' && curlyBracketCount == 1 && bracketCount == 0) {
				// we have a inner array initializer, mark it
				if (squareBracketCount == 0) {
					if (mapValueStart == string::npos) mapValueStart = i;
				}
				// increase square bracket count
				squareBracketCount++;
			} else
			// end of array initializer
			if (c == ']' && squareBracketCount == 1 && curlyBracketCount == 1 && bracketCount == 0) {
				squareBracketCount--;
				// otherwise push inner array initializer
				if (squareBracketCount == 0 && mapValueStart != string::npos && initializerString[mapValueStart] == '[') {
					// parse and insert into map
					string_view mapKey;
					auto dequotedMapKey = false;
					// map key
					if (mapKeyStart != string::npos) {
						auto mapKeyLength = mapKeyEnd - mapKeyStart + 1;
						if (mapKeyLength > 0) mapKey = _StringTools::viewTrim(string_view(&initializerString[mapKeyStart], mapKeyLength));
						if (viewIsStringLiteral(mapKey) == true) {
							mapKey = dequote(mapKey);
							dequotedMapKey = true;
						}
					}
					//
					mapKeyStart = string::npos;
					mapKeyEnd = string::npos;
					// validate map key
					if (mapKey.empty() == true || viewIsKey(mapKey) == false) {
						_Console::printLine(minitScript->getStatementInformation(statement, mapKeyLineIdx) + ": Invalid key name, ignoring map entry: " + string(mapKey));
					} else {
						auto _private = dequotedMapKey == true?false:viewIsKeyPrivate(mapKey);
						if (_private == true) mapKey = viewGetPrivateKey(mapKey);
						//
						inlineFunctionSignatureStartCandidate = string::npos;
						inlineFunctionLineIdxCandidate = LINE_NONE;
						// array value
						if (mapValueStart != string::npos) {
							mapValueEnd = i;
							auto mapValueLength = mapValueEnd - mapValueStart + 1;
							if (mapValueLength > 0) {
								auto mapValueStringView = _StringTools::viewTrim(string_view(&initializerString[mapValueStart], mapValueLength));
								if (mapValueStringView.empty() == false) {
									auto mapValue = initializeArray(mapValueStringView, minitScript, scriptIdx, statement);
									variable.setMapEntry(string(mapKey), mapValue, _private);
									//
									hasValues = true;
								}
							}
							//
							mapValueStart = string::npos;
							mapValueEnd = string::npos;
						}
						//
						inlineFunctionLineIdx = LINE_NONE;
					}
					//
					parseMode = PARSEMODE_KEY;
					mapKeyLineIdx = LINE_NONE;
				}
			} else
			// set up map key  start
			if (curlyBracketCount == 1 && squareBracketCount == 0 && bracketCount == 0 && c != ' ' && c != '\t' && c != '\n') {
				if (parseMode == PARSEMODE_KEY && mapKeyStart == string::npos) {
					mapKeyStart = i;
					mapKeyLineIdx = subLineIdx;
				} else
				if (parseMode == PARSEMODE_VALUE && mapValueStart == string::npos) {
					mapValueStart = i;
				}
			}
		}
		//
		lc = lc == '\\' && c == '\\'?'\0':c;
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
	auto initalizer = make_unique<MinitScript::Variable::Initializer>(string(initializerString), statement, nullptr);
	variable.ir.initializer->copy(initalizer.get());
	//
	return variable;
}

void MinitScript::Variable::setFunctionCallStatement(const string& initializerStatement, MinitScript* minitScript, int scriptIdx, const Statement& statement) {
	setType(TYPE_FUNCTION_CALL);
	getStringValueReference().setValue(initializerStatement);
	//
	Statement initializerScriptStatement(
		statement.line,
		statement.statementIdx,
		initializerStatement,
		initializerStatement,
		MinitScript::STATEMENTIDX_NONE
	);
	//
	string_view methodName;
	vector<ParserArgument> arguments;
	string accessObjectMemberStatement;
	SyntaxTreeNode* evaluateSyntaxTree = new SyntaxTreeNode();
	if (minitScript->parseStatement(initializerStatement, methodName, arguments, initializerScriptStatement, accessObjectMemberStatement) == false) {
		//
	} else
	if (minitScript->createStatementSyntaxTree(scriptIdx, methodName, arguments, initializerScriptStatement, *evaluateSyntaxTree) == false) {
		//
	} else {
		getInitializerReference() = new Initializer(initializerStatement, statement, evaluateSyntaxTree);
	}
}

inline MinitScript::Variable* MinitScript::evaluateVariableAccessIntern(MinitScript::Variable* variablePtr, const string& variableStatement, const string& callerMethod, Variable*& parentVariable, int64_t& arrayIdx, string& key, int& setAccessBool, const Statement* statement, bool expectVariable) {
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
			//
			bool privateParentScope = variablePtr->isPrivateScope();
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
						//
						if (variablePtr->isPrivate() == true && privateParentScope == false) {
							//
							_Console::printLine(getStatementInformation(*statement) + ": Private variable: " + variableStatement + ": access not allowed from outside of object");
							//
							parentVariable = nullptr;
							return nullptr;
						}
					} else {
						if (expectVariable == true) {
							_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": key not found: '" + key + "'");
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
					_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": map/set access operator, but variable is not of type map/set");
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
					_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": index out of bounds: 0 <= " + to_string(arrayIdx) + " < " + to_string(variablePtr->getArrayValueReference().size()));
					return nullptr;
				}
			} else {
				_Console::printLine((statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": access operator, expected array, but got: " + variablePtr->getValueAsString());
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

			//
			if (parentVariable != nullptr && parentVariable->isPrivateScope() == true) privateParentScope = true;
		}
		//
		return variablePtr;
	}
}

inline void MinitScript::setVariableInternal(const string& variableStatement, Variable* parentVariable, Variable* variablePtr, int64_t arrayIdx, const string& key, const Variable& variable, const Statement* statement, bool createReference) {
	// common case
	if (variablePtr != nullptr) {
		if (variablePtr->isConstant() == false) {
			if (createReference == true) {
				variablePtr->setReference(&variable);
			} else {
				variablePtr->setValue(variable);
			}
		} else {
			_Console::printLine(getStatementInformation(*statement) + ": Constant: " + variableStatement + ": assignment of constant is not allowed");
		}
		return;
	} else
	// array add operator
	if (key.empty() == false) {
		if (parentVariable == nullptr) {
			// no op, we complain somewhere else, lol
		} else
		// all checks passed, push to map
		if (parentVariable->getType() == MinitScript::TYPE_MAP) {
			// check if our parent is not a const variable
			if (parentVariable->isConstant() == false) {
				parentVariable->setMapEntry(key, createReference == false?Variable::createNonReferenceVariable(&variable):Variable::createReferenceVariable(&variable));
			} else {
				_Console::printLine(getStatementInformation(*statement) + ": Constant: " + variableStatement + ": assignment of constant is not allowed");
			}
		} else
		if (parentVariable->getType() == MinitScript::TYPE_SET) {
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
					_Console::printLine("MinitScript::" + callerMethod + "(): '" + scriptFileName + "': Variable: " + variableStatement + ": set access operator: expected boolean variable to remove/insert key in set, but got " + variable.getTypeAsString());
				}
			} else {
				_Console::printLine(getStatementInformation(*statement) + ": Constant: " + variableStatement + ": assignment of constant is not allowed");
			}
		} else {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MinitScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": map/set access operator: expected map/set, but got " + parentVariable->getTypeAsString() + ": '" + key + "'");
		}
		//
		return;
	} else
	if (arrayIdx == ARRAYIDX_ADD) {
		if (parentVariable == nullptr) {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MinitScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": [] array push operator without array");
		} else
		if (parentVariable->getType() == MinitScript::TYPE_ARRAY) {
			// check if our parent is not a const variable
			if (parentVariable->isConstant() == false) {
				// all checks passed, push variable to array
				parentVariable->pushArrayEntry(createReference == false?Variable::createNonReferenceVariable(&variable):Variable::createReferenceVariable(&variable));
			} else {
				_Console::printLine(getStatementInformation(*statement) + ": Constant: " + variableStatement + ": assignment of constant is not allowed");
			}
		} else {
			string callerMethod = __FUNCTION__;
			_Console::printLine("MinitScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": [] array push operator: expected array, but got " + parentVariable->getTypeAsString());
		}
		//
		return;
	}
}

inline bool MinitScript::evaluateInternal(const string& statement, const string& executableStatement, Variable& returnValue, bool pushScriptState) {
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
	vector<ParserArgument> arguments;
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
		returnValue.setValue(
			executeStatement(
				evaluateSyntaxTree,
				evaluateStatement
			)
		);
		//
		if (pushScriptState == true) popScriptState();
		//
		return true;
	}
}

inline const MinitScript::Variable MinitScript::initializeVariable(const Variable& variable) {
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

inline bool MinitScript::viewIsKey(const string_view& candidate) {
	if (candidate.empty() == true) return false;
	auto i = 0;
	if (candidate[i] == '-') i++;
	for (; i < candidate.size(); i++) {
		auto c = candidate[i];
		if (_Character::isAlphaNumeric(c) == false && c != '_') return false;
	}
	return true;
}

inline bool MinitScript::viewIsKeyPrivate(const string_view& candidate) {
	if (candidate.empty() == true) return false;
	if (candidate[0] == '-') return true;
	return false;
}

inline const string_view MinitScript::viewGetPrivateKey(const string_view& candidate) {
	return string_view(&candidate.data()[1], candidate.size() - 1);
}

inline bool MinitScript::getVariableAccessOperatorLeftRightIndices(const string& variableStatement, const string& callerMethod, string::size_type& accessOperatorLeftIdx, string::size_type& accessOperatorRightIdx, const Statement* statement, int startIdx) {
	accessOperatorLeftIdx = string::npos;
	accessOperatorRightIdx = string::npos;
	auto haveKey = false;
	auto squareBracketsCount = 0;
	// improve me!
	if (startIdx > 0) {
		haveKey = variableStatement[startIdx - 1] == '.';
		if (haveKey == true) accessOperatorLeftIdx = startIdx - 1;
	} else
	if (startIdx == 0) {
		//
		auto lc = '\0';
		for (auto i = 0; i < variableStatement.length(); i++) {
			auto c = variableStatement[i];
			if (lc == ':' && c == ':') {
				startIdx = i + 1;
				break;
			}
			//
			lc = c;
		}
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
				_Console::printLine("MinitScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": unexpected char: ']'");
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
				_Console::printLine("MinitScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": unexpected char: ']'");
				return false;
			}
		}
	}
	//
	if (haveKey == true) accessOperatorRightIdx = variableStatement.size();
	//
	return true;
}

inline bool MinitScript::evaluateAccess(const string& variableStatement, const string& callerMethod, string::size_type& arrayAccessOperatorLeftIdx, string::size_type& arrayAccessOperatorRightIdx, int64_t& arrayIdx, string& key, const Statement* statement) {
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
				_Console::printLine("MinitScript::" + callerMethod + "(): " + (statement != nullptr?getStatementInformation(*statement):scriptFileName) + ": Variable: " + variableStatement + ": failed to evaluate expression: '" + string(arrayIdxExpressionStringView) + "'");
				return false;
			}
		}
	} else {
		arrayIdx = ARRAYIDX_ADD;
	}
	//
	return true;
}

void MinitScript::setConstantInternal(Variable& variable) {
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

void MinitScript::unsetConstantInternal(Variable& variable) {
	if (variable.isReference() == true) {
		_Console::printLine("MinitScript::unsetConstantInternal(): Can not unset constant if reference variable is given.");
		return;
	}
	variable.unsetConstant();
	switch (variable.getType()) {
		case TYPE_ARRAY:
			{
				auto arrayPointer = variable.getArrayPointer();
				if (arrayPointer == nullptr) break;
				for (const auto arrayEntry: *arrayPointer) {
					unsetConstant(*arrayEntry);
				}
				//
				break;
			}
		case TYPE_MAP:
			{
				auto mapPointer = variable.getMapPointer();
				if (mapPointer == nullptr) break;
				for (const auto& [mapKey, mapValue]: *mapPointer) {
					unsetConstant(*mapValue);
				}
				//
				break;
			}
		default:
			break;
	}
}

void MinitScript::garbageCollection() {
	auto garbageCollectionDataTypesIndicesCopy = garbageCollectionDataTypesIndices;
	for (auto index: garbageCollectionDataTypesIndicesCopy) {
		auto& garbageCollectionDataType = garbageCollectionDataTypes[index];
		garbageCollectionDataType.dataType->garbageCollection(garbageCollectionDataType.context);
	}
}
