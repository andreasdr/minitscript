#include <memory>
#include <span>
#include <string>

#include <miniscript/miniscript/MiniScript.h>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/miniscript/HTTPDownloadClientClass.h>
#include <miniscript/network/httpclient/HTTPDownloadClient.h>

using std::make_shared;
using std::shared_ptr;
using std::span;
using std::string;

using miniscript::miniscript::HTTPDownloadClientClass;

using miniscript::miniscript::MiniScript;
using _Console = miniscript::utilities::Console;
using _HTTPDownloadClient = miniscript::network::httpclient::HTTPDownloadClient;

const string HTTPDownloadClientClass::TYPE_NAME = "HTTPDownloadClientClass";
vector<shared_ptr<_HTTPDownloadClient>> HTTPDownloadClientClass::instances;

void HTTPDownloadClientClass::initialize() {
}

void HTTPDownloadClientClass::registerConstants(MiniScript* miniScript) const {
}

void HTTPDownloadClientClass::registerMethods(MiniScript* miniScript) const {

}

void HTTPDownloadClientClass::unsetVariableValue(MiniScript::Variable& variable) const {
	delete static_cast<shared_ptr<_HTTPDownloadClient>*>(variable.getValuePtr());
}

void HTTPDownloadClientClass::setVariableValue(MiniScript::Variable& variable) const {
	auto sharedPtr = make_shared<_HTTPDownloadClient>();
	variable.setValuePtr(new shared_ptr<_HTTPDownloadClient>(sharedPtr));
}

void HTTPDownloadClientClass::setVariableValue(MiniScript::Variable& variable, const void* value) const {
	*static_cast<shared_ptr<_HTTPDownloadClient>*>(variable.getValuePtr()) = *static_cast<const shared_ptr<_HTTPDownloadClient>*>(value);
}

void HTTPDownloadClientClass::copyVariable(MiniScript::Variable& to, const MiniScript::Variable& from) const {
	to.setType(MiniScript::TYPE_HTTPDOWNLOADCLIENT);
	*static_cast<shared_ptr<_HTTPDownloadClient>*>(to.getValuePtr()) = *static_cast<shared_ptr<_HTTPDownloadClient>*>(from.getValuePtr());
}

bool HTTPDownloadClientClass::mul(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

bool HTTPDownloadClientClass::div(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

bool HTTPDownloadClientClass::add(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

bool HTTPDownloadClientClass::sub(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

const string& HTTPDownloadClientClass::getTypeAsString() const {
	return TYPE_NAME;
}

const string HTTPDownloadClientClass::getValueAsString(const MiniScript::Variable& variable) const {
	return
		"HTTPDownloadClientClass()";
}

