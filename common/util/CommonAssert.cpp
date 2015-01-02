//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  CommonLogging.h
//  
//__________________________________________________________________________________________________

#include "stdafx.h"
#include "util/server_types.h"
#include "util/CommonAssert.h"
#include "util/StringUtils.h"
#include "util/CommonLogging.h"

//#define VALGRIND_RUN 1
#include <boost/format.hpp> 

#include <ace/OS.h>


#if defined(__GNUC__) && !defined(_WINDOWS)
#include <signal.h>
#include <unistd.h>
#endif


namespace
{
  commonAssertHandler_t g_pAssertHandler = NULL;
  commonAssertMode_e    g_eAssertMode = e_AssertModeServerFatal;
}

void commonHandleStandardAssert(const wchar_t* pExp, const wchar_t* pFile, int Line)
{
    // convert wchars to chars
    char buff1[1024];
    char buff2[1024];
    StringCopy( (char *)buff1, 1024, (const U16*)pExp);
    StringCopy( (char *)buff2, 1024, (const U16*)pFile);
    commonHandleAssert( buff1, buff2, Line, e_NoAssertFlags );
}

void commonSetAssertHandler( commonAssertHandler_t pHandler )
{
  g_pAssertHandler = pHandler;
}

void commonSetAssertMode( commonAssertMode_e eMode )
{

  g_eAssertMode = eMode;
}

commonAssertMode_e commonGetAssertMode(void)
{
  return g_eAssertMode;
}

#if defined(__GNUC__) && !defined(_WINDOWS)
bool IsDebuggerAttached(void)
{
  static std::string s_cStatusFilename = (boost::format("/proc/%1%/status") % (U32)getpid()).str();
  FILE *pFile = fopen(s_cStatusFilename.c_str(), "rt");
  if(pFile)
  {
    char zLine[128];
    while(fgets(zLine, sizeof(zLine), pFile))
    {
      if(strncmp("TracerPid:", zLine, 10) == 0)
      {
        int nTracerPid = atoi(zLine+10);
        fclose(pFile);
        return nTracerPid != 0;
      }
    }
    fclose(pFile);
  }
  return false;
}
#endif


void commonHandleAssert( const char* pExp, const char* pFile, int nLine, commonAssertFlags_e eFlags )
{
  static int nWaitFlag = 0;
  if( g_pAssertHandler && g_pAssertHandler( pExp, pFile, nLine, eFlags ))
  {
    return;
  }

  LogErrorf( "%s(%d): ASSERTION FAILED: \"%s\"\n", pFile, nLine, pExp );

#if !defined( _WIN32 ) || defined( NDEBUG ) || defined(VALGRIND_RUN)
  if((eFlags & e_RecoverableAssert) == e_RecoverableAssert)
  {
#if !defined( _XBOX ) && !defined( _WIN32 )
    raise(SIGUSR1);
#endif
    return;
  }
  else
#endif
  {
    switch( g_eAssertMode )
    {
    case  e_AssertModeSleep:
      {
        for( ;; );
      }
      break;
    case  e_AssertModeExit:
      {
        g_eAssertMode = e_AssertModeAbort;  // Abort if we assert while exiting.
        exit( 50 );
      }
      break;
    case  e_AssertModeAbort:
    case  e_AssertModeSilentAbort:
      {
        LogErrorf( "ABORTING");
        abort();
      }
      break;

    case  e_AssertModeIgnore:
      {
        // do nothing just return
      }
      break;

    case e_AssertModeWaitTill:
      {
        while(nWaitFlag == 0)
        {
          ACE_OS::sleep(1);
        }
      }
      break;

        case e_AssertModeServerFatal:
        {
            //TODO: ServerFatal();
        }
      break;
    }
  }
}

std::string commonFormatAssert( const char* pFormat, ... )
{
  char    aBuf[ 1024 ]; // todo - replace with commonstring which does allocations on the heap and have printf style functions
  va_list cArgs;

  va_start( cArgs, pFormat );
#ifdef _WIN32
  _vsnprintf_s( aBuf, sizeof( aBuf ), _TRUNCATE, pFormat, cArgs );
#else
  vsnprintf( aBuf, sizeof( aBuf ), pFormat, cArgs );
#endif
  va_end( cArgs );

  return std::string( aBuf );
}

void CrashProgram(void)
{
    // Cause a CRASH
    *(volatile int* )0 = 0xDEADBEEF;
}