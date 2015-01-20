//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io All Rights Reserved.
//  
//  StringHelpers.cpp
//  
//  
//__________________________________________________________________________________________________

#include "stdafx.h"
#include "StringHelpers.h"
#include <string.h>

namespace 
{

	// string helpers
	void BuildStringInternal( std::string &out, const wchar_t *string, U32 length )
	{
		char *workingBuffer = (char*)alloca( length*3 );
#if !defined(__GNUC__) && defined(_WINDOWS)

	int count = ::WideCharToMultiByte( CP_UTF8, 0, string, length, workingBuffer, length*3, NULL, NULL );
	if(count >= 0)
		out.assign(workingBuffer, count);
#else
    int count = (int)wcstombs(workingBuffer, string, length*3);
	if(count >= 0)
		out.assign(workingBuffer, count);
#endif // _WINDOWS
	}

	void BuildWideStringInternal( std::wstring &out, const char *string, U32 length )
	{
    U32 maxlen = (length*2)*sizeof(wchar_t);
		wchar_t *workingBuffer = (wchar_t*)alloca( maxlen );
#if !defined(__GNUC__) && defined(_WINDOWS)
	int count = ::MultiByteToWideChar( CP_UTF8, 0, string, length, workingBuffer, length*2 );
	if(count >= 0)
		out.assign(workingBuffer, count);
#else
    int count = mbstowcs(workingBuffer, string, length*2);
	if(count >= 0)
	    out.assign(workingBuffer, count);
#endif // _WINDOWS
	}


} // anonymous namespace


// string functions
void StringHelpers::BuildString( std::string &out, const wchar_t *string )
{
	BuildStringInternal( out, string, (U32)wcslen(string));
}

void StringHelpers::BuildString( std::string &out, const std::wstring &string )
{
	BuildStringInternal( out, string.c_str(), (U32)string.length());
}

void StringHelpers::BuildWideString( std::wstring &out, const char *string )
{
	BuildWideStringInternal( out, string, (U32)strlen(string));
}

void StringHelpers::BuildWideString( std::wstring &out, const std::string &string )
{
	BuildWideStringInternal( out, string.c_str(), (U32)string.length());
}

// debug streaming
void StringHelpers::FormatStringEscaped( std::ostream &stream, const char *string )
{
	stream << '\"';
	while ( *string )
	{
		switch( *string )
		{
		case '\\':
			stream << "\\\\";
			break;

		case '\"':
			stream << "\\\"";
			break;

		case '\n':
			stream << "\\n";
			break;

		case '\r':
			stream << "\\r";
			break;

		case '\a':
			stream << "\\b";
			break;

		case '\b':
			stream << "\\b";
			break;

		case '\t':
			stream << "\\t";
			break;

		default:
			if ( *string < 32 )
			{
				stream << "\\x";
				static const char *hexArray = "0123456789ABCDEF";
				stream << hexArray[*string/16] << hexArray[*string%16];
			} else {
				stream << *string;
			}
		}
		++string;
	}
	stream << "\"";
}


void StringHelpers::Replace( std::string &string, const std::string &src, const std::string &dest )
{
	size_t index = string.find(src);
	while ( index != string.npos )
	{
		string.replace(string.begin()+index, string.begin()+index+src.size(), dest.begin(), dest.end());
		index = string.find(src, index + dest.size());
	}
}
