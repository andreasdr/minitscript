#pragma once

#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/fwd-minitscript.h>
#include <minitscript/minitscript/Library.h>

using std::string;

using minitscript::minitscript::MinitScript;

using _Context = minitscript::minitscript::Context;
using _Library = minitscript::minitscript::Library;

/** 
 * Native script library
 * @author Andreas Drewke
 */
class minitscript::minitscript::NativeLibrary: public _Library
{

public:
	// forbid class copy
	_FORBID_CLASS_COPY(NativeLibrary)

	/**
	 * Public constructor
	 * @param context context or nullptr
	 */
	NativeLibrary(_Context* context): _Library(context) {
	}

	/**
	 * Load script from library
	 * @param pathName path name
	 * @param fileName file name
	 * @param basePathName base path name
	 * @return MinitScript script from library
	 */
	virtual MinitScript* loadScript(const string& pathName, const string& fileName, const string& basePathName = string()) override;

};
