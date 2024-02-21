#pragma once

#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/os/network/NetworkException.h>

using minitscript::os::network::NetworkException;

/**
 * Socket exception class
 * @author Andreas Drewke
 */
class minitscript::os::network::NetworkSocketException: public NetworkException {
public:
	/**
	 * Public constructor
	 * @param &message message exception message
	 */
	NetworkSocketException(const string &message) throw();
};
