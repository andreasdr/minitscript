#pragma once

#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/network/httpclient/fwd-minitscript.h>
#include <minitscript/utilities/ExceptionBase.h>

using _ExceptionBase = minitscript::utilities::ExceptionBase;

/**
 * HTTP client exception
 * @author Andreas Drewke
 */
class minitscript::network::httpclient::HTTPClientException: public _ExceptionBase {
public:
	/**
	 * @brief public constructor
	 * @param &message message exception message
	 */
	HTTPClientException(const string &message) throw();
};
