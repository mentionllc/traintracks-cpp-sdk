//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  StringUtils.cpp
//  
//__________________________________________________________________________________________________

#include "stdafx.h"
#include "util/StringUtils.h"
#include <string>
#include <algorithm>
#include <functional>
#include <vector>

#if defined(__GNUC__) && !defined(_WINDOWS)
#include <errno.h>
#endif

char* StringCopy( char* pDest, U32 nDestSize, const U16* pSrc )
{
	U16  u;
	U16  u1;
	U16  u2;
	char*   pEnd;

	if( pDest && nDestSize && pSrc )
	{
		pEnd = pDest + nDestSize - 5;
		do
		{
			u1 = *pSrc++;
			if( u1 <= 0x007F )
			{
				*pDest++ = ( U8 )u1;
			}
			else if( u1 <= 0x07FF )
			{
				*pDest++ = ( U8 )( 0xC0 | (( u1 & 0x07C0 ) >> 6 ));
				*pDest++ = ( U8 )( 0x80 | ( u1 & 0x003F ));
			}
			else if(( u1 <= 0xD7FF ) || ( u1 >= 0xE000 ))
			{
				*pDest++ = ( U8 )( 0xE0 | (( u1 & 0xF000 ) >> 12 ));
				*pDest++ = ( U8 )( 0x80 | (( u1 & 0x0FC0 ) >> 6 ));
				*pDest++ = ( U8 )( 0x80 | ( u1 & 0x003F ));
			}
			else
			{
				u2 = *pSrc++;
				u = (( u1 & 0x03C0 ) >> 6 ) + 1;
				*pDest++ = ( U8 )( 0xF0 | (( u & 0x1C ) >> 2 ));
				*pDest++ = ( U8 )( 0x80 | (( u & 0x03 ) << 4 ) | (( u1 & 0x003F ) >> 2 ));
				*pDest++ = ( U8 )( 0x80 | (( u1 & 0x0003 ) << 4 ) | (( u2 & 0x03C0 ) >> 6 ));
				*pDest++ = ( U8 )( 0x80 | ( u2 & 0x003F ));
			}
		}
		while( u1 && ( pDest < pEnd ));
		*pDest = 0;
	}
	return pDest;
}

// Convert UTF8 to UTF16...
U16* StringCopy( U16* pDest, U32 nDestSize, const char* pSrc )
{
	U32 c1;
	U32 c2;
	U32 c3;
	U32 c4;
	U32 w;
	U16* pEnd;

	if( pDest && nDestSize && pSrc )
	{
		pEnd = pDest + nDestSize - 3;
		do
		{
			c1 = *pSrc++;
			if(( c1 & 0xF0 ) == 0xF0 )
			{
				c2 = *pSrc++;
				c3 = *pSrc++;
				c4 = *pSrc++;
				w = ((( c1 & 0x03 ) << 2 ) | (( c2 & 0x30 ) >> 4 )) - 1;
				*pDest++ = ( U16 )( 0xD800 | ( w << 6 ) | (( c2 & 0xF0 ) << 2 ) | (( c3 & 0x30 ) >> 4 ));
				*pDest++ = ( U16 )( 0xD3 | (( c3 & 0x0F ) << 6 ) | ( c4 & 0x3F ));
			}
			else if(( c1 & 0xE0 ) == 0xE0 )
			{
				c2 = *pSrc++;
				c3 = *pSrc++;
				*pDest++ = ( U16 )((( c1 & 0x0F ) << 12 ) | (( c2 & 0x3F ) << 6 ) | ( c3 & 0x3F ));
			}
			else if(( c1 & 0xC0 ) == 0xC0 )
			{
				c2 = *pSrc++;
				*pDest++ = ( U16 )((( c1 & 0x1F ) << 6 ) | ( c2 & 0x3F ));
			}
			else
			{
				*pDest++ = ( U16 )c1;
			}
		}
		while( c1 && ( pDest < pEnd ));
		*pDest = 0;
	}
	return pDest;
}

const std::string StringGetUpper(const std::string& cString)
{
	std::string cUpperString(cString);
	// input range of elements to change
	// where to start to store the changed elements
	std::transform(cUpperString.begin(),cUpperString.end(), cUpperString.begin(),toupper);
	return cUpperString;
}

const std::string StringGetLower(const std::string& cString)
{
	std::string cLowerString(cString);
	// input range of elements to change
	// where to start to store the changed elements
	std::transform(cLowerString.begin(),cLowerString.end(), cLowerString.begin(),tolower);
	return cLowerString;
}

const std::string SQLQuote( const std::string &source )
{
	bool retIsValid = false;
	std::string ret;
	for ( size_t i = 0; i < source.size(); ++i )
	{
		// MySQL apparently escapes \, which is NOT ansi-standard
		if ( source[i] == '\'' || source[i] == '\\' )
		{
			if ( !retIsValid )
			{
				ret.reserve(source.size()*2);
				ret.append( source.begin(), source.begin() + i );
				retIsValid = true;
			}
			if ( source[i] == '\'' )
				ret.append("''");
			else 
				ret.append("\\\\");
		}
		else if ( retIsValid )
			ret.append(1, source[i]);
	}
	return retIsValid ? ret : source;
}

int SplitString(const std::string& cText,const std::string& cSeparators,std::vector<std::string>& cWordList,int nFlags)
{
	cWordList.clear();
	int nFound = 0;
	int nLen = (int)cText.length();
	int nStart, nStop;

	if(nFlags & e_SplitStringFlag_KeepEmptyWords)
		nStart = 0;
	else
		nStart = (int)cText.find_first_not_of(cSeparators);

	while (nStart != (int)std::string::npos && nStart < nLen)
	{
		nStop = (int)cText.find_first_of(cSeparators, nStart);
		if (nStop == (int)std::string::npos)
		{
			nStop = nLen;
		}
		else
		{
			nFound ++;
		}
		cWordList.push_back(cText.substr(nStart, nStop - nStart));
		if(nFlags & e_SplitStringFlag_KeepEmptyWords)
			nStart = nStop + 1;
		else
			nStart = (int)cText.find_first_not_of(cSeparators, nStop+1);
	}
	return nFound;
}

std::string replaceOnce(std::string& result, const std::string& replaceWhat, const std::string& replaceWithWhat)
{
  const int pos = (const int)result.find(replaceWhat);
  if(pos==-1) return result;
  result.replace(pos, replaceWhat.size(), replaceWithWhat);
  return result;
}

std::string relaceAll(std::string& result, const std::string& replaceWhat, const std::string& replaceWithWhat)
{
  while(1)
  {
    const int pos = (const int)result.find(replaceWhat);
    if(pos==-1) break;
    result.replace(pos, replaceWhat.size(), replaceWithWhat);
  }
  return result;
}



#if defined(__linux__)

//! Reimplementation of Microsofts strncpy_s. Returns STRUNCATE on truncation.

int strncpy_s(char* strDestination, U32 sizeInBytes, const char* strSource, U32 nMaxCount)
{
	if (sizeInBytes == 0)
		return EINVAL;

	strDestination[sizeInBytes-1] = 0;

	if (nMaxCount == _TRUNCATE || nMaxCount >= sizeInBytes) {
		strncpy(strDestination, strSource, sizeInBytes);
	}
	else {
		strncpy(strDestination, strSource, nMaxCount);
		strDestination[nMaxCount]=0;
	}

	if (strDestination[sizeInBytes-1] == 0) {
		// strncpy does not leave a terminating zero on trunc
		// so no truncation.
		return 0;
	}
	strDestination[sizeInBytes-1]=0;
	return STRUNCATE;
}

#include <errno.h>

//! Reimplementation of Microsofts strcat_s. Note that the sizeInBytes is the TOTAL buffer size.
int strcat_s(char *strDestination, U32 sizeInBytes, const char *strSource)
{
	if (!strSource)
		return EINVAL;
	if (sizeInBytes == 0)
		return EINVAL;

	strDestination[sizeInBytes-1]=0;
	U32 posCopy = strlen(strDestination);
	if (posCopy >= sizeInBytes)
		return ERANGE;

	strncpy(&strDestination[posCopy], strSource, sizeInBytes-posCopy);
	if (strDestination[sizeInBytes-1]==0)
		return 0;

	strDestination[sizeInBytes-1]=0;
	return ERANGE;
}

int strcpy_s(char* strDestination, U32 sizeInBytes, const char* strSource)
{
	return strncpy_s(strDestination, sizeInBytes, strSource, _TRUNCATE);
}

#endif

#ifdef UNITTEST

#include <assert.h>
#include <string.h>

// TODO: Might want to turn this into a proper unittest. It found a couple of bugs for me.

int main(void)
{
	char zBuf[3];
	int ret;

	// test strcat_s
	zBuf[0] = 0;
	ret = strcat_s(zBuf, sizeof(zBuf), "a");
	assert(ret == 0);
	assert(!strcmp(zBuf, "a"));
	ret = strcat_s(zBuf, sizeof(zBuf),  "b");
	assert(ret == 0);
	assert(!strcmp(zBuf, "ab"));
	ret = strcat_s(zBuf, sizeof(zBuf), "c");
	assert(ret == ERANGE);
	assert(!strcmp(zBuf, "ab"));
	zBuf[0] = 0;
	ret = strcat_s(zBuf, sizeof(zBuf), "abc");
	assert(ret == ERANGE);
	assert(!strcmp(zBuf, "ab"));

	// test strcpy_s
	ret = strcpy_s(zBuf, sizeof(zBuf), "A");
	assert(ret == 0);
	assert(!strcmp(zBuf, "A"));
	ret = strcpy_s(zBuf, sizeof(zBuf), "AB");
	assert(ret == 0);
	assert(!strcmp(zBuf, "AB"));
	ret = strcpy_s(zBuf, sizeof(zBuf), "ABC");
	assert(ret == STRUNCATE);
	assert(!strcmp(zBuf, "AB"));

	// test strncpy_s
	ret = strncpy_s(zBuf, sizeof(zBuf), "abcdefgh", 0);
	assert(ret == 0);
	assert(!strcmp(zBuf, ""));
	ret = strncpy_s(zBuf, sizeof(zBuf), "abcdefgh", 1);
	assert(ret == 0);
	assert(!strcmp(zBuf, "a"));
	ret = strncpy_s(zBuf, sizeof(zBuf), "abcdefgh", 2);
	assert(ret == 0);
	assert(!strcmp(zBuf, "ab"));
	ret = strncpy_s(zBuf, sizeof(zBuf), "abcdefgh", 3);
	assert(ret == STRUNCATE);
	assert(!strcmp(zBuf, "ab"));

	// test strcpy_s
	ret = strcpy_s(zBuf, sizeof(zBuf), "abcdefgh");
	assert(ret == STRUNCATE);
	assert(!strcmp(zBuf, "ab"));

	ret = strcpy_s(zBuf, sizeof(zBuf), "AB");
	assert(ret == 0);
	assert(!strcmp(zBuf, "AB"));

	return 0;
}

#endif
