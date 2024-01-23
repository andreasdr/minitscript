#include <regex>
#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/math/Math.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/miniscript/StringMethods.h>
#include <miniscript/utilities/Character.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Float.h>
#include <miniscript/utilities/Integer.h>
#include <miniscript/utilities/UTF8StringTokenizer.h>
#include <miniscript/utilities/UTF8StringTools.h>
#include <miniscript/utilities/UTF8CharacterIterator.h>

using std::smatch;
using std::span;

using miniscript::miniscript::StringMethods;

using miniscript::miniscript::MiniScript;

using _Math = miniscript::math::Math;
using _Character = miniscript::utilities::Character;
using _Console = miniscript::utilities::Console;
using _Float = miniscript::utilities::Float;
using _Integer = miniscript::utilities::Integer;
using _UTF8StringTools = miniscript::utilities::UTF8StringTools;
using _UTF8CharacterIterator = miniscript::utilities::UTF8CharacterIterator;

void StringMethods::registerConstants(MiniScript* miniScript) {
}

void StringMethods::registerMethods(MiniScript* miniScript) {
	// string functions
	{
		//
		class MethodString: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodString(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(stringValue);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodString(miniScript));
	}
	{
		//
		class MethodStringGetLength: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringGetLength(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::getLength";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(static_cast<int64_t>(_UTF8StringTools::getLength(stringValue, arguments[0].getStringValueCache())));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringGetLength(miniScript));
	}
	{
		//
		class MethodStringGetSize: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringGetSize(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::getSize";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(static_cast<int64_t>(stringValue.size()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringGetSize(miniScript));
	}
	{
		//
		class MethodStringCharAt: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringCharAt(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "index", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::charAt";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				int64_t index;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getIntegerValue(arguments, 1, index) == true) {
					returnValue.setValue(_UTF8StringTools::getCharAt(stringValue, index, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringCharAt(miniScript));
	}
	{
		//
		class MethodStringStartsWith: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringStartsWith(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "prefix", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::startsWith";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string prefix;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, prefix) == true) {
					returnValue.setValue(_UTF8StringTools::startsWith(stringValue, prefix));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringStartsWith(miniScript));
	}
	{
		//
		class MethodStringEndsWith: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringEndsWith(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "suffix", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::endsWith";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string suffix;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, suffix) == true) {
					returnValue.setValue(_UTF8StringTools::endsWith(stringValue, suffix));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringEndsWith(miniScript));
	}
	{
		//
		class MethodStringReplace: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringReplace(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "by", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "beginIndex", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::replace";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string what;
				string by;
				int64_t beginIndex = 0;
				if ((arguments.size() == 3 || arguments.size() == 4) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, what) == true &&
					MiniScript::getStringValue(arguments, 2, by) == true &&
					MiniScript::getIntegerValue(arguments, 3, beginIndex, true) == true) {
					returnValue.setValue(_UTF8StringTools::replace(stringValue, what, by, beginIndex, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringReplace(miniScript));
	}
	{
		//
		class MethodStringIndexOf: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringIndexOf(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "beginIndex", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::indexOf";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string what;
				int64_t beginIndex = 0;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, what) == true &&
					MiniScript::getIntegerValue(arguments, 2, beginIndex, true) == true) {
					returnValue.setValue(_UTF8StringTools::indexOf(stringValue, what, beginIndex, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringIndexOf(miniScript));
	}
	{
		//
		class MethodStringFirstIndexOf: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringFirstIndexOf(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "beginIndex", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::firstIndexOf";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string what;
				int64_t beginIndex = 0;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, what) == true &&
					MiniScript::getIntegerValue(arguments, 2, beginIndex, true) == true) {
					returnValue.setValue(_UTF8StringTools::firstIndexOf(stringValue, what, beginIndex, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringFirstIndexOf(miniScript));
	}
	{
		//
		class MethodStringLastIndexOf: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringLastIndexOf(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "beginIndex", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::lastIndexOf";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string what;
				int64_t beginIndex = 0;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, what) == true &&
					MiniScript::getIntegerValue(arguments, 2, beginIndex, true) == true) {
					returnValue.setValue(_UTF8StringTools::lastIndexOf(stringValue, what, beginIndex, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringLastIndexOf(miniScript));
	}
	{
		//
		class MethodStringFirstIndexOfChars: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringFirstIndexOfChars(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "beginIndex", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::firstIndexOfChars";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string what;
				int64_t beginIndex = 0;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, what) == true &&
					MiniScript::getIntegerValue(arguments, 2, beginIndex, true) == true) {
					returnValue.setValue(
						static_cast<int64_t>(
							_UTF8StringTools::firstIndexOfChars(
								stringValue,
								what,
								beginIndex,
								arguments[0].getStringValueCache(),
								arguments[1].getStringValueCache()
							)
						)
					);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringFirstIndexOfChars(miniScript));
	}
	{
		//
		class MethodStringLastIndexOfChars: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringLastIndexOfChars(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "endIndex", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_INTEGER
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::lastIndexOfChars";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string what;
				int64_t endIndex = string::npos;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, what) == true &&
					MiniScript::getIntegerValue(arguments, 2, endIndex, true) == true) {
					returnValue.setValue(
						static_cast<int64_t>(
							_UTF8StringTools::lastIndexOfChars(
								stringValue,
								what,
								endIndex,
								arguments[0].getStringValueCache(),
								arguments[1].getStringValueCache()
							)
						)
					);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringLastIndexOfChars(miniScript));
	}
	{
		//
		class MethodStringSubString: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringSubString(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "beginIndex", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "endIndex", .optional = true, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::substring";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				int64_t beginIndex;
				int64_t endIndex = string::npos;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getIntegerValue(arguments, 1, beginIndex) == true &&
					MiniScript::getIntegerValue(arguments, 2, endIndex, true) == true) {
					// utf8 character iterator
					returnValue.setValue(_UTF8StringTools::substring(stringValue, beginIndex, endIndex));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringSubString(miniScript));
	}
	{
		//
		class MethodStringEqualsIgnoreCase: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringEqualsIgnoreCase(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "other", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::equalsIgnoreCase";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string other;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, other) == true) {
					returnValue.setValue(_UTF8StringTools::equalsIgnoreCase(stringValue, other, arguments[0].getStringValueCache(), arguments[1].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringEqualsIgnoreCase(miniScript));
	}
	{
		//
		class MethodStringTrim: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringTrim(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::trim";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(_UTF8StringTools::trim(stringValue, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringTrim(miniScript));
	}
	{
		//
		class MethodStringRegexMatch: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringRegexMatch(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "pattern", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_ARRAY, .name = "matches", .optional = true, .reference = true, .nullable = true },
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::regexMatch";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string pattern;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, pattern) == true) {
					if (arguments.size() == 3) {
						smatch matches;
						returnValue.setValue(_UTF8StringTools::regexMatch(stringValue, pattern, &matches));
						arguments[2].setType(MiniScript::TYPE_ARRAY);
						arguments[2].clearArray();
						for (const auto& match: matches) {
							arguments[2].pushArrayEntry(MiniScript::Variable(string(match.str())));
						}
					} else {
						returnValue.setValue(_UTF8StringTools::regexMatch(stringValue, pattern));
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringRegexMatch(miniScript));
	}
	{
		//
		class MethodStringRegexSearch: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringRegexSearch(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "pattern", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_ARRAY, .name = "matches", .optional = true, .reference = true, .nullable = true },
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::regexSearch";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string pattern;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, pattern) == true) {
					if (arguments.size() == 3) {
						smatch matches;
						returnValue.setValue(_UTF8StringTools::regexSearch(stringValue, pattern, &matches));
						arguments[2].setType(MiniScript::TYPE_ARRAY);
						arguments[2].clearArray();
						for (const auto& match: matches) {
							arguments[2].pushArrayEntry(MiniScript::Variable(string(match.str())));
						}
					} else {
						returnValue.setValue(_UTF8StringTools::regexSearch(stringValue, pattern));
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringRegexSearch(miniScript));
	}
	{
		//
		class MethodStringRegexReplace: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringRegexReplace(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "pattern", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "by", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::regexReplace";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string pattern;
				string by;
				if (arguments.size() == 3 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, pattern) == true &&
					MiniScript::getStringValue(arguments, 2, by) == true) {
					returnValue.setValue(_UTF8StringTools::regexReplace(stringValue, pattern, by));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringRegexReplace(miniScript));
	}
	{
		//
		class MethodStringTokenize: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringTokenize(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "delimiters", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_ARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::tokenize";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string delimiters;
				if (arguments.size() == 2 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, delimiters) == true) {
					auto tokenizedStringVector = _UTF8StringTools::tokenize(stringValue, delimiters);
					returnValue.setType(MiniScript::TYPE_ARRAY);
					for (const auto& tokenizedString: tokenizedStringVector) {
						returnValue.pushArrayEntry(tokenizedString);
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringTokenize(miniScript));
	}
	{
		//
		class MethodStringGenerate: public MiniScript::Method {
		private:
			MiniScript *miniScript { nullptr };
		public:
			MethodStringGenerate(MiniScript *miniScript) :
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "what", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "count", .optional = true, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "String::generate";
			}
			void executeMethod(span<MiniScript::Variable> &arguments, MiniScript::Variable &returnValue, const MiniScript::Statement &statement) override {
				string what;
				int64_t count = 1;
				if ((arguments.size() == 1 || arguments.size() == 2) &&
					MiniScript::getStringValue(arguments, 0, what) == true &&
					MiniScript::getIntegerValue(arguments, 1, count, true) == true) {
					returnValue.setValue(_UTF8StringTools::generate(what, count));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringGenerate(miniScript));
	}
	{
		//
		class MethodStringIndent: public MiniScript::Method {
		private:
			MiniScript *miniScript { nullptr };
		public:
			MethodStringIndent(MiniScript *miniScript) :
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "with", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "count", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {
				//
			}
			const string getMethodName() override {
				return "String::indent";
			}
			void executeMethod(span<MiniScript::Variable> &arguments, MiniScript::Variable &returnValue, const MiniScript::Statement &statement) override {
				string src;
				string with;
				int64_t count = 1;
				if ((arguments.size() == 2 || arguments.size() == 3) &&
					MiniScript::getStringValue(arguments, 0, src) == true &&
					MiniScript::getStringValue(arguments, 1, with) == true &&
					MiniScript::getIntegerValue(arguments, 2, count, true) == true) {
					returnValue.setValue(_UTF8StringTools::indent(src, with, count));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringIndent(miniScript));
	}
	{
		//
		class MethodStringConcatenate: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringConcatenate(MiniScript* miniScript): MiniScript::Method({}, MiniScript::TYPE_STRING), miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::concatenate";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string result;
				for (const auto& argument: arguments) {
					result+= argument.getValueAsString();
				}
				returnValue.setValue(result);
			}
			bool isVariadic() const override {
				return true;
			}
		};
		miniScript->registerMethod(new MethodStringConcatenate(miniScript));
	}
	{
		//
		class MethodToStringUpperCase: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodToStringUpperCase(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::toUpperCase";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(_UTF8StringTools::toUpperCase(stringValue, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodToStringUpperCase(miniScript));
	}
	{
		//
		class MethodToStringLowerCase: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodToStringLowerCase(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::toLowerCase";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(_UTF8StringTools::toLowerCase(stringValue, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodToStringLowerCase(miniScript));
	}
	{
		//
		class MethodStringIsEmpty: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringIsEmpty(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::isEmpty";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(stringValue.empty());
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringIsEmpty(miniScript));
	}
	{
		//
		class MethodStringIsFloat: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringIsFloat(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::isFloat";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(_Float::is(stringValue));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringIsFloat(miniScript));
	}
	{
		//
		class MethodStringIsInteger: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringIsInteger(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_BOOLEAN
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::isInteger";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setValue(_Integer::is(stringValue));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringIsInteger(miniScript));
	}
	{
		//
		class MethodStringPadLeft: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringPadLeft(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "by", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "toLength", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::padLeft";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string by;
				int64_t toLength;
				if (arguments.size() == 3 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, by) == true &&
					MiniScript::getIntegerValue(arguments, 2, toLength) == true) {
					returnValue.setValue(_UTF8StringTools::padLeft(stringValue, by, toLength, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringPadLeft(miniScript));
	}
	{
		//
		class MethodStringPadRight: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringPadRight(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "by", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_INTEGER, .name = "toLength", .optional = false, .reference = false, .nullable = false }
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::padRight";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				string by;
				int64_t toLength;
				if (arguments.size() == 3 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true &&
					MiniScript::getStringValue(arguments, 1, by) == true &&
					MiniScript::getIntegerValue(arguments, 2, toLength) == true) {
					returnValue.setValue(_UTF8StringTools::padRight(stringValue, by, toLength, arguments[0].getStringValueCache()));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringPadRight(miniScript));
	}
	{
		//
		class MethodStringToByteArray: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringToByteArray(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "string", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_BYTEARRAY
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::toByteArray";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					MiniScript::getStringValue(arguments, 0, stringValue) == true) {
					returnValue.setType(MiniScript::TYPE_BYTEARRAY);
					for (auto i = 0; i < stringValue.size(); i++) returnValue.pushByteArrayEntry(stringValue[i]);
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringToByteArray(miniScript));
	}
	{
		//
		class MethodStringFromByteArray: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringFromByteArray(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_BYTEARRAY, .name = "byteArray", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::fromByteArray";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				string stringValue;
				if (arguments.size() == 1 &&
					arguments[0].getType() == MiniScript::TYPE_BYTEARRAY) {
					auto byteArrayPointer = arguments[0].getByteArrayPointer();
					if (byteArrayPointer != nullptr) {
						returnValue.setValue(string((const char*)(byteArrayPointer->data()), byteArrayPointer->size()));
					}
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringFromByteArray(miniScript));
	}
	{
		//
		class MethodStringFromCodePoint: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			MethodStringFromCodePoint(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_INTEGER, .name = "codePoint", .optional = false, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "String::fromCodePoint";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				int64_t codePoint;
				if (arguments.size() == 1 &&
					MiniScript::getIntegerValue(arguments, 0, codePoint) == true) {
					returnValue.setValue(_Character::toString(codePoint));
				} else {
					MINISCRIPT_METHODUSAGE_COMPLAIN(getMethodName());
				}
			}
		};
		miniScript->registerMethod(new MethodStringFromCodePoint(miniScript));
	}
}
