#include <span>

#include <memory>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/ContextMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::span;
using std::unique_ptr;

using miniscript::miniscript::ContextMethods;

using miniscript::miniscript::MiniScript;

using _Console = miniscript::utilities::Console;

void ContextMethods::registerConstants(MiniScript* miniScript) {
}

void ContextMethods::registerMethods(MiniScript* miniScript) {
	if (miniScript->getContext() == nullptr) return;
	{
		//
		class MethodContextScriptHasCallable: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodContextScriptHasCallable(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "scriptId", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "callable", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "context.script.hasCallable";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string scriptId;
				string callable;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, scriptId) == true &&
					MiniScript::getStringValue(arguments, 1, callable) == true) {
					auto script = miniScript->getContext()->getScript(scriptId);
					if (script == nullptr) {
						returnValue.setValue(false);
					} else {
						auto scriptIdx = script->getFunctionScriptIdx(callable);
						if (scriptIdx == MiniScript::SCRIPTIDX_NONE || script->getScripts()[scriptIdx].callable == false) {
							returnValue.setValue(false);
						} else {
							returnValue.setValue(true);
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodContextScriptHasCallable(miniScript));
	}
	{
		//
		class MethodContextScriptCall: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodContextScriptCall(MiniScript* miniScript):
				Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "scriptId", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "callable", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_PSEUDO_MIXED
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "context.script.call";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string scriptId;
				string callable;
				if (arguments.size() >= 2 &&
					MiniScript::getStringValue(arguments, 0, scriptId) == true &&
					MiniScript::getStringValue(arguments, 1, callable) == true) {
					auto script = dynamic_cast<MiniScript*>(miniScript->getContext()->getScript(scriptId));
					if (script == nullptr) {
						MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "No script with given id: " + scriptId);
					} else {
						auto scriptIdx = script->getFunctionScriptIdx(callable);
						if (scriptIdx == MiniScript::SCRIPTIDX_NONE || script->getScripts()[scriptIdx].callable == false) {
							MINISCRIPT_METHODUSAGE_COMPLAINM(getMethodName(), "Callable not found: " + callable);
						} else {
							miniScript->getContext()->push(script);
							#if defined (__clang__)
								// Clang currently does not support initializing span using begin and end iterators,
								vector<MiniScript::Variable> callArguments(arguments.size() - 2);
								for (auto i = 2; i < arguments.size(); i++) callArguments[i - 2] = move(arguments[i]);
								// call
								span callArgumentsSpan(callArguments);
								script->call(scriptIdx, callArgumentsSpan, returnValue);
								// move back arguments
								for (auto i = 2; i < arguments.size(); i++) arguments[i] = move(callArguments[i - 2]);
							#else
								span callArgumentsSpan(arguments.begin() + 2, arguments.end());
								script->call(scriptIdx, callArgumentsSpan, returnValue);
							#endif
							miniScript->getContext()->pop();
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
			bool isVariadic() const override {
				return true;
			}
		};
		miniScript->registerMethod(new MethodContextScriptCall(miniScript));
	}
	{
		//
		class MethodContextScriptLoadScript: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodContextScriptLoadScript(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "scriptId", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_BOOLEAN, .name = "verbose", .optional = true, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "context.script.loadScript";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string scriptId;
				string pathName;
				string fileName;
				bool verbose = false;
				if ((arguments.size() == 3 || arguments.size() == 4) &&
					MiniScript::getStringValue(arguments, 0, scriptId) == true &&
					MiniScript::getStringValue(arguments, 1, pathName) == true &&
					MiniScript::getStringValue(arguments, 2, fileName) == true &&
					MiniScript::getBooleanValue(arguments, 3, verbose, true) == true) {
					unique_ptr<MiniScript> script;
					// try to load from (native) library
					if (miniScript->getLibrary() != nullptr) {
						script = unique_ptr<MiniScript>(
							miniScript->getLibrary()->loadScript(
								pathName,
								fileName
							)
						);
					}
					// no native script?
					if (script == nullptr) {
						// yarrrr, parse ordinary script
						script = make_unique<MiniScript>();
						script->setContext(miniScript->getContext());
						script->parseScript(
							pathName,
							fileName
						);
					}
					if (script != nullptr) {
						// verbose
						if (verbose == true) _Console::printLine(script->getInformation());
						//
						if (script->isValid() == false) {
							_Console::printLine("Script not valid.");
						} else {
							miniScript->getContext()->addScript(scriptId, script.release());
						}
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodContextScriptLoadScript(miniScript));
	}
	{
		//
		class MethodContextScriptRemoveScript: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodContextScriptRemoveScript(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "scriptId", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "context.script.removeScript";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string scriptId;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, scriptId) == true) {
					miniScript->getContext()->removeScript(scriptId);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodContextScriptRemoveScript(miniScript));
	}
	{
		//
		class MethodContextScriptGetScriptIds: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodContextScriptGetScriptIds(MiniScript* miniScript):
				MiniScript::Method({}, MiniScript::TYPE_ARRAY),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "context.script.getScriptIds";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() == 0) {
					auto scriptIds = miniScript->getContext()->getScriptIds();
					returnValue.setType(MiniScript::TYPE_ARRAY);
					for (const auto& scriptId: scriptIds) returnValue.pushArrayEntry(scriptId);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodContextScriptGetScriptIds(miniScript));
	}
}
