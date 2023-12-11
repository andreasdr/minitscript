#include <miniscript/utilities/UTF8StringTools.h>

#include <algorithm>
#include <functional>
#include <regex>
#include <string>
#include <string_view>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/Character.h>
#include <miniscript/utilities/UTF8StringTokenizer.h>
#include <miniscript/utilities/UTF8CharacterIterator.h>

#include <miniscript/utilities/Console.h>

using std::find_if;
using std::isspace;
using std::regex;
using std::regex_match;
using std::regex_replace;
using std::replace;
using std::string;
using std::string_view;
using std::tolower;
using std::toupper;
using std::transform;

using miniscript::utilities::UTF8StringTools;

using miniscript::utilities::Character;
using miniscript::utilities::UTF8StringTokenizer;
using miniscript::utilities::UTF8CharacterIterator;

using miniscript::utilities::Console;

const string UTF8StringTools::replace(
	const string& src,
	const string& what,
	const string& by,
	int64_t beginIndex,
	UTF8CharacterIterator::UTF8PositionCache* cache)
{
	auto u8BeginIndex = getUtf8BinaryIndex(src, beginIndex, cache);
	//
	string result = src;
	if (what.empty()) return result;
	while ((u8BeginIndex = result.find(what, u8BeginIndex)) != std::string::npos) {
		result.replace(u8BeginIndex, what.length(), by);
		u8BeginIndex += by.length();
	}
	return result;
}

int64_t UTF8StringTools::firstIndexOf(
	const string& src,
	const string& what,
	int64_t beginIndex,
	UTF8CharacterIterator::UTF8PositionCache* cache
) {
	// utf8 character iterator
	UTF8CharacterIterator u8It(src, cache);
	u8It.seekCharacterPosition(beginIndex);
	//
	auto index = src.find(what, u8It.getBinaryPosition());
	if (index == string::npos) {
		return string::npos;
	} else {
		u8It.seekBinaryPosition(index);
		return static_cast<int64_t>(u8It.getCharacterPosition());
	}
}

int64_t UTF8StringTools::lastIndexOf(
	const string& src,
	const string& what,
	int64_t beginIndex,
	::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache
) {
	// utf8 character iterator
	UTF8CharacterIterator u8It(src, cache);
	u8It.seekCharacterPosition(beginIndex);
	auto binaryBeginIndex = u8It.getBinaryPosition();
	//
	int64_t index = -1;
	while (true == true) {
		auto binaryIndex = src.find(what, binaryBeginIndex);
		if (binaryIndex == string::npos) {
			return index;
		} else {
			u8It.seekBinaryPosition(binaryIndex);
			index = u8It.getCharacterPosition();
			//
			binaryBeginIndex = binaryIndex + what.size();
		}
	}
	//
	return index;
}

int64_t UTF8StringTools::firstIndexOfChars(const string& src, const string& what, int64_t beginIndex, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* srcCache, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* whatCache) {
	// utf8 character iterator
	UTF8CharacterIterator whatU8It(what, whatCache);
	//
	int64_t index = -1;
	while (whatU8It.hasNext() == true) {
		auto whatChar = Character::toString(whatU8It.next());
		auto whatIndex = UTF8StringTools::indexOf(src, whatChar, beginIndex, srcCache);
		if (whatIndex != string::npos) index = index == -1?whatIndex:(::miniscript::math::Math::min(index, whatIndex));
	}
	//
	if (index == string::npos) {
		return string::npos;
	} else {
		return index;
	}
}

int64_t UTF8StringTools::lastIndexOfChars(const string& src, const string& what, int64_t endIndex, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* srcCache, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* whatCache) {
	// utf8 character iterator
	UTF8CharacterIterator whatU8It(what, whatCache);
	//
	int64_t currentIndex = 0;
	int64_t index = -1;
	while (true == true) {
		auto hit = false;
		while (whatU8It.hasNext() == true) {
			auto whatChar = Character::toString(whatU8It.next());
			auto whatIndex = UTF8StringTools::indexOf(src, whatChar, currentIndex, srcCache);
			if (whatIndex != string::npos && whatIndex < endIndex) {
				hit = true;
				index = ::miniscript::math::Math::max(index, whatIndex);
			}
			//
			currentIndex++;
		}
		if (hit == false) break;
	}
	//
	if (index == string::npos) {
		return string::npos;
	} else {
		return index;
	}
}

const string_view UTF8StringTools::viewSubstring(const string_view& src, int64_t beginIndex, int64_t endIndex, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	// utf8 character iterator
	UTF8CharacterIterator u8It(src, cache);
	u8It.seekCharacterPosition(beginIndex);
	auto u8BeginIndex = u8It.getBinaryPosition();
	//
	if (endIndex == string::npos) {
		return src.substr(u8BeginIndex);
	} else {
		u8It.seekCharacterPosition(endIndex) ;
		auto u8EndIndex = u8It.getBinaryPosition();
		//
		return src.substr(u8BeginIndex, u8EndIndex - u8BeginIndex);
	}
}

bool UTF8StringTools::equalsIgnoreCase(
	const string& string1,
	const string& string2,
	UTF8CharacterIterator::UTF8PositionCache* string1Cache,
	UTF8CharacterIterator::UTF8PositionCache* string2Cache
) {
	if (getLength(string1, string1Cache) != getLength(string2, string2Cache)) return false;
	// utf8 character iterator
	UTF8CharacterIterator string1U8It(string1, string1Cache);
	UTF8CharacterIterator string2U8It(string1, string2Cache);
	//
	while (true == true) {
		auto string1Next = string1U8It.hasNext();
		auto string2Next = string2U8It.hasNext();
		if (string1Next != string2Next) return false;
		if (string1Next == false) return true;
		auto c1 = Character::toUpperCase(string1U8It.next());
		auto c2 = Character::toUpperCase(string2U8It.next());
		if (c1 != c2) return false;
	}
	//
	return false;
}

const string UTF8StringTools::trim(const string& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	auto result = viewTrim(string_view(src), cache);
	return string(result.data(), result.size());
}

const string_view UTF8StringTools::viewTrim(const string_view& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	// utf8 character iterator
	UTF8CharacterIterator u8It(src, cache);
	//
	int64_t firstNonSpace = string::npos;
	int64_t lastNonSpace = string::npos;
	while (u8It.hasNext() == true) {
		auto c = u8It.next();
		if (Character::isSpace(c) == false) {
			if (firstNonSpace == string::npos) firstNonSpace = u8It.getCharacterPosition() - 1;
			lastNonSpace = u8It.getCharacterPosition() - 1;
		}
	}
	//
	if (firstNonSpace == string::npos) return string();
	//
	return viewSubstring(src, firstNonSpace, lastNonSpace + 1);
}

const string UTF8StringTools::toLowerCase(const string& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	string result;
	// utf8 character iterator
	UTF8CharacterIterator u8It(src, cache);
	//
	while (u8It.hasNext() == true) Character::appendToString(result, Character::toLowerCase(u8It.next()));
	//
	return result;
}

const string UTF8StringTools::toUpperCase(const string& src, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	string result;
	// utf8 character iterator
	UTF8CharacterIterator u8It(src, cache);
	//
	while (u8It.hasNext() == true) Character::appendToString(result, Character::toUpperCase(u8It.next()));
	//
	return result;
}

bool UTF8StringTools::regexMatch(const string& src, const string& pattern) {
	// TODO: return found groups
	return regex_match(src, regex(pattern, std::regex::ECMAScript));
}

bool UTF8StringTools::regexSearch(const string& src, const string& pattern) {
	// TODO: return found groups
	return regex_search(src, regex(pattern, std::regex::ECMAScript));
}

const string UTF8StringTools::regexReplace(const string& src, const string& pattern, const string& by) {
	return regex_replace(src, regex(pattern, std::regex::ECMAScript), by);
}

const vector<string> UTF8StringTools::tokenize(const string& str, const string& delimiters, bool emptyTokens) {
	UTF8StringTokenizer t;
	t.tokenize(str, delimiters, emptyTokens);
	return t.getTokens();
}

const string UTF8StringTools::padLeft(const string& src, const string& by, int64_t toLength, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	auto result = src;
	while (getLength(result) < toLength) result = by + result;
	return result;
}

const string UTF8StringTools::padRight(const string& src, const string& by, int64_t toLength, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	auto result = src;
	UTF8CharacterIterator::UTF8PositionCache resultCache;
	if (cache != nullptr) resultCache = *cache;
	while (getLength(result, &resultCache) < toLength) result = result + by;
	return result;
}

int64_t UTF8StringTools::getLength(const string& str, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	::miniscript::utilities::UTF8CharacterIterator u8It(str, cache);
	while (u8It.hasNext() == true) u8It.next();
	return u8It.getCharacterPosition();
}

const string UTF8StringTools::getCharAt(const string& str, int64_t index, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	// utf8 character iterator
	::miniscript::utilities::UTF8CharacterIterator u8It(str, cache);
	u8It.seekCharacterPosition(index);
	//
	return u8It.hasNext() == true?::miniscript::utilities::Character::toString(u8It.next()):string();
}

int64_t UTF8StringTools::getUtf8BinaryIndex(const string& str, int64_t charIdx, ::miniscript::utilities::UTF8CharacterIterator::UTF8PositionCache* cache) {
	::miniscript::utilities::UTF8CharacterIterator u8It(str, cache);
	u8It.seekCharacterPosition(charIdx);
	return u8It.getBinaryPosition();
}
