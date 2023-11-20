#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/FileSystemMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Exception.h>
#include <miniscript/utilities/FileSystem.h>

using std::span;

using miniscript::miniscript::FileSystemMethods;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;
using miniscript::utilities::Exception;
using miniscript::utilities::FileSystem;

void FileSystemMethods::registerMethods(MiniScript* miniScript) {
	// file system methods
	{
		//
		class ScriptFileSystemGetFileName: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemGetFileName(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.getFileName";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true) {
					returnValue.setValue(FileSystem::getFileName(pathName, fileName));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemGetFileName(miniScript));
	}
	{
		//
		class ScriptFileSystemGetFileSize: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemGetFileSize(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.getFileSize";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true) {
					try {
						returnValue.setValue(static_cast<int64_t>(FileSystem::getFileSize(pathName, fileName)));
					} catch (Exception& exception) {
						Console::println("An error occurred: " + string(exception.what()));
					}
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemGetFileSize(miniScript));
	}
	{
		//
		class ScriptFileSystemGetContentAsString: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemGetContentAsString(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.getContentAsString";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true) {
					try {
						returnValue.setValue(FileSystem::getContentAsString(pathName, fileName));
					} catch (Exception& exception) {
						Console::println("An error occurred: " + string(exception.what()));
					}
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemGetContentAsString(miniScript));
	}
	{
		//
		class ScriptFileSystemSetContentFromString: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemSetContentFromString(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "content", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.setContentFromString";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				string content;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true &&
					MiniScript::getStringValue(argumentValues, 2, content, false) == true) {
					try {
						FileSystem::setContentFromString(pathName, fileName, content);
						returnValue.setValue(true);
					} catch (Exception& exception) {
						returnValue.setValue(false);
						Console::println("An error occurred: " + string(exception.what()));
					}
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemSetContentFromString(miniScript));
	}
}
