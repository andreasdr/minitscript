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
					MiniScript::TYPE_INTEGER,
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
	{
		//
		class ScriptFileSystemGetContentAsStringArray: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemGetContentAsStringArray(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_ARRAY,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.getContentAsStringArray";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true) {
					vector<string> contentAsStringArray;
					try {
						FileSystem::getContentAsStringArray(pathName, fileName, contentAsStringArray);
						//
						returnValue.setType(MiniScript::TYPE_ARRAY);
						for (const auto& contentAsStringArrayEntry: contentAsStringArray) {
							returnValue.pushArrayEntry(contentAsStringArrayEntry);
						}
					} catch (Exception& exception) {
						Console::println("An error occurred: " + string(exception.what()));
					}
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemGetContentAsStringArray(miniScript));
	}
	{
		//
		class ScriptFileSystemSetContentFromStringArray: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemSetContentFromStringArray(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_ARRAY, .name = "content", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.setContentFromStringArray";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				if (argumentValues.size() == 3 &&
					MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true &&
					argumentValues[2].getType() == MiniScript::TYPE_ARRAY) {
					vector<string> contentAsStringArray;
					auto arrayPointer = argumentValues[2].getArrayPointer();
					if (arrayPointer != nullptr) for (const auto arrayEntry: *arrayPointer) contentAsStringArray.push_back(arrayEntry->getValueAsString());
					try {
						FileSystem::setContentFromStringArray(pathName, fileName, contentAsStringArray);
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
		miniScript->registerMethod(new ScriptFileSystemSetContentFromStringArray(miniScript));
	}
	{
		//
		class ScriptFileSystemIsPath: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemIsPath(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BOOLEAN,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.isPath";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true) {
					returnValue.setValue(FileSystem::isPath(pathName));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemIsPath(miniScript));
	}
	{
		//
		class ScriptFileSystemIsDrive: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemIsDrive(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BOOLEAN,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.isDrive";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true) {
					returnValue.setValue(FileSystem::isDrive(pathName));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemIsDrive(miniScript));
	}
	{
		// TODO:
		//	fileExists -> doesFileExist + doesFolderExist?
		//	fileExists -> exists(pathName, fileName) + exists(pathName)?
		class ScriptFileSystemFileExists: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemFileExists(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "fileName", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BOOLEAN,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.fileExists";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string fileName;
				if (MiniScript::getStringValue(argumentValues, 0, fileName, false) == true) {
					returnValue.setValue(FileSystem::fileExists(fileName));
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemFileExists(miniScript));
	}
	{
		//
		class ScriptFileSystemGetCanonicalPath: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemGetCanonicalPath(MiniScript* miniScript):
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
				return "filesystem.getCanonicalPath";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				string fileName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true &&
					MiniScript::getStringValue(argumentValues, 1, fileName, false) == true) {
					try {
						returnValue.setValue(FileSystem::getCanonicalPath(pathName, fileName));
					} catch (Exception& exception) {
						Console::println("An error occurred: " + string(exception.what()));
					}
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemGetCanonicalPath(miniScript));
	}
	{
		//
		class ScriptFileSystemGetCurrentWorkingPathName: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemGetCurrentWorkingPathName(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{},
					MiniScript::TYPE_STRING,
					true
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.getCurrentWorkingPathName";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				returnValue.setValue(FileSystem::getCurrentWorkingPathName());
			}
		};
		miniScript->registerMethod(new ScriptFileSystemGetCurrentWorkingPathName(miniScript));
	}
	{
		//
		class ScriptFileSystemChangePath: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptFileSystemChangePath(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "pathName", .optional = false, .reference = false, .nullable = false },
					}
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "filesystem.changePath";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string pathName;
				if (MiniScript::getStringValue(argumentValues, 0, pathName, false) == true) {
					FileSystem::changePath(pathName);
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptFileSystemChangePath(miniScript));
	}
}
