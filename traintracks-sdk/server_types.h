//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io All Rights Reserved.
//  
//  server_types
//  
//  
//__________________________________________________________________________________________________

#ifndef SERVER_TYPES_H
#define SERVER_TYPES_H

#pragma once

#include <string>

#if !defined(__GNUC__) && defined(_WINDOWS)
#include <tchar.h>
#endif

#undef  NULL
#if defined( _WIN64 )

#define NULL  0LL
#else
#define NULL  0
#endif

#define FWBIT( n )                    (( U32 )1 << ( n ))
#define FWBIT64( n )                  (( U64 )1 << ( n ))

//
// Platform types
//
#if defined( _WINDOWS )

#define MACHINE_TYPE_LITTLE_ENDIAN
#undef  MACHINE_TYPE_BIG_ENDIAN

#define DllImport               __declspec( dllimport )
#define DllExport               __declspec( dllexport )

//typedef unsigned __int64        U64;
//typedef signed __int64          sint64_cl;
//typedef signed __int64			int64_cl;

#define stricmp                 _stricmp

#elif defined( __GNUC__ )

#define MACHINE_TYPE_BIG_ENDIAN
#undef  MACHINE_TYPE_LITTLE_ENDIAN

#define DllImport
#define DllExport

#define stricmp                 strcasecmp

#endif

// convenience types
typedef unsigned int          u32;

// 8bit types
typedef unsigned char         U8;
typedef signed char           I8;
typedef unsigned char         u8;
typedef signed char           i8;

// 16 bit types
typedef unsigned short        U16;
typedef signed short          I16;
typedef unsigned short        u16;
typedef signed short          i16;

// 32 bit types
typedef unsigned int          U32;
typedef signed int            I32;
typedef unsigned int          u32;
typedef signed int            i32;

#if defined( _WINDOWS )
// 64 bit types
typedef unsigned __int64      U64;
typedef signed __int64        I64;
typedef unsigned __int64      u64;
typedef signed __int64        i64;
#define DOUBLE_CAST

#elif defined( __GNUC__ )
typedef unsigned long           ULONG_PTR;
typedef unsigned long long      U64;
typedef signed long long        I64;
typedef unsigned long long      u64;
typedef signed long long        i64;
#endif

typedef u32          intptr;

// f32 types
typedef float                 F32;
typedef double                F64;
typedef float                 f32;
typedef double                f64;


// stand-in for 32-bit pointers in ps3 structs
typedef u32          p32;

#if defined(_WIN32) || defined(WIN32)
#pragma warning(disable:4201)
#endif
//__________________________________________________________________________________________________
//
//Useful defines
//
//__________________________________________________________________________________________________

// tchar string
namespace std
{
#ifdef UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif // UNICODE
}

// Linux compiles
#if defined(__GNUC__) && !defined(_WINDOWS)
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#ifndef FLASH_PLATFORM
#include <alloca.h>
#include <sys/timeb.h>
#endif // #ifndef FLASH_PLATFORM
#include <wchar.h>

// TCHAR-type functions
#ifndef FLASH_PLATFORM
#define TCHAR char
#define _TCHAR char
typedef long time64_t;
typedef long __time32_t;   /* 32-bit time value */


typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE;

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif

#define vsnprintf_s
#define LPVOID void*

#define DWORD64 U64
#define BOOL int
#define DWORD64 U64
#define DWORD U32
#define _time64 time
#define UINT U32
#define WORD U16
#define LPWORD U16*
#define LPBYTE U8*
#define __int32 I32
#endif

#define DOUBLE_CAST (double)
#define __int64 long long
#define _atoi64 atoll

#define _tmain	main
#define _T(a)	a
#define	_totupper	_toupper
#define _ftprintf	fprintf
#define _tcsicmp	strcasecmp
#define	_tcscpy		strcpy
#define _tgetenv	getenv
#define _tcscat		strcat
#define	_stscanf	sscanf
#define	_ttoi		atoi
// some other remaps
#define _getcwd		getcwd
#define _tcscmp strcmp
#define _strcmpi stricmp
#define _stricmp strcasecmp
#define _isnan isnan
#define MAX_PATH (2048)
#define _alloca alloca
#define _wtoi atoi
#define _ftime ftime
#define _timeb timeb
#define vsprintf_s(buff, len, format, argptr)  vsprintf(buff, format, argptr)
#define _wcsicmp wcsicmp

// Linux doesn't do the _s-style functions

// strncpy() is NOT the same as strcpy_s as strncpy() does NOT copy the NUL if there is an overflow condition. That is bad.
inline char *strcpy_s( char *dest, int size, char *src )
{
	char *ret = dest;
	while ( size > 1 && *src )
		*dest++ = *src++;
	*dest++ = 0;
	return ret;
}

inline int sprintf_s( char *dest, int destSize, const char *format, ... )
{
	va_list vl;
	va_start(vl,format);
	int ret = vsprintf(dest, format, vl);
	va_end(vl);
	return ret;
}
#define _stprintf_s	sprintf_s
#define _localtime64_s(time,long_time)	localtime64((long_time))
#endif

// disables the copy constructor and assignment operator
#define DISABLE_COPYASSIGN(Class)		private: Class(const Class&); void operator=(const Class&)

// disables the copy constructor and assignment operator and default constructor. TECHNICALLY, this can be the same as the above since the default constructor is automatically
// disable whenever any other constructor is present, but we'll include it here for clarity
#define DISABLE_COPYASSIGNDEFAULT(Class)		private: Class(); Class(const Class&); void operator=(const Class&)

#endif // SERVER_TYPES_H
