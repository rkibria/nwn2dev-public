#ifndef _SOURCE_PROGRAMS_SKYWINGUTILS_ENCODING_CHARSETCONV_H
#define _SOURCE_PROGRAMS_SKYWINGUTILS_ENCODING_CHARSETCONV_H

#ifdef _MSC_VER
#pragma once
#endif

namespace swutil
{
	//
	// Convert 8-bit characters from the given codepage to UTF-8.
	// Text and Result MAY be the same buffer.
	//
	bool
	UTF8Encode(
		nwn2dev__in const std::string & Text,
		nwn2dev__out std::string & Result,
		nwn2dev__in UINT Codepage = CP_ACP
		);

	//
	// Convert 16-bit characters to UTF-8.
	//
	bool
	UTF8Encode(
		nwn2dev__in const std::wstring & Text,
		nwn2dev__out std::string & Result
		);

	//
	// Convert UTF-8 characters to 16-bit characters.
	//
	bool
	UTF8Decode(
		nwn2dev__in const std::string & UTF8,
		nwn2dev__out std::wstring & Result
		);

	//
	// Convert 8-bit characters to 16-bit characters using the given codepage.
	//
	bool
	AnsiToUnicode(
		nwn2dev__in const std::string & Ansi,
		nwn2dev__out std::wstring & Result,
		nwn2dev__in UINT Codepage = CP_ACP
		);

	//
	// Convert 16-bit characters to 8-bit characters using the given codepage.
	//
	bool
	UnicodeToAnsi(
		nwn2dev__in const std::wstring & Unicode,
		nwn2dev__out std::string & Result,
		nwn2dev__in UINT Codepage = CP_ACP
		);

	//
	// Case conversion to lowercase.
	//
	template<class T> struct strtolwr : public std::unary_function<T, T>
	{

		wchar_t operator()(const wchar_t& arg)
		{
			return iswupper(arg) ? towlower(arg) : arg;
		}

		char operator()(const char& arg)
		{
			return isupper((unsigned char)arg) ? tolower((unsigned char)arg) : arg;
		}

	};

	//
	// Case conversion to lowercase.
	//
	template<class T> inline std::basic_string<T>& strlwr(std::basic_string<T>& Text)
	{
		std::transform(Text.begin(), Text.end(), Text.begin(), strtolwr<T>());
		return Text;
	}

	//
	// Case conversion to uppercase.
	//
	template<class T> struct strtoupr : public std::unary_function<T, T>
	{

		wchar_t operator()(const wchar_t& arg)
		{
			return iswupper(arg) ? arg : towupper(arg);
		}

		char operator()(const char& arg)
		{
			return isupper((unsigned char)arg) ? arg : (char)toupper((unsigned char)arg);
		}

	};

	//
	// Case conversion to uppercase.
	//
	template<class T> inline std::basic_string<T>& strupr(std::basic_string<T>& Text)
	{
		std::transform(Text.begin(), Text.end(), Text.begin(), strtoupr<T>());
	}

}

#endif


