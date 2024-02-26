#pragma once

#include <string>

#include <minitscript/minitscript.h>
#include <minitscript/minitscript/fwd-minitscript.h>

using std::string;

using minitscript::minitscript::MinitScript;
using _Context = minitscript::minitscript::Context;

/** 
 * Script library
 * @author Andreas Drewke
 */
class minitscript::minitscript::Library
{
protected:
	_Context* context { nullptr };

public:
	// forbid class copy
	_FORBID_CLASS_COPY(Library)

	/**
	 * Public constructor
	 * @param context context or nullptr
	 */
	Library(_Context* context);

	/**
	 * @return context
	 */
	inline _Context* getContext() {
		return context;
	}

	/**
	 * Set context
	 * @param context context
	 */
	inline void setContext(_Context* context) {
		this->context = context;
	}

	/**
	 * Destructor
	 */
	virtual ~Library();

	/**
	 * Load script from library
	 * @param pathName path name
	 * @param fileName file name
	 * @param basePathName base path name
	 * @return MinitScript script from library
	 */
	virtual MinitScript* loadScript(const string& pathName, const string& fileName, const string& basePathName = string());

};
