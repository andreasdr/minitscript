#include <memory>
#include <span>
#include <string>

#include <miniscript/miniscript/MiniScript.h>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/miniscript/HTTPDownloadClient.h>
#include <miniscript/network/httpclient/HTTPDownloadClient.h>

using std::make_shared;
using std::shared_ptr;
using std::span;
using std::string;

using miniscript::miniscript::HTTPDownloadClient;

using miniscript::miniscript::MiniScript;
using _Console = miniscript::utilities::Console;
using _HTTPDownloadClient = miniscript::network::httpclient::HTTPDownloadClient;

const string HTTPDownloadClient::TYPE_NAME = "HTTPDownloadClient";
vector<shared_ptr<_HTTPDownloadClient>> HTTPDownloadClient::instances;

void HTTPDownloadClient::initialize() {
}

void HTTPDownloadClient::registerConstants(MiniScript* miniScript) const {
}

void HTTPDownloadClient::registerMethods(MiniScript* miniScript) const {

}

void HTTPDownloadClient::unsetVariableValue(MiniScript::Variable& variable) const {
	delete static_cast<shared_ptr<_HTTPDownloadClient>*>(variable.getValuePtr());
}

void HTTPDownloadClient::setVariableValue(MiniScript::Variable& variable) const {
	auto sharedPtr = make_shared<_HTTPDownloadClient>();
	variable.setValuePtr(new shared_ptr<_HTTPDownloadClient>(sharedPtr));
}

void HTTPDownloadClient::setVariableValue(MiniScript::Variable& variable, const void* value) const {
	*static_cast<shared_ptr<_HTTPDownloadClient>*>(variable.getValuePtr()) = *static_cast<const shared_ptr<_HTTPDownloadClient>*>(value);
}

void HTTPDownloadClient::copyVariable(MiniScript::Variable& to, const MiniScript::Variable& from) const {
	to.setType(MiniScript::TYPE_HTTPDOWNLOADCLIENT);
	*static_cast<shared_ptr<_HTTPDownloadClient>*>(to.getValuePtr()) = *static_cast<shared_ptr<_HTTPDownloadClient>*>(from.getValuePtr());
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

