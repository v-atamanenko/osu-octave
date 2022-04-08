////////////////////////////////////////////////////////////////
//                                                            //
//      OsUtils.h                                             //
//      HaselLoyance 2017-2018, Unlicense                     //
//      https://github.com/HaselLoyance/osu-parser            //
//                                                            //
////////////////////////////////////////////////////////////////

#ifndef OSU_PARSER_UTILS_H
#define OSU_PARSER_UTILS_H

#include <algorithm>
#include <cctype>
#include <string>
#include <math.h>
#include <limits>
#include "OsTypes.h"

#define IsBitSet(variable, bitNumber) bool((variable) & (1 << (bitNumber)))

namespace osuParser
{
	// Splits a string into multiple parts with given delimiter
	//   str, string, string to split
	//   delimiter, string, in what places to split the string
	//   output, ref vector<string>, all parts of the splitted string
	// Eg: SplitString("1|2|3,4|5|6", ',', output) -> ["1|2|3","4|5|6"]
	void SplitString(const std::string & str, const std::string & delimiter, std::vector<std::string> & output);

	// Trims (removes whitespace) passed string
	//   str, ref string, string to trim
	void TrimString(std::string & str);

    // Trims quotes (") from passed string
    //   str, ref string, string to trim
    void TrimStringQuotes(std::string & str);

	// Checks to see if two double values are equal
	//   a, double, first value to check
	//   b, double, second value to check
	// Returns true if both values are equal, false otherwise
	bool IsEqualDouble(const double & a, const double & b);

	// String names for each available mod
	const extern std::vector<std::string> _modNames;

	// String names for each available game mode
	const extern std::vector<std::string> _modeNames;

	// String names for each available sample set
	const extern std::vector<std::string> _sampleSetNames;

	// String names for each available beatmap event
	const extern std::vector<std::string> _eventNames;

	// String names for each available hit sound type
	const extern std::vector<std::string> _hitSoundNames;

	// String names for each available slider type
	const extern std::vector<std::string> _sliderNames;

	// Converts any enumerated type value to the string from
	// associated vector of string values
	// <T>: enumerated type
	//   val, T, value of the enumerated type
	//   names, vector<string>, dictionary of string values for enum
	//   defaultTo, string, what to default return value to if there is
	//     no associated string value, defaults to "Unknown"
	// Returns a string representation of the enumerated type value
	template<typename T>
	std::string EnumToString(const T & val, const std::vector<std::string> & names, const std::string & defaultTo = "Unknown")
	{
		if (val < 0 || val > names.size())
		{
			return defaultTo;
		}

		return names[val];
	}

	// Converts string value to any enumerated type value via
	// associated vector of string values
	// <T>: enumerated type
	//   val, string, string value
	//   names, vector<string>, dictionary of string values for enum
	//   defaultTo, T, what to default return value to if there is
	//     no associated enumerated type value, defaults to T(0)
	// Returns a T representation of the string value
	template<typename T>
	T StringToEnum(const std::string & val, const std::vector<std::string> & names, const T & defaultTo = static_cast<T>(0))
	{
		for (size_t i = 0; i < names.size(); i++)
		{
			if (names[i] == val)
			{
				return static_cast<T>(i);
			}
		}

		return defaultTo;
	}
}

#endif
