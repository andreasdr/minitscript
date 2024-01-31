#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/BaseMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Hex.h>
#include <miniscript/utilities/Time.h>

using std::span;

using miniscript::miniscript::BaseMethods;

using miniscript::miniscript::MiniScript;

using _Console = miniscript::utilities::Console;
using _Hex = miniscript::utilities::Hex;
using _Time = miniscript::utilities::Time;

void BaseMethods::registerConstants(MiniScript* miniScript) {
}

void BaseMethods::registerMethods(MiniScript* miniScript) {
	// base methods
	{
		//
		class MethodReturn: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodReturn(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "return";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 0 || arguments.size() == 1) {
					if (miniScript->isFunctionRunning() == false) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "No function is being executed, return($value) has no effect");
					} else
					if (arguments.size() == 1) miniScript->getScriptState().returnValue = arguments[0];
					miniScript->stopRunning();
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodReturn(miniScript));
	}
	{
		//
		class MethodBreak: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodBreak(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "levels", .optional = true, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "break";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t levels = 1;
				if ((arguments.size() == 0 || arguments.size() == 1) &&
					MiniScript::getIntegerValue(arguments, 0, levels, true) == true) {
					if (miniScript->getScriptState().blockStack.empty() == true) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "break without forCondition/forTime");
					} else {
						int64_t level = 0;
						auto& blockStack = miniScript->getScriptState().blockStack;
						MiniScript::ScriptState::Block* endType = nullptr;
						vector<int> blockStacksToRemove;
						for (int i = blockStack.size() - 1; i >= 0; i--) {
							if (blockStack[i].type == MiniScript::ScriptState::BLOCKTYPE_FOR) {
								endType = &blockStack[i];
								level++;
								blockStacksToRemove.push_back(i);
								if (level == levels) break;
							} else
							if (level < levels) {
								blockStacksToRemove.push_back(i);
							}
						}
						if (endType == nullptr) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "break without forCondition, forTime");
						} else
						if (levels != level) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "break(" + to_string(levels) + ") + without " + to_string(levels) + " levels of forCondition, forTime");
						} else
						if (endType->breakStatement != nullptr) {
							auto breakStatement = endType->breakStatement;
							for (auto i: blockStacksToRemove) blockStack.erase(blockStack.begin() + i);
							miniScript->gotoStatement(*breakStatement);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "No break statement");
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodBreak(miniScript));
	}
	{
		//
		class MethodContinue: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodContinue(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "levels", .optional = true, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "continue";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t levels = 1;
				if ((arguments.size() == 0 || arguments.size() == 1) &&
					MiniScript::getIntegerValue(arguments, 0, levels, true) == true) {
					if (miniScript->getScriptState().blockStack.empty() == true) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "continue without forCondition, forTime");
					} else {
						int64_t level = 0;
						auto& blockStack = miniScript->getScriptState().blockStack;
						MiniScript::ScriptState::Block* endType = nullptr;
						vector<int> blockStacksToRemove;
						for (int i = blockStack.size() - 1; i >= 0; i--) {
							if (blockStack[i].type == MiniScript::ScriptState::BLOCKTYPE_FOR) {
								endType = &blockStack[i];
								level++;
								if (level == levels) {
									break;
								} else {
									blockStacksToRemove.push_back(i);
								}
							} else
							if (level < levels) {
								blockStacksToRemove.push_back(i);
							}
						}
						if (endType == nullptr) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "continue without forCondition, forTime");
						} else
						if (levels != level) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "continue(" + to_string(levels) + ") + without " + to_string(levels) + " levels of forCondition, forTime");
						} else
						if (endType->continueStatement != nullptr) {
							auto continueStatement = endType->continueStatement;
							for (auto i: blockStacksToRemove) blockStack.erase(blockStack.begin() + i);
							miniScript->gotoStatement(*continueStatement);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "No continue statement");
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodContinue(miniScript));
	}
	{
		//
		class MethodEnd: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodEnd(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "end";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 0) {
					if (miniScript->getScriptState().blockStack.empty() == true) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "end without if/elseif/else/switch/case/default/forCondition/forTime/end");
					} else {
						auto& blockStack = miniScript->getScriptState().blockStack;
						auto& block = blockStack.back();
						if ((block.type == MiniScript::ScriptState::BLOCKTYPE_FUNCTION || block.type == MiniScript::ScriptState::BLOCKTYPE_STACKLET) &&
							miniScript->scriptStateStack.size() <= 2) {
							miniScript->stopRunning();
						} else
						if (block.type ==  MiniScript::ScriptState::BLOCKTYPE_FOR && block.parameter.getType() == MiniScript::TYPE_INTEGER) {
							vector<MiniScript::Variable> arguments {};
							span argumentsSpan(arguments);
							MiniScript::Variable returnValue;
							int64_t iterationStackletFunctionIdx;
							if (block.parameter.getIntegerValue(iterationStackletFunctionIdx) == true &&
								iterationStackletFunctionIdx != MiniScript::SCRIPTIDX_NONE) {
								miniScript->callStacklet(iterationStackletFunctionIdx, argumentsSpan, returnValue);
							} else {
								MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "end with for: iteration stacklet: invalid stacklet");
							}
						}
						blockStack.erase(blockStack.begin() + blockStack.size() - 1);
						if (statement.gotoStatementIdx != MiniScript::STATEMENTIDX_NONE) {
							miniScript->gotoStatementGoto(statement);
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodEnd(miniScript));
	}
	{
		//
		class MethodForTime: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodForTime(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "time", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "forTime";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t time;
				if (arguments.size() == 1 &&
					miniScript->getIntegerValue(arguments, 0, time) == true) {
					auto& scriptState = miniScript->getScriptState();
					auto now = _Time::getCurrentMillis();
					auto timeWaitStarted = now;
					auto forTimeStartedIt = scriptState.forTimeStarted.find(statement.line);
					if (forTimeStartedIt == scriptState.forTimeStarted.end()) {
						miniScript->getScriptState().forTimeStarted[statement.line] = timeWaitStarted;
					} else {
						timeWaitStarted = forTimeStartedIt->second;
					}
					//
					if (_Time::getCurrentMillis() > timeWaitStarted + time) {
						scriptState.forTimeStarted.erase(statement.line);
						miniScript->gotoStatementGoto(statement);
					} else {
						scriptState.blockStack.emplace_back(
							MiniScript::ScriptState::BLOCKTYPE_FORTIME,
							false,
							&miniScript->getScripts()[scriptState.scriptIdx].statements[statement.gotoStatementIdx - 1],
							&miniScript->getScripts()[scriptState.scriptIdx].statements[statement.gotoStatementIdx],
							MiniScript::Variable()
						);
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodForTime(miniScript));
	}
	{
		//
		class MethodForCondition: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodForCondition(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "condition", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STACKLET_ASSIGNMENT, .name = "iterationStacklet", .optional = true, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "forCondition";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValue;
				string iterationStacklet;
				if ((arguments.size() == 1 || arguments.size() == 2) &&
					miniScript->getBooleanValue(arguments, 0, booleanValue) == true &&
					miniScript->getStackletValue(arguments, 1, iterationStacklet, true) == true) {
					if (booleanValue == false) {
						miniScript->gotoStatementGoto(statement);
					} else {
						// TODO: we need to cache later the function/script index instead of the function name
						auto iterationStackletFunctionIdx =
							iterationStacklet.empty() == true?
								MiniScript::SCRIPTIDX_NONE:
								miniScript->getFunctionScriptIdx(iterationStacklet);
						// check if valid
						if (iterationStacklet.empty() == false && iterationStackletFunctionIdx == MiniScript::SCRIPTIDX_NONE) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "Stacklet not found: " + iterationStacklet);
						} else {
							auto& scriptState = miniScript->getScriptState();
							scriptState.blockStack.emplace_back(
								MiniScript::ScriptState::BLOCKTYPE_FOR,
								false,
								&miniScript->getScripts()[scriptState.scriptIdx].statements[statement.gotoStatementIdx - 1],
								&miniScript->getScripts()[scriptState.scriptIdx].statements[statement.gotoStatementIdx],
								iterationStacklet.empty() == true?MiniScript::Variable():MiniScript::Variable(static_cast<int64_t>(iterationStackletFunctionIdx))
							);
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodForCondition(miniScript));
	}
	{
		//
		class MethodIfCondition: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodIfCondition(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "condition", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "if";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValue;
				if (arguments.size() == 1 &&
					miniScript->getBooleanValue(arguments, 0, booleanValue) == true) {
					miniScript->getScriptState().blockStack.emplace_back(MiniScript::ScriptState::BLOCKTYPE_IF, booleanValue, nullptr, nullptr, MiniScript::Variable());
					if (booleanValue == false) {
						miniScript->gotoStatementGoto(statement);
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodIfCondition(miniScript));
	}
	{
		//
		class MethodElseIfCondition: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodElseIfCondition(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "condition", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "elseif";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValue;
				if (arguments.size() == 1 &&
					miniScript->getBooleanValue(arguments, 0, booleanValue) == true) {
					auto& scriptState = miniScript->getScriptState();
					if (miniScript->getScriptState().blockStack.empty() == true) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "elseif without if");
					} else {
						auto& block = scriptState.blockStack.back();
						if (block.type != MiniScript::ScriptState::BlockType::BLOCKTYPE_IF) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "elseif without if");
						} else
						if (block.match == true || booleanValue == false) {
							miniScript->gotoStatementGoto(statement);
						} else {
							block.match = booleanValue;
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodElseIfCondition(miniScript));
	}
	{
		//
		class MethodElse: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodElse(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "else";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 0) {
					auto& scriptState = miniScript->getScriptState();
					auto& block = scriptState.blockStack.back();
					if (block.type != MiniScript::ScriptState::BlockType::BLOCKTYPE_IF) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "else without if");
					} else
					if (block.match == true) {
						miniScript->gotoStatementGoto(statement);
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodElse(miniScript));
	}
	// switch
	{
		//
		class MethodSwitch: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodSwitch(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "switch";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 1) {
					auto& scriptState = miniScript->getScriptState();
					scriptState.blockStack.emplace_back(MiniScript::ScriptState::BLOCKTYPE_SWITCH, false, nullptr, nullptr, arguments[0]);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodSwitch(miniScript));
	}
	{
		//
		class MethodCase: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodCase(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "case";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 1) {
					auto& scriptState = miniScript->getScriptState();
					if (miniScript->getScriptState().blockStack.empty() == true) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "case without switch");
					} else {
						auto& block = scriptState.blockStack.back();
						if (block.type != MiniScript::ScriptState::BlockType::BLOCKTYPE_SWITCH) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "case without switch");
						} else {
							auto match = arguments[0].getValueAsString() == block.parameter.getValueAsString();
							if (block.match == true || match == false) {
								miniScript->gotoStatementGoto(statement);
							} else {
								block.match = match;
								scriptState.blockStack.emplace_back(MiniScript::ScriptState::BLOCKTYPE_CASE, false, nullptr, nullptr, MiniScript::Variable());
							}
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodCase(miniScript));
	}
	{
		//
		class MethodDefault: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodDefault(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "default";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 0) {
					auto& scriptState = miniScript->getScriptState();
					if (miniScript->getScriptState().blockStack.empty() == true) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "default without switch");
					} else {
						auto& block = scriptState.blockStack.back();
						if (block.type != MiniScript::ScriptState::BlockType::BLOCKTYPE_SWITCH) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "default without switch");
						} else
						if (block.match == true) {
							miniScript->gotoStatementGoto(statement);
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodDefault(miniScript));
	}
	// equality
	{
		//
		class MethodEquals: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodEquals(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "equals";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 2) {
					returnValue.setValue(true);
					for (auto i = 1; i < arguments.size(); i++) {
						if (arguments[0].getValueAsString() != arguments[i].getValueAsString()) {
							returnValue.setValue(false);
							break;
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_EQUALS;
			}
		};
		miniScript->registerMethod(new MethodEquals(miniScript));
	}
	{
		//
		class MethodNotEqual: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodNotEqual(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "notEqual";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 2) {
					returnValue.setValue(true);
					for (auto i = 1; i < arguments.size(); i++) {
						if (arguments[0].getValueAsString() == arguments[i].getValueAsString()) {
							returnValue.setValue(false);
							break;
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_NOTEQUAL;
			}
		};
		miniScript->registerMethod(new MethodNotEqual(miniScript));
	}
	// int methods
	{
		//
		class MethodInt: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodInt(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "integer", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "integer";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t integerValue;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, integerValue) == true) {
					returnValue.setValue(integerValue);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodInt(miniScript));
	}
	// float methods
	//	TODO: move me into FloatMethods
	{
		//
		class MethodFloat: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodFloat(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_FLOAT, .name = "float", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_FLOAT
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "float";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float floatValue;
				if (arguments.size() == 1 &&
					MiniScript::getFloatValue(arguments, 0, floatValue) == true) {
					returnValue.setValue(floatValue);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodFloat(miniScript));
	}
	{
		//
		class MethodFloatToIntValue: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodFloatToIntValue(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_FLOAT, .name = "float", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "float.toIntegerValue";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				float floatValue;
				if (arguments.size() == 1 &&
					MiniScript::getFloatValue(arguments, 0, floatValue) == true) {
					returnValue.setValue(static_cast<int64_t>(*((uint32_t*)&floatValue)));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodFloatToIntValue(miniScript));
	}
	{
		//
		class MethodFloatfromIntValue: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodFloatfromIntValue(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "integer", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_FLOAT
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "float.fromIntegerValue";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t intValue;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, intValue) == true) {
					returnValue.setValue(*((float*)&intValue));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodFloatfromIntValue(miniScript));
	}
	//
	{
		//
		class MethodGreater: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodGreater(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "greater";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 2) {
					if (MiniScript::hasType(arguments, MiniScript::TYPE_STRING) == true) {
						string stringValueA;
						string stringValueB;
						if (MiniScript::getStringValue(arguments, 0, stringValueA, false) == true &&
							MiniScript::getStringValue(arguments, 1, stringValueB, false) == true) {
							returnValue.setValue(stringValueA > stringValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					} else {
						float floatValueA;
						float floatValueB;
						if (MiniScript::getFloatValue(arguments, 0, floatValueA, false) == true &&
							MiniScript::getFloatValue(arguments, 1, floatValueB, false) == true) {
							returnValue.setValue(floatValueA > floatValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_GREATER;
			}
		};
		miniScript->registerMethod(new MethodGreater(miniScript));
	}
	{
		//
		class MethodGreaterEquals: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodGreaterEquals(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "greaterEquals";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 2) {
					if (MiniScript::hasType(arguments, MiniScript::TYPE_STRING) == true) {
						string stringValueA;
						string stringValueB;
						if (MiniScript::getStringValue(arguments, 0, stringValueA, false) == true &&
							MiniScript::getStringValue(arguments, 1, stringValueB, false) == true) {
							returnValue.setValue(stringValueA >= stringValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					} else {
						float floatValueA;
						float floatValueB;
						if (MiniScript::getFloatValue(arguments, 0, floatValueA, false) == true &&
							MiniScript::getFloatValue(arguments, 1, floatValueB, false) == true) {
							returnValue.setValue(floatValueA >= floatValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_GREATEREQUALS;
			}
		};
		miniScript->registerMethod(new MethodGreaterEquals(miniScript));
	}
	{
		//
		class MethodLesser: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodLesser(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "lesser";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 2) {
					if (MiniScript::hasType(arguments, MiniScript::TYPE_STRING) == true) {
						string stringValueA;
						string stringValueB;
						if (MiniScript::getStringValue(arguments, 0, stringValueA, false) == true &&
							MiniScript::getStringValue(arguments, 1, stringValueB, false) == true) {
							returnValue.setValue(stringValueA < stringValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					} else {
						float floatValueA;
						float floatValueB;
						if (MiniScript::getFloatValue(arguments, 0, floatValueA, false) == true &&
							MiniScript::getFloatValue(arguments, 1, floatValueB, false) == true) {
							returnValue.setValue(floatValueA < floatValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_LESSER;
			}
		};
		miniScript->registerMethod(new MethodLesser(miniScript));
	}
	{
		//
		class MethodLesserEquals: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodLesserEquals(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "lesserEquals";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 2) {
					if (MiniScript::hasType(arguments, MiniScript::TYPE_STRING) == true) {
						string stringValueA;
						string stringValueB;
						if (MiniScript::getStringValue(arguments, 0, stringValueA, false) == true &&
							MiniScript::getStringValue(arguments, 1, stringValueB, false) == true) {
							returnValue.setValue(stringValueA <= stringValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					} else {
						float floatValueA;
						float floatValueB;
						if (MiniScript::getFloatValue(arguments, 0, floatValueA, false) == true &&
							MiniScript::getFloatValue(arguments, 1, floatValueB, false) == true) {
							returnValue.setValue(floatValueA <= floatValueB);
						} else {
							MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_LESSEREQUALS;
			}
		};
		miniScript->registerMethod(new MethodLesserEquals(miniScript));
	}
	// bool methods
	{
		//
		class MethodBool: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodBool(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "boolean", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "boolean";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValue;
				if (arguments.size() == 1 &&
					MiniScript::getBooleanValue(arguments, 0, booleanValue) == true) {
					returnValue.setValue(booleanValue);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodBool(miniScript));
	}
	{
		//
		class MethodNot: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodNot(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "boolean", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN), miniScript(miniScript) {}
			const string getMethodName() override {
				return "not";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValue = false;
				if (arguments.size() == 1 &&
					MiniScript::getBooleanValue(arguments, 0, booleanValue) == true) {
					returnValue.setValue(!booleanValue);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_NOT;
			}
		};
		miniScript->registerMethod(new MethodNot(miniScript));
	}
	{
		//
		class MethodAnd: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodAnd(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "and";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValueA;
				bool booleanValueB;
				if (arguments.size() == 2 &&
					MiniScript::getBooleanValue(arguments, 0, booleanValueA) == true &&
					MiniScript::getBooleanValue(arguments, 1, booleanValueB) == true) {
					returnValue.setValue(booleanValueA && booleanValueB);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_AND;
			}
		};
		miniScript->registerMethod(new MethodAnd(miniScript));
	}
	{
		//
		class MethodOr: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodOr(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "or";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				bool booleanValueA;
				bool booleanValueB;
				if (arguments.size() == 2 &&
					MiniScript::getBooleanValue(arguments, 0, booleanValueA) == true &&
					MiniScript::getBooleanValue(arguments, 1, booleanValueB) == true) {
					returnValue.setValue(booleanValueA || booleanValueB);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_OR;
			}
		};
		miniScript->registerMethod(new MethodOr(miniScript));
	}
	// get variable
	{
		//
		class MethodGetVariable: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodGetVariable(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "variable", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "getVariable";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string variable;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, variable) == true) {
					returnValue = miniScript->getVariable(variable, &statement);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodGetVariable(miniScript));
	}
	// get variable reference
	{
		//
		class MethodGetVariableReference: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodGetVariableReference(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "variable", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "getVariableReference";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string variable;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, variable) == true) {
					returnValue = miniScript->getVariable(variable, &statement, true);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			bool isPrivate() const override {
				return true;
			}
		};
		miniScript->registerMethod(new MethodGetVariableReference(miniScript));
	}
	// set variable
	{
		//
		class MethodSetVariable: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodSetVariable(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "variable", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "setVariable";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string variable;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, variable) == true) {
					miniScript->setVariable(variable, arguments[1], &statement);
					returnValue = arguments[1];
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_SET;
			}
		};
		miniScript->registerMethod(new MethodSetVariable(miniScript));
	}
	// set constant
	{
		//
		class MethodSetConstant: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodSetConstant(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "constant", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "setConstant";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string constant;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, constant) == true) {
					MiniScript::setConstant(arguments[1]);
					miniScript->setVariable(constant, arguments[1], &statement);
					returnValue = arguments[1];
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodSetConstant(miniScript));
	}
	{
		//
		class MethodPostfixIncrement: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodPostfixIncrement(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "variable", .optional = false, .reference = true, .nullable = false },
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "postfixIncrement";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t value;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, value) == true) {
					arguments[0].setValue(value + 1);
					returnValue.setValue(value);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_POSTFIX_INCREMENT;
			}
		};
		miniScript->registerMethod(new MethodPostfixIncrement(miniScript));
	}
	{
		//
		class MethodPostfixDecrement: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodPostfixDecrement(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "variable", .optional = false, .reference = true, .nullable = false },
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "postfixDecrement";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t value;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, value) == true) {
					arguments[0].setValue(value - 1);
					returnValue.setValue(value);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_POSTFIX_DECREMENT;
			}
		};
		miniScript->registerMethod(new MethodPostfixDecrement(miniScript));
	}
	{
		//
		class MethodPrefixIncrement: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodPrefixIncrement(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "variable", .optional = false, .reference = true, .nullable = false },
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "prefixIncrement";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t value;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, value) == true) {
					++value;
					arguments[0].setValue(value);
					returnValue.setValue(value);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_PREFIX_INCREMENT;
			}
		};
		miniScript->registerMethod(new MethodPrefixIncrement(miniScript));
	}
	{
		//
		class MethodPrefixDecrement: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodPrefixDecrement(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "variable", .optional = false, .reference = true, .nullable = false },
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "prefixDecrement";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t value;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, value) == true) {
					--value;
					arguments[0].setValue(value);
					returnValue.setValue(value);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_PREFIX_DECREMENT;
			}
		};
		miniScript->registerMethod(new MethodPrefixDecrement(miniScript));
	}
	//
	{
		//
		class MethodBitwiseNot: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodBitwiseNot(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "value", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_INTEGER),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "bitwiseNot";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t value;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, value) == true) {
					returnValue.setValue(~value);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_BITWISENOT;
			}
		};
		miniScript->registerMethod(new MethodBitwiseNot(miniScript));
	}
	//
	{
		class MethodBitwiseAnd: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodBitwiseAnd(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "bitwiseAnd";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t valueA;
				int64_t valueB;
				if (MiniScript::getIntegerValue(arguments, 0, valueA) == true &&
					MiniScript::getIntegerValue(arguments, 1, valueB) == true) {
					returnValue.setValue(valueA & valueB);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_BITWISEAND;
			}
		};
		miniScript->registerMethod(new MethodBitwiseAnd(miniScript));
	}
	//
	{
		class MethodBitwiseOr: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodBitwiseOr(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "bitwiseOr";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t valueA;
				int64_t valueB;
				if (arguments.size() == 2 &&
					MiniScript::getIntegerValue(arguments, 0, valueA) == true &&
					MiniScript::getIntegerValue(arguments, 1, valueB) == true) {
					returnValue.setValue(valueA | valueB);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_BITWISEOR;
			}
		};
		miniScript->registerMethod(new MethodBitwiseOr(miniScript));
	}
	//
	{
		class MethodBitwiseXor: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodBitwiseXor(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "a", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "b", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER),
					miniScript(miniScript) {}
			const string getMethodName() override {
				return "bitwiseXor";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t valueA;
				int64_t valueB;
				if (arguments.size() == 2 &&
					MiniScript::getIntegerValue(arguments, 0, valueA) == true &&
					MiniScript::getIntegerValue(arguments, 1, valueB) == true) {
					returnValue.setValue(valueA ^ valueB);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			MiniScript::Operator getOperator() const override {
				return MiniScript::OPERATOR_BITWISEXOR;
			}
		};
		miniScript->registerMethod(new MethodBitwiseXor(miniScript));
	}
	// hex: move me into HexMethods
	{
		//
		class MethodHexEncode: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodHexEncode(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "value", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "hex.encode";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t value;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, value) == true) {
					returnValue.setValue(_Hex::encodeInt(value));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodHexEncode(miniScript));
	}
	{
		//
		class MethodHexDecode: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodHexDecode(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "value", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "hex.decode";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string value;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, value) == true) {
					returnValue.setValue(static_cast<int64_t>(_Hex::decodeInt(value)));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodHexDecode(miniScript));
	}
}
