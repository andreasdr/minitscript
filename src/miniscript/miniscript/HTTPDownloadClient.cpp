#include <span>
#include <string>

#include <miniscript/miniscript/MiniScript.h>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/miniscript/HTTPDownloadClient.h>

using std::span;
using std::string;

using miniscript::miniscript::HTTPDownloadClient;

using miniscript::miniscript::MiniScript;
using _Console = miniscript::utilities::Console;

const string HTTPDownloadClient::TYPE_NAME = "HTTPDownloadClient";

void HTTPDownloadClient::initialize() {
}

void HTTPDownloadClient::registerConstants(MiniScript* miniScript) const {
}

void HTTPDownloadClient::registerMethods(MiniScript* miniScript) const {
}

void HTTPDownloadClient::unsetVariableValue(MiniScript::Variable& variable) const {
	// delete static_cast<xyz*>(variable.getValuePtr());
}

void HTTPDownloadClient::setVariableValue(MiniScript::Variable& variable) const {
	// variable.setValuePtr(new xyz());
}

void HTTPDownloadClient::setVariableValue(MiniScript::Variable& variable, const void* value) const {
	// *static_cast<xyz*>(variable.getValuePtr()) = *static_cast<const xyz*>(value);
}

void HTTPDownloadClient::copyVariable(MiniScript::Variable& to, const MiniScript::Variable& from) const {
	// to.setType(TYPE_HTTPDOWNLOADCLIENT);
	// *static_cast<*xyz>(to.getValuePtr()) = *static_cast<xyz*>(from.getValuePtr());
}

bool HTTPDownloadClient::mul(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

bool HTTPDownloadClient::div(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

bool HTTPDownloadClient::add(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

bool HTTPDownloadClient::sub(MiniScript* miniScript, const span<MiniScript::Variable>& arguments, MiniScript::Variable& returnValue, const MiniScript::Statement& statement) const {
	return false;
}

const string& HTTPDownloadClient::getTypeAsString() const {
	return TYPE_NAME;
}

const string HTTPDownloadClient::getValueAsString(const MiniScript::Variable& variable) const {
	return
		"HTTPDownloadClient()";
}

