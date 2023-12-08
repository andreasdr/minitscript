#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/ByteArrayMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>

using std::span;

using miniscript::miniscript::ByteArrayMethods;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;

void ByteArrayMethods::registerMethods(MiniScript* miniScript) {
	// array methods
	{
		//
		class ScriptMethodByteArray: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArray(MiniScript* miniScript):
				MiniScript::Method(
					{},
					MiniScript::TYPE_BYTEARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray";
			}
			bool isVariadic() const override {
				return true;
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				returnValue.setType(MiniScript::TYPE_BYTEARRAY);
				uint8_t value;
				for (const auto& argumentValue: argumentValues) {
					if (argumentValue.getByteValue(miniScript, value, &statement) == true) {
						returnValue.pushByteArrayEntry(value);
					} else {
						Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
						miniScript->startErrorScript();
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArray(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayLength: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayLength(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.length";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (argumentValues.size() != 1 || argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					returnValue.setValue(static_cast<int64_t>(argumentValues[0].getByteArraySize()));
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayLength(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayPush: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayPush(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = true, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.push";
			}
			bool isVariadic() const override {
				return true;
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				if (argumentValues.size() < 1 || argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					uint8_t value;
					for (auto i = 1; i < argumentValues.size(); i++) {
						if (argumentValues[i].getByteValue(miniScript, value, &statement) == true) {
							argumentValues[0].pushByteArrayEntry(value);
						}
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayPush(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayGet: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayGet(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "index", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.get";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t index;
				if ((argumentValues.size() <= 1 || argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY) ||
					MiniScript::getIntegerValue(argumentValues, 1, index, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					returnValue.setValue(static_cast<int64_t>(argumentValues[0].getByteArrayEntry(index)));
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayGet(miniScript));
	}
	{
		//
		class ScriptMethodByteArraySet: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArraySet(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "index", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "value", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.set";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t index;
				if ((argumentValues.size() <= 2 || argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY) ||
					MiniScript::getIntegerValue(argumentValues, 1, index, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					uint8_t value;
					if (argumentValues[2].getByteValue(miniScript, value, &statement) == true) {
						argumentValues[0].setByteArrayEntry(index, value);
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArraySet(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayRemove: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayRemove(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "index", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.remove";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t index;
				if ((argumentValues.size() < 2 || argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY) ||
					MiniScript::getIntegerValue(argumentValues, 1, index, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					argumentValues[0].removeByteArrayEntry(index);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayRemove(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayAppend: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayAppend(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "other", .optional = false, .reference = true, .nullable = false },
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.appendByteArray";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				if (argumentValues.size() != 2 ||
					argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY ||
					argumentValues[1].getType() != MiniScript::TYPE_BYTEARRAY) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto byteArrayPtr = argumentValues[0].getByteArrayPointer();
					auto otherByteArrayPtr = argumentValues[1].getByteArrayPointer();
					if (byteArrayPtr != nullptr && otherByteArrayPtr != nullptr) {
						for (const auto value: *otherByteArrayPtr) byteArrayPtr->push_back(value);
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayAppend(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayExtract: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayExtract(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = true, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "index", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "length", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BYTEARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.extractByteArray";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t index;
				int64_t length;
				if (argumentValues.size() != 3 ||
					argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY ||
					MiniScript::getIntegerValue(argumentValues, 1, index, false) == false ||
					MiniScript::getIntegerValue(argumentValues, 2, length, false) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto byteArrayPtr = argumentValues[0].getByteArrayPointer();
					if (byteArrayPtr != nullptr) {
						returnValue.setType(MiniScript::TYPE_BYTEARRAY);
						for (auto i = index; i >= 0 && i < byteArrayPtr->size() && i < index + length; i++) returnValue.pushByteArrayEntry((*byteArrayPtr)[i]);
					}
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayExtract(miniScript));
	}
	{
		//
		class ScriptMethodByteArrayClear: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodByteArrayClear(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "bytearray", .optional = false, .reference = true, .nullable = false }
					},
					MiniScript::TYPE_NULL
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "bytearray.clear";
			}
			void executeMethod(span<MiniScript::Variable>& argumentValues, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t index;
				if (argumentValues.size() != 1 || argumentValues[0].getType() != MiniScript::TYPE_BYTEARRAY) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					argumentValues[0].clearByteArray();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodByteArrayClear(miniScript));
	}
}
