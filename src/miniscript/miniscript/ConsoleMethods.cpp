#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/ConsoleMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::span;

using miniscript::miniscript::ConsoleMethods;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;

void ConsoleMethods::registerMethods(MiniScript* miniScript) {
	// console
	{
		//
		class ScriptMethodConsoleLog: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodConsoleLog(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "console.log";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				for (const auto& argumentValue: argumentValues) {
					Console::print(argumentValue.getValueAsString());
				}
				Console::println();
			}
			bool isVariadic() const override {
				return true;
			}
		};
		miniScript->registerMethod(new ScriptMethodConsoleLog(miniScript));
	}
	{
		//
		class ScriptMethodConsoleDump: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodConsoleDump(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "console.dump";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (argumentValues.size() != 1) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					Console::println(argumentValues[0].getValueAsString(true));
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodConsoleDump(miniScript));
	}
}
