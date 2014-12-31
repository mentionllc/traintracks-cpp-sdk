#ifndef UTIL_StdAfx_H
#define UTIL_StdAfx_H

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


#if !defined(__GNUC__) && defined(_WINDOWS)
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#if defined( _WINDOWS )
  #pragma warning( disable : 4996 )  // 'function' was declared deprecated
#endif

#if defined( _WINDOWS )
  #pragma warning(disable:4003) // get rid of these: warning C4003: not enough actual parameters for macro '__inner_control_entrypoint'
	// make FD_SETSIZE ACE compatible
	#define FD_SETSIZE 1024
    #include <winsock2.h> // Must be included before <windows.h>
    #pragma pack()        // Fix the damage done by winsock2.
  #pragma warning(default:4003)
#endif

// For logging
//#include "Util/CommonLogging.h"

// Windows Header Files:
#if !defined(__GNUC__) && defined(_WINDOWS)
#include <windows.h>
#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc/malloc.h>
#include <memory.h>


#endif
