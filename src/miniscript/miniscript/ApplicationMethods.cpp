#include <array>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <memory>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/ApplicationMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/os/filesystem/FileSystem.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Exception.h>
#include <miniscript/utilities/StringTools.h>

using std::array;
using std::span;
using std::shared_ptr;

using miniscript::miniscript::ApplicationMethods;

using miniscript::miniscript::MiniScript;
using miniscript::os::filesystem::FileSystem;
using miniscript::utilities::Exception;

using _Console = miniscript::utilities::Console;
using _StringTools = miniscript::utilities::StringTools;

void ApplicationMethods::registerConstants(MiniScript* miniScript) {
	miniScript->setConstant("$Application::EXITCODE_SUCCESS", MiniScript::Variable(static_cast<int64_t>(EXIT_SUCCESS)));
	miniScript->setConstant("$Application::EXITCODE_FAILURE", MiniScript::Variable(static_cast<int64_t>(EXIT_FAILURE)));
	//
	#if defined(__FreeBSD__)
		miniScript->setConstant("$Application::OS", string("FreeBSD"));
	#elif defined(__HAIKU__)
		miniScript->setConstant("$Application::OS", string("Haiku"));
	#elif defined(__linux__)
		miniScript->setConstant("$Application::OS", string("Linux"));
	#elif defined(__APPLE__)
		miniScript->setConstant("$Application::OS", string("MacOSX"));
	#elif defined(__NetBSD__)
		miniScript->setConstant("$Application::OS", string("NetBSD"));
	#elif defined(__OpenBSD__)
		miniScript->setConstant("$Application::OS", string("OpenBSD"));
	#elif defined(_MSC_VER)
		miniScript->setConstant("$Application::OS", string("Windows-MSC"));
	#elif defined(_WIN32)
		miniScript->setConstant("$Application::OS", string("Windows-MINGW"));
	#else
		miniScript->setConstant("$Application::OS", string("Unknown"));
	#endif
	#if defined(__i386__) || defined(_M_IX86)
		miniScript->setConstant("$Application::CPU", string("X86"));
	#elif defined(__amd64__) || defined(_M_X64)
		miniScript->setConstant("$Application::CPU", string("X64"));
	#elif defined(__ia64__) || defined(_M_IA64)
		miniScript->setConstant("$Application::CPU", string("IA64"));
	#elif defined(__aarch64__) || defined(_M_ARM64)
		miniScript->setConstant("$Application::CPU", string("ARM64"));
	#elif defined(__arm__) || defined(_M_ARM)
		miniScript->setConstant("$Application::CPU", string("ARM"));
	#elif defined(__riscv)
		#if __riscv_xlen == 64
			miniScript->setConstant("$Application::CPU", string("RISCV64"));
		#else
			miniScript->setConstant("$Application::CPU", string("RISCV"));
		#endif
	#elif defined(__powerpc64__)
		miniScript->setConstant("$Application::CPU", string("PPC64"));
	#elif defined(__powerpc__)
		miniScript->setConstant("$Application::CPU", string("PPC"));
	#else
		miniScript->setConstant("$Application::CPU", string("Unknown"));
	#endif
}

const string ApplicationMethods::execute(const string& command, int* exitCode, string* error) {
	FILE* pipe = nullptr;
	auto _exitCode = EXIT_FAILURE;
	// see: https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
	array<char, 128> buffer;
	string result;
	string errorFile;
	auto _command = command;
	if (error != nullptr) {
		errorFile = tmpnam(nullptr);
		_command+= " 2>" + errorFile;
	}
	//
	try {
		pipe = popen(_command.c_str(), "r");
		if (pipe == nullptr) throw std::runtime_error("popen() failed!");
		while (feof(pipe) == false) {
			if (fgets(buffer.data(), buffer.size(), pipe) != nullptr) result += buffer.data();
		}
	} catch (Exception& exception) {
		_Console::printLine("ApplicationMethods::execute(): An error occurred: " + string(exception.what()));
	}
	//
	#if defined(_MSC_VER)
		_exitCode = _pclose(pipe);
	#else
		_exitCode = pclose(pipe);
	#endif
	//
	if (exitCode != nullptr) *exitCode = _exitCode;
	//
	if (error != nullptr) {
		try {
			*error = FileSystem::getContentAsString(
				FileSystem::getPathName(errorFile),
				FileSystem::getFileName(errorFile)
			);
		} catch (Exception& exception) {
			_Console::printLine("ApplicationMethods::execute(): An error occurred: " + string(exception.what()));
		}
	}
	//
	return result;
}

void ApplicationMethods::registerMethods(MiniScript* miniScript) {
	// application
	{
		//
		class MethodApplicationExecute: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodApplicationExecute(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "command", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "application.execute";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string command;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, command) == true) {
					returnValue.setValue(ApplicationMethods::execute(command));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodApplicationExecute(miniScript));
	}
	//
	if (miniScript->getContext() != nullptr) {
		//
		{
			//
			class MethodApplicationGetArguments: public MiniScript::Method {
			private:
				MiniScript* miniScript { nullptr };
			public:
				MethodApplicationGetArguments(MiniScript* miniScript):
					MiniScript::Method({}, MiniScript::TYPE_ARRAY),
					miniScript(miniScript) {}
				const string getMethodName() override {
					return "application.getArguments";
				}
				void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
					if (arguments.size() == 0) {
						returnValue.setType(MiniScript::TYPE_ARRAY);
						for (const auto& argumentValue: miniScript->getContext()->getArgumentValues()) {
							returnValue.pushArrayEntry(MiniScript::Variable(argumentValue));
						}
					} else {
						MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
					}
				}
			};
			miniScript->registerMethod(new MethodApplicationGetArguments(miniScript));
		}
		{
			//
			class MethodApplicationExit: public MiniScript::Method {
			private:
				MiniScript* miniScript { nullptr };
			public:
				MethodApplicationExit(MiniScript* miniScript):
					MiniScript::Method(
						{
							{ .type = MiniScript::TYPE_INTEGER, .name = "exitCode", .optional = true, .reference = false, .nullable = false },
						}
					),
					miniScript(miniScript) {}
				const string getMethodName() override {
					return "application.exit";
				}
				void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
					int64_t exitCode = 0ll;
					if ((arguments.size() == 0 || arguments.size() == 1) &&
						MiniScript::getIntegerValue(arguments, 0, exitCode, true) == true) {
						miniScript->getContext()->setExitCode(static_cast<int>(exitCode));
						miniScript->stopScriptExecution();
						miniScript->stopRunning();
					} else {
						MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
					}
				}
			};
			miniScript->registerMethod(new MethodApplicationExit(miniScript));
		}
	}
}
