#include <span>

#include <unordered_map>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/NetworkMethods.h>
#include <miniscript/miniscript/MiniScript.h>
#include <miniscript/network/httpclient/HTTPClient.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/Exception.h>

using std::span;
using std::unordered_map;

using miniscript::miniscript::NetworkMethods;

using miniscript::miniscript::MiniScript;
using miniscript::network::httpclient::HTTPClient;
using miniscript::utilities::Console;
using miniscript::utilities::Exception;

/**
 * Methode
 * 	network.httpclient.executeGet
 * 	IN
 * 		URL
 *  	?$getParameters
 *  	?$headers
 *
 *  OUT
 *  	body
 *  	?statusCode
 * 		?responseheaders
 *
 */

void NetworkMethods::registerMethods(MiniScript* miniScript) {
	// network
	{
		//
		class ScriptMethodNetworkHTTPClientGET: public MiniScript::ScriptMethod {
		private:
			MiniScript* miniScript { nullptr };
		public:
			ScriptMethodNetworkHTTPClientGET(MiniScript* miniScript):
				MiniScript::ScriptMethod(
					{
						{ .type = MiniScript::TYPE_STRING, .name = "url", .optional = false, .reference = false, .nullable = false },
						{ .type = MiniScript::TYPE_MAP, .name = "getParameters", .optional = false, .reference = false, .nullable = true },
						{ .type = MiniScript::TYPE_MAP, .name = "headers", .optional = false, .reference = false, .nullable = true },
					},
					MiniScript::TYPE_MAP,
					true
				),
				miniScript(miniScript) {}
			const string getMethodName() override {
				return "network.httpclient.get";
			}
			void executeMethod(span<MiniScript::ScriptVariable>& argumentValues, MiniScript::ScriptVariable& returnValue, const MiniScript::ScriptStatement& statement) override {
				string url;
				if (argumentValues.size() <= 3 &&
					MiniScript::getStringValue(argumentValues, 0, url, false) == true &&
					(argumentValues.size() <= 1 || argumentValues[1].getType() == MiniScript::TYPE_MAP) &&
					(argumentValues.size() <= 2 || argumentValues[2].getType() == MiniScript::TYPE_MAP)) {
					// GET parameters
					unordered_map<string, string> getParameters;
					auto getParameterMapPtr = argumentValues[1].getMapPointer();
					if (getParameterMapPtr != nullptr) {
						for (const auto& [key, value]: *getParameterMapPtr) {
							getParameters[key] = value->getValueAsString();
						}
					}
					// headers
					unordered_map<string, string> headers;
					auto headersMapPtr = argumentValues[2].getMapPointer();
					if (headersMapPtr != nullptr) {
						for (const auto& [key, value]: *headersMapPtr) {
							headers[key] = value->getValueAsString();
						}
					}
					//
					try {
						HTTPClient httpClient;
						httpClient.setMethod(HTTPClient::HTTP_METHOD_GET);
						httpClient.setGETParameters(getParameters);
						httpClient.setHeaders(headers);
						httpClient.setURL(url);
						httpClient.execute();
						string response;
						char c;
						while (httpClient.getResponse().eof() == false) {
							httpClient.getResponse().get(c);
							response+= c;
						}
						//
						returnValue.setType(MiniScript::TYPE_MAP);
						returnValue.setMapEntry("status", static_cast<int64_t>(httpClient.getStatusCode()));
						returnValue.setMapEntry("response", response);
						//
						MiniScript::ScriptVariable responseHeaders;
						responseHeaders.setType(MiniScript::TYPE_MAP);
						for (const auto& [headerName, headerValue]: httpClient.getResponseHeaders()) {
							responseHeaders.setMapEntry(headerName, headerValue);
						}
						returnValue.setMapEntry("headers", responseHeaders);
					} catch (Exception& exception) {
						Console::println("An error occurred: " + string(exception.what()));
					}
				} else {
					Console::println(getMethodName() + "(): " + miniScript->getStatementInformation(statement) + ": argument mismatch: expected arguments: " + miniScript->getArgumentInformation(getMethodName()));
					miniScript->startErrorScript();
				}
			}
		};
		miniScript->registerMethod(new ScriptMethodNetworkHTTPClientGET(miniScript));
	}
}
