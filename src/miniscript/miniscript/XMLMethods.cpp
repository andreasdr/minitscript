#include <span>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/miniscript/XMLMethods.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/StringTools.h>

using std::span;

using miniscript::miniscript::XMLMethods;

using miniscript::miniscript::MiniScript;
using miniscript::utilities::Console;
using miniscript::utilities::StringTools;

void XMLMethods::registerMethods(MiniScript* miniScript) {
	// xml
	{
		//
		class ScriptMethodXMLCreateTag: public MiniScript::Method {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodXMLCreateTag(MiniScript* miniScript):
				MiniScript::Method(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "name", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_MAP, .name = "attributes", .optional = true, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_STRING, .name = "innerXML", .optional = true, .reference = false, .nullable = false },
					},
					MiniScript::TYPE_STRING
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "xml.createTag";
			}
			void executeMethod(span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) override {
				//
				string name;
				string innerXML;
				if (MiniScript::getStringValue(arguments, 0, name, false) == false ||
					(arguments.size() >= 2 && arguments[1].getType() != MiniScript::TYPE_MAP) ||
					MiniScript::getStringValue(arguments, 2, innerXML, true) == false) {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				} else {
					auto mapPtr = arguments[1].getMapPointer();
					string xml;
					xml+= "<" + name;
					if (mapPtr != nullptr && mapPtr->empty() == false) {
						for(const auto& [mapEntryName, mapEntryValue]: *mapPtr) {
							xml+= " " + mapEntryName + "=\"" + escape(mapEntryValue->getValueAsString()) + "\"";
						}
					}
					if (innerXML.empty() == true) {
						xml+= "/>";
					} else {
						xml+= ">" + innerXML + "</" + name + ">";
					}
					//
					returnValue.setValue(xml);
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodXMLCreateTag(miniScript));
	}
}

inline const string XMLMethods::escape(const string& str) {
	string result;
	result = StringTools::replace(str, "&", "&amp;");
	result = StringTools::replace(result, "\"", "&quot;");
	result = StringTools::replace(result, "'", "&#39;");
	result = StringTools::replace(result, "<", "&lt;");
	result = StringTools::replace(result, ">", "&gt;");
	return result;
}
