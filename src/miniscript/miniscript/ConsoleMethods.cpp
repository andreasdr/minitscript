#include <iostream>
#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/ConsoleMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::cin;
using std::getline;
using std::span;

using miniscript::miniscript::ConsoleMethods;

using miniscript::miniscript::MiniScript;

using _Console = miniscript::utilities::Console;

void ConsoleMethods::registerConstants(MiniScript* miniScript) {
}

void ConsoleMethods::registerMethods(MiniScript* miniScript) {
	// console
	{
		//
		class MethodConsoleLog: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsoleLog(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "console.log";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				for (const auto& argument: arguments) {
					_Console::print(argument.getValueAsString());
				}
				_Console::println();
			}
			bool isVariadic() const override {
				return true;
			}
		};
		miniScript->registerMethod(new MethodConsoleLog(miniScript));
	}
	{
		//
		class MethodConsoleDump: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsoleDump(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_PSEUDO_MIXED, .name = "value", .optional = false, .reference = false, .nullable = false }
					}
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "console.dump";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (arguments.size() != 1) {
					_Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					_Console::println(arguments[0].getValueAsString(true));
				}
			}
		};
		miniScript->registerMethod(new MethodConsoleDump(miniScript));
	}
	{
		//
		class MethodConsolePrint: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsolePrint(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "console.print";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				for (const auto& argument: arguments) {
					_Console::print(argument.getValueAsString());
				}
			}
			bool isVariadic() const override {
				return true;
			}
		};
		miniScript->registerMethod(new MethodConsolePrint(miniScript));
	}
	{
		//
		class MethodConsolePrintln: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsolePrintln(MiniScript* miniScript): MiniScript::Method(), miniScript(miniScript) {}
			const string getMethodName() override {
				return "console.println";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				for (const auto& argument: arguments) {
					_Console::print(argument.getValueAsString());
				}
				_Console::println();
			}
			bool isVariadic() const override {
				return true;
			}
		};
		miniScript->registerMethod(new MethodConsolePrintln(miniScript));
	}
	{
		//
		class MethodConsoleReadln: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsoleReadln(MiniScript* miniScript):
				MiniScript::Method({}, MiniScript::TYPE_STRING),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "console.readln";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string line;
				getline(cin, line);
				returnValue.setValue(line);
			}
		};
		miniScript->registerMethod(new MethodConsoleReadln(miniScript));
	}
	{
		//
		class MethodConsoleReadAll: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsoleReadAll(MiniScript* miniScript):
				MiniScript::Method({}, MiniScript::TYPE_STRING),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "console.readAll";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string line;
				string result;
				while (cin.eof() == false && getline(cin, line)) {
					result+= line + "\n";
				}
				returnValue.setValue(result);
			}
		};
		miniScript->registerMethod(new MethodConsoleReadAll(miniScript));
	}
	{
		//
		class MethodConsoleReadAllAsArray: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodConsoleReadAllAsArray(MiniScript* miniScript):
				MiniScript::Method({}, MiniScript::TYPE_ARRAY),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "console.readAllAsArray";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setType(MiniScript::TYPE_ARRAY);
				string line;
				while (cin.eof() == false && getline(cin, line)) {
					returnValue.pushArrayEntry(MiniScript::Variable(line));
				}
			}
		};
		miniScript->registerMethod(new MethodConsoleReadAllAsArray(miniScript));
	}
}
