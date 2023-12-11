#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/fwd-miniscript.h>
#include <miniscript/utilities/UTF8CharacterIterator.h>

using std::string;
using std::string_view;
using std::vector;

/**
 * UTF8 String tools class
 * @author Andreas Drewke
 */
class miniscript::utilities::UTF8StringTools final
{
public:
	/**
	 * Checks if string starts with prefix
	 * @param src source string
     * @param prefix prefix string
	 * @return bool
	 */
	inline static const bool startsWith(const string& src, const string& prefix) {
		return src.find(prefix) == 0;
	}

	/**
	 * Checks if string starts with prefix
	 * @param src source string
     * @param prefix prefix string
	 * @return bool
	 */
	inline static const bool viewStartsWith(const string_view& src, const string& prefix) {
		return src.find(prefix) == 0;
	}

	/**
	 * Checks if string ends with suffix
	 * @param src source string
     * @param suffix suffix string
	 * @return bool
	 */
	inline static const bool endsWith(const string& src, const string& suffix) {
		return
			src.size() >= suffix.size() &&
			src.compare(src.size() - suffix.size(), suffix.size(), suffix) == 0;
	}

	/**
	 * Checks if string ends with suffix
	 * @param src source string
     * @param suffix suffix string
	 * @return bool
	 */
	inline static const bool viewEndsWith(const string_view& src, const string& suffix) {
		return
			src.size() >= suffix.size() &&
			src.compare(src.size() - suffix.size(), suffix.size(), suffix) == 0;
	}

	/**
	 * Replace string with another string
	 * @param src source string to be processed
	 * @param what what to replace
	 * @param by to replace by
	 * @param beginIndex index to begin with
	 * @param cache src Utf8 position cache
	 * @return new string
	 */
	static const string replace(
		const string& src,
		const string& what,
		const string& by,
		int64_t beginIndex = 0,
		::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr
	);

	/**
	 * Finds first index of given string
	 * @param src source string
	 * @param what what
	 * @param beginIndex index to begin with
	 * @param cache src Utf8 position cache
	 * @return index or -1 if not found
	 */
	inline static int64_t indexOf(
		const string& src,
		const string& what,
		int64_t beginIndex = 0,
		::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr
	) {
		return firstIndexOf(src, what, beginIndex, cache);
	}

	/**
	 * Finds first index of given string
	 * @param src source string
	 * @param what what
	 * @param beginIndex index to begin with
	 * @param cache src Utf8 position cache
	 * @return index or -1 if not found
	 */
	static int64_t firstIndexOf(
		const string& src,
		const string& what,
		int64_t beginIndex = 0,
		::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr
	);

	/**
	 * Finds last index of given string
	 * @param src source string
	 * @param what what
	 * @param beginIndex index to begin with
	 * @param cache src Utf8 position cache
	 * @return index or -1 if not found
	 */
	static int64_t lastIndexOf(
		const string& src,
		const string& what,
		int64_t beginIndex = 0,
		::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr
	);

	/**
	 * Finds first index of characters provided within given string
	 * @param src source string
	 * @param what what
	 * @param beginIndex index to begin with
	 * @param srcCache src Utf8 position cache
	 * @param whatCache what Utf8 position cache
	 * @return index or -1 if not found
	 */
	static int64_t firstIndexOfChars(const string& src, const string& what, int64_t beginIndex = 0, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* srcCache = nullptr, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* whatCache = nullptr);

	/**
	 * Finds last index of characters provided within given string
	 * @param src source string
	 * @param what what
	 * @param endIndex index to end with
	 * @param srcCache src Utf8 position cache
	 * @param whatCache what Utf8 position cache
	 * @return index or -1 if not found
	 */
	static int64_t lastIndexOfChars(const string& src, const string& what, int64_t endIndex = -1, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* srcCache = nullptr, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* whatCache = nullptr);

	/**
	 * Returns substring of given string from begin index to end index
	 * @param src source string
	 * @param beginIndex begin index
	 * @param endIndex end index or -1
	 * @param cache src Utf8 position cache
	 * @return new string
	 */
	inline static const string substring(const string& src, int64_t beginIndex, int64_t endIndex = string::npos, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* srcCache = nullptr, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr) {
		auto result = viewSubstring(string_view(src), beginIndex, endIndex, cache);
		return string(result.data(), result.size());
	}

	/**
	 * Returns substring of given string from begin index to end index
	 * @param src source string
	 * @param beginIndex begin index
	 * @param endIndex end index or -1
	 * @param cache src Utf8 position cache
	 * @return new string
	 */
	static const string_view viewSubstring(const string_view& src, int64_t beginIndex, int64_t endIndex, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Checks if string equals ignoring case
	 * @param string1 string 1
	 * @param string2 string 2
	 * @param string1Cache string1 Utf8 position cache
	 * @param string2Cache string2 Utf8 position cache
	 * @return equals
	 */
	static bool equalsIgnoreCase(
		const string& string1,
		const string& string2,
		::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* string1Cache = nullptr,
		::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* string2Cache = nullptr
	);

	/**
	 * Trim string
	 * @param src source string
	 * @param cache src Utf8 position cache
	 * @return trimmed string
	 */
	static const string trim(const string& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Trim string
	 * @param src source string
	 * @param cache src Utf8 position cache
	 * @return trimmed string
	 */
	static const string_view viewTrim(const string_view& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Transform string to lower case
	 * @param src source string
	 * @param cache Utf8 position cache
	 * @return transformed string
	 */
	static const string toLowerCase(const string& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Transform string to upper case
	 * @param src source string
	 * @param cache Utf8 position cache
	 * @return transformed string
	 */
	static const string toUpperCase(const string& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Check if pattern matches whole string
	 * @param src source string to test
	 * @param pattern pattern
	 * @return if pattern matches whole string
	 */
	static bool regexMatch(const string& src, const string& pattern);

	/**
	 * Do regex pattern search
	 * @param src source string to test
	 * @param pattern pattern
	 * @return if search was successful
	 */
	static bool regexSearch(const string& src, const string& pattern);

	/**
	 * Replace regex pattern with given string
	 * @param src source string to operate on
	 * @param pattern pattern to search
	 * @param by string that will replace pattern occurrances
	 */
	static const string regexReplace(const string& src, const string& pattern, const string& by);

	/**
	 * Tokenize
	 * @param str string to tokenize
	 * @param delimiters delimiters
	 * @param emptyTokens include empty tokens
	 * @return tokens
	 */
	static const vector<string> tokenize(const string& str, const string& delimiters, bool emptyTokens = false);

	/**
	 * Pad a string left
	 * @param src source
	 * @param by by
	 * @param toSize to size
	 * @param cache src Utf8 position cache
	 */
	static const string padLeft(const string& src, const string& by, int64_t toLength, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Pad a string right
	 * @param src source
	 * @param by by
	 * @param toSize to size
	 * @param cache src Utf8 position cache
	 */
	static const string padRight(const string& src, const string& by, int64_t toLength, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Indent a string
	 * @param src source
	 * @param with with
	 * @param count count
	 */
	inline static const string indent(const string& src, const string& with, int64_t count) {
		string result;
		for (auto i = 0; i < count; i++) result+= with;
		return result + src;
	}

	/**
	 * Indent a string
	 * @param src source
	 * @param with with
	 * @param count count
	 */
	inline static const string generate(const string& what, int64_t count = 1) {
		string result;
		for (auto i = 0; i < count; i++) result+= what;
		return result;
	}

	/**
	 * Get Utf8 string length
	 * @param str string
	 * @param cache str Utf8 position cache
	 * @return utf8 string length
	 */
	static int64_t getLength(const string& str, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Get Utf8 character at given index
	 * @param str string
	 * @param index index
	 * @param cache src Utf8 position cache
	 */
	static const string getCharAt(const string& str, int64_t index, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

	/**
	 * Get Utf8 binary buffer index
	 * @param str string
	 * @param charIdx character index
	 * @param cache src Utf8 position cache
	 * @return UTF binary buffer position from given character/code point index
	 */
	static int64_t getUtf8BinaryIndex(const string& str, int64_t charIdx, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache = nullptr);

};

