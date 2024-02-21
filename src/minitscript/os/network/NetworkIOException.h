#pragma once

#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/os/network/NetworkException.h>

using std::string;

using minitscript::os::network::NetworkException;

/**
 * Base exception class for network IO exceptions
 * @author Andreas Drewke
 */
class minitscript::os::network::NetworkIOException: public NetworkException {
public:
	/**
	 * Public constructor
	 * @param &message message exception message
	 */
	NetworkIOException(const string &message) throw();
};
