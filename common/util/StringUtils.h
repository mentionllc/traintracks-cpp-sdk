//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  StringUtils.h
//  
//__________________________________________________________________________________________________
#ifndef UTIL_STRING_UTILS_H__
#define UTIL_STRING_UTILS_H__

#include "util/server_types.h"
#include <string>
#include <vector>

const std::string StringGetUpper(const std::string& cString);
const std::string StringGetLower(const std::string& cString);

enum SplitStringFlags_e
{
  e_SplitStringFlag_KeepEmptyWords = (1 << 0),
};

extern int SplitString(const std::string& cText,const std::string& cSeparators,std::vector<std::string>& cWordList,int nFlags = 0);

extern char* StringCopy( char* pDest, U32 nDestSize, const U16* pSrc );
extern U16* StringCopy( U16* pDest, U32 nDestSize, const char* pSrc );

extern U32 StringFormat( char* dest, const char* format, ... );

extern const std::string StringGetUpper(const std::string& cString);
extern const std::string StringGetLower(const std::string& cString);

extern const std::string SQLQuote( const std::string &source );

extern std::string replaceOnce(std::string& result, const std::string& replaceWhat, const std::string& replaceWithWhat);
extern std::string relaceAll(std::string& result, const std::string& replaceWhat, const std::string& replaceWithWhat);


// replacements for Linux
#if defined(__GNUC__) && !defined(_WINDOWS)
#include <wchar.h>

#if !defined(_TRUNCATE)
#define _TRUNCATE ((U32)-1)
#define STRUNCATE       80
#endif

// Reimplementation of Microsoft's strncpy_s. Returns STRUNCATE on truncation.
extern int strncpy_s(char* strDestination, U32 sizeInBytes, const char* strSource, size_t nMaxCount);

extern int strcat_s(char* strDestination, U32 sizeInBytes, const char* strSource);

extern int strcpy_s(char* strDestination, U32 sizeInBytes, const char* strSource);


#endif

// a functor that does case insensitive comparisons
class CaseInsenstiveLess
{
public:
	// standard string
	bool operator()( const std::string &a, const std::string &b ) const
	{
		return stricmp(a.c_str(),b.c_str()) < 0;
	}
	bool operator()( const std::string &a, const char *b ) const
	{
		return stricmp(a.c_str(),b) < 0;
	}
	bool operator()( const char *a, const std::string &b ) const
	{
		return stricmp(a,b.c_str()) < 0;
	}
	bool operator()( const char *a, const char *b ) const
	{
		return stricmp(a,b) < 0;
	}

#if !defined(__GNUC__) && defined(_WINDOWS)
	// wide string
	bool operator()( const std::wstring &a, const std::wstring &b ) const
	{
		return _wcsicmp(a.c_str(),b.c_str()) < 0;
	}
	bool operator()( const std::wstring &a, const wchar_t *b ) const
	{
		return _wcsicmp(a.c_str(),b) < 0;
	}
	bool operator()( const wchar_t *a, const std::wstring &b ) const
	{
		return _wcsicmp(a,b.c_str()) < 0;
	}
	bool operator()( const wchar_t *a, const wchar_t *b ) const
	{
		return _wcsicmp(a,b) < 0;
	}
#endif

};



#endif
