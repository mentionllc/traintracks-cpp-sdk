//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  StringHelpers.h
//  
//__________________________________________________________________________________________________

#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

#include <string>
#include <ostream>
#include "util/server_types.h"

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String manipulation functions
namespace StringHelpers
{

	const std::string BuildString( const char *string );
	void BuildString( std::string &out, const char *string );
	const std::string BuildString( const wchar_t *string );
	void BuildString( std::string &out, const wchar_t *string );
	const std::string BuildString( const std::string &string );
	void BuildString( std::string &out, const std::string &string );
	const std::string BuildString( const std::wstring &string );
	void BuildString( std::string &out, const std::wstring &string );

	const std::wstring BuildWideString( const char *string );
	void BuildWideString( std::wstring &out, const char *string );
	const std::wstring BuildWideString( const wchar_t *string );
	void BuildWideString( std::wstring &out, const wchar_t *string );
	const std::wstring BuildWideString( const std::string &string );
	void BuildWideString( std::wstring &out, const std::string &string );
	const std::wstring BuildWideString( const std::wstring &string );
	void BuildWideString( std::wstring &out, const std::wstring &string );

	const std::tstring BuildTString( const std::string &string );
	void BuildTString( std::tstring &out, const std::string &string );
	const std::tstring BuildTString( const char *string );
	void BuildTString( std::tstring &out, const char *string );
	const std::tstring BuildTString( const std::wstring &string );
	void BuildTString( std::tstring &out, const std::wstring &string );
	const std::tstring BuildTString( const wchar_t *string );
	void BuildTString( std::tstring &out, const wchar_t *string );

	// debug printing -- strings
	void FormatStringEscaped( std::ostream &stream, const char *string );
  inline void FormatStringEscaped( std::ostream &stream, const std::string &string ) { FormatStringEscaped(stream, (const char *)string.c_str()); }


	// helper object to allow you to directly stream the class with operator<<
	template <class T>
	class EscapedStringClass
	{
	public:
		EscapedStringClass( T object )
			: m_object(object)
		{
			// empty
		}

		friend std::ostream &operator<<( std::ostream &stream, EscapedStringClass object ) { FormatStringEscaped(stream,object.m_object); return stream; }

	private:
		T m_object;
	};

	const EscapedStringClass<const std::string&> EscapedString( const std::string &string );
	const EscapedStringClass<const char*> EscapedString( const char *string );


	// inlines
	inline const std::string BuildString( const char *string ) { return std::string(string); }
	inline void BuildString( std::string &out, const char *string ) { out.assign(string); }
	inline const std::string BuildString( const wchar_t *string ) { std::string ret; BuildString(ret, string); return ret; }
	inline const std::string BuildString( const std::string &string ) { return string; }
	inline void BuildString( std::string &out, const std::string &string ) { out = string; }
	inline const std::string BuildString( const std::wstring &string ) { std::string ret; BuildString(ret,string); return ret; }

	inline const std::wstring BuildWideString( const char *string ) { std::wstring ret; BuildWideString(ret,string); return ret; }
	inline const std::wstring BuildWideString( const wchar_t *string ) { return std::wstring(string); }
	inline void BuildWideString( std::wstring &out, const wchar_t *string ) { out.assign(string); }
	inline const std::wstring BuildWideString( const std::string &string ) { std::wstring ret; BuildWideString(ret, string); return ret; }
	inline const std::wstring BuildWideString( const std::wstring &string ) { std::wstring ret; BuildWideString(ret, string); return ret; }

#ifdef UNICODE
	inline const std::tstring BuildTString( const std::string &string ) { return BuildWideString(string); }
	inline void BuildTString( std::tstring &out, const std::string &string ) { BuildWideString(out,string); }
	inline const std::tstring BuildTString( const char *string ) { return BuildWideString(string); }
	inline void BuildTString( std::tstring &out, const char *string ) { BuildWideString(out,string); }
	inline const std::tstring BuildTString( const std::wstring &string ) { return BuildWideString(string); }
	inline void BuildTString( std::tstring &out, const std::wstring &string ) { BuildWideString(out,string); }
	inline const std::tstring BuildTString( const wchar_t *string ) { return BuildWideString(string); }
	inline void BuildTString( std::tstring &out, const wchar_t *string ) { BuildWideString(out,string); }
#else
	inline const std::tstring BuildTString( const std::string &string ) { return BuildString(string); }
	inline void BuildTString( std::tstring &out, const std::string &string ) { BuildString(out,string); }
	inline const std::tstring BuildTString( const char *string ) { return BuildString(string); }
	inline void BuildTString( std::tstring &out, const char *string ) { BuildString(out,string); }
	inline const std::tstring BuildTString( const std::wstring &string ) { return BuildString(string); }
	inline void BuildTString( std::tstring &out, const std::wstring &string ) { BuildString(out,string); }
	inline const std::tstring BuildTString( const wchar_t *string ) { return BuildString(string); }
	inline void BuildTString( std::tstring &out, const wchar_t *string ) { BuildString(out,string); }
#endif // UNICODE

	inline const EscapedStringClass<const std::string&> EscapedString( const std::string &string ) { return string; }
	inline const EscapedStringClass<const char*> EscapedString( const char *string ) { return string; }

	void Replace( std::string &string, const std::string &src, const std::string &dest );

}

#endif // STRINGHELPERS_H

