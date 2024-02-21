#pragma once

#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/os/network/fwd-minitscript.h>
#include <minitscript/utilities/fwd-minitscript.h>
#include <minitscript/utilities/ExceptionBase.h>

using _ExceptionBase = minitscript::utilities::ExceptionBase;

/**
 * Base exception class for network exceptions
 * @author Andreas Drewke
 */
class minitscript::os::network::NetworkException: public _ExceptionBase {
public:
	/**
	 * Constructor
	 * @param &message message exception message
	 */
	NetworkException(const string &message) throw();
};
