#include <minitscript/minitscript/Version.h>

#include <string>

#include <minitscript/minitscript.h>

using std::string;

using minitscript::minitscript::Version;

string Version::getVersion() {
	return "0.9.28 PRE-BETA";
}

string Version::getCopyright() {
	return "Developed 2012-2024 by Andreas Drewke, Dominik Hepp, Kolja Gumpert, drewke.net, mindty.com. Please see the license @ https://github.com/Mindty-Kollektiv/minitscript/blob/master/LICENSE";
}
