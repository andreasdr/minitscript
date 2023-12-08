#include <span>

#include <miniscript/miniscript/SetMethods.h>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::span;

using miniscript::miniscript::SetMethods;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;

void SetMethods::registerMethods(MiniScript* miniScript) {
	// sets
	{
		//
		class ScriptMethodSet: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSet(MiniScript* miniScript):
				MiniScript::Method(
					{},
					MiniScript::TYPE_SET
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setType(MiniScript::TYPE_SET);
			}
		};
		miniScript->registerMethod(new ScriptMethodSet(miniScript));
	}
	{
		//
		class ScriptMethodSetInsert: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSetInsert(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_SET, .name = "set", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set.insert";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() != 2 ||
					arguments[0].getType() != MiniScript::TYPE_SET ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					arguments[0].insertSetKey(key);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSetInsert(miniScript));
	}
	{
		//
		class ScriptMethodSetHas: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSetHas(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_SET, .name = "set", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set.has";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() < 2 ||
					arguments[0].getType() != MiniScript::TYPE_SET ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					returnValue.setValue(arguments[0].hasSetKey(key));
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSetHas(miniScript));
	}
	{
		//
		class ScriptMethodSetRemove: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSetRemove(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_SET, .name = "set", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set.remove";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() < 2 ||
					arguments[0].getType() != MiniScript::TYPE_SET ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					arguments[0].removeSetKey(key);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSetRemove(miniScript));
	}
	{
		//
		class ScriptMethodSetGetKeys: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSetGetKeys(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_SET, .name = "set", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_ARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set.getKeys";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				if (arguments.size() != 1 ||
					arguments[0].getType() != MiniScript::TYPE_SET) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto keys = arguments[0].getSetKeys();
					returnValue.setType(MiniScript::TYPE_ARRAY);
					for (const auto& key: keys) {
						returnValue.pushArrayEntry(key);
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSetGetKeys(miniScript));
	}
	{
		//
		class ScriptMethodSetClear: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSetClear(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_SET, .name = "set", .optional = false, .reference = true, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set.clear";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if ((arguments.size() != 1 || arguments[0].getType() != MiniScript::TYPE_SET)) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					arguments[0].clearSet();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSetClear(miniScript));
	}
	{
		//
		class ScriptMethodSetForEach: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodSetForEach(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_SET, .name = "set", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_FUNCTION_ASSIGNMENT, .name = "function", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "cookie", .optional = true, .reference = true, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "set.forEach";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string function;
				if ((arguments.size() != 2 && arguments.size() != 3) ||
					arguments[0].getType() != MiniScript::TYPE_SET ||
					MiniScript::getStringValue(arguments, 1, function, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto setPtr = arguments[0].getSetPointer();
					if (setPtr != nullptr) {
						for (auto setEntry: *setPtr) {
							vector<MiniScript::Variable> functionArguments { MiniScript::Variable(setEntry) };
							if (arguments.size() == 3) functionArguments.push_back(arguments[2]);
							span functionArgumentsSpan(functionArguments);
							MiniScript::Variable functionReturnValue;
							miniScript->call(function, functionArgumentsSpan, functionReturnValue);
							// exit condition
							bool result = false;
							functionReturnValue.getBooleanValue(result, false);
							if (result == true) break;
						}
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodSetForEach(miniScript));
	}
}
