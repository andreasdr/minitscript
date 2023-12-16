#pragma once

#include <string>
#include <set>
#include <unordered_set>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/fwd-miniscript.h>
#include <miniscript/utilities/Properties.h>

using std::string;
using std::set;
using std::unordered_set;

using miniscript::utilities::Properties;

/**
 * MiniScript documentation
 * @author Andreas Drewke
 */
class miniscript::miniscript::Documentation {
public:
	/**
	 * Generate methods documentation
	 * @param heading heading
	 * @param mainHeadingIdx main heading index
	 * @param miniScript MiniScript script instance
	 * @param descriptions descriptions
	 * @param descriptionPrefix desscription prefix
	 * @param categories categories
	 * @param allClassMethods all class methods
	 * @param omitMiniScript omit MiniScript script instance
	 * @return documentation
	 */
	static const string generateMethodsDocumentation(
		const string& heading,
		int mainHeadingIdx,
		MiniScript* miniScript,
		Properties& descriptions,
		const string& descriptionPrefix,
		unordered_set<string>& categories,
		const set<string>& allClassMethods,
		MiniScript* omitMiniScript = nullptr
	);

	/**
	 * Generate classes documentation
	 * @param heading heading
	 * @param mainHeadingIdx main heading index
	 * @param miniScript MiniScript script instance
	 * @param descriptions descriptions
	 * @param descriptionPrefix description prefix
	 * @param allClassMethods all class methods
	 * @return documentation
	 */
	static const string generateClassesDocumentation(
		const string& heading,
		int mainHeadingIdx,
		MiniScript* miniScript,
		Properties& descriptions,
		const string& descriptionPrefix,
		set<string>& allClassMethods
	);

	/**
	 * Generate operators documentation
	 * @param heading heading
	 * @param mainHeadingIdx main heading index
	 * @param miniScript MiniScript script instance
	 * @return documentation
	 */
	static const string generateOperatorsDocumentation(
		const string& heading,
		int mainHeadingIdx,
		MiniScript* miniScript
	);
};
