#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MapMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::span;

using miniscript::miniscript::MapMethods;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;

void MapMethods::registerMethods(MiniScript* miniScript) {
	// map
	{
		//
		class ScriptMethodMap: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMap(MiniScript* miniScript):
				MiniScript::Method(
					{},
					MiniScript::TYPE_MAP
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setType(MiniScript::TYPE_MAP);
			}
		};
		miniScript->registerMethod(new ScriptMethodMap(miniScript));
	}
	{
		//
		class ScriptMethodMapSet: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapSet(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.set";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() != 3 ||
					arguments[0].getType() != MiniScript::TYPE_MAP ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					arguments[0].setMapEntry(key, arguments[2]);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapSet(miniScript));
	}
	{
		//
		class ScriptMethodMapHas: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapHas(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.has";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() < 2 ||
					arguments[0].getType() != MiniScript::TYPE_MAP ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					returnValue.setValue(arguments[0].hasMapEntry(key));
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapHas(miniScript));
	}
	{
		//
		class ScriptMethodMapGet: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapGet(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.get";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() < 2 ||
					arguments[0].getType() != MiniScript::TYPE_MAP ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					returnValue = arguments[0].getMapEntry(key);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapGet(miniScript));
	}
	{
		//
		class ScriptMethodMapRemove: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapRemove(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "key", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.remove";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string key;
				if (arguments.size() < 2 ||
					arguments[0].getType() != MiniScript::TYPE_MAP ||
					MiniScript::getStringValue(arguments, 1, key, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					arguments[0].removeMapEntry(key);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapRemove(miniScript));
	}
	{
		//
		class ScriptMethodMapGetKeys: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapGetKeys(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_ARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.getKeys";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				if (arguments.size() != 1 ||
					arguments[0].getType() != MiniScript::TYPE_MAP) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto keys = arguments[0].getMapKeys();
					returnValue.setType(MiniScript::TYPE_ARRAY);
					for (const auto& key: keys) {
						returnValue.pushArrayEntry(key);
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapGetKeys(miniScript));
	}
	{
		//
		class ScriptMethodMapGetValues: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapGetValues(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_ARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.getValues";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				if (arguments.size() != 1 ||
					arguments[0].getType() != MiniScript::TYPE_MAP) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto values = arguments[0].getMapValues();
					returnValue.setType(MiniScript::TYPE_ARRAY);
					for (const auto value: values) {
						returnValue.pushArrayEntry(*value);
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapGetValues(miniScript));
	}
	{
		//
		class ScriptMethodMapClear: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapClear(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = true, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.clear";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() != 1 || arguments[0].getType() != MiniScript::TYPE_MAP) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					arguments[0].clearMap();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodMapClear(miniScript));
	}
	{
		//
		class ScriptMethodMapForEach: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodMapForEach(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_MAP, .name = "map", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_FUNCTION_ASSIGNMENT, .name = "function", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "cookie", .optional = true, .reference = true, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "map.forEach";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string function;
				if ((arguments.size() != 2 && arguments.size() != 3) ||
					arguments[0].getType() != MiniScript::TYPE_MAP ||
					MiniScript::getStringValue(arguments, 1, function, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto mapPtr = arguments[0].getMapPointer();
					if (mapPtr != nullptr) {
						for (const auto& [mapKey, mapValue]: *mapPtr) {
							vector<MiniScript::Variable> functionArguments { MiniScript::Variable(mapKey), MiniScript::Variable::createReferenceVariable(mapValue) };
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
		miniScript->registerMethod(new ScriptMethodMapForEach(miniScript));
	}
}
