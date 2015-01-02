//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io. All Rights Reserved.
//  
//  ClientAsset.cpp
//  
//__________________________________________________________________________________________________


#include "stdafx.h"
#if !defined(__GNUC__) && defined(_WINDOWS)
#include <windows.h>
#include <DbgHelp.h>
#endif

#include "CallStack.h"
#include "ExceptionHandler.h"
#include "Util/fw_server_types.h"
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include "Util/BwAssert.h"
#include "Util/StringUtils.h"
#include "Util/CommonLogging.h"

//________________________________________________
//  
//________________________________________________
//#define VALGRIND_RUN 1

//________________________________________________
//  
//________________________________________________
std::vector< void* > ClientAssert::gIgnored;
void* ClientAssert::gRawCallstack[ 256 ];
int ClientAssert::gStackCount = 0;
bool ClientAssert::gNoBugReport = false;
std::string ClientAssert::gExpression;
std::string ClientAssert::gComments;
std::string ClientAssert::gResolvedCallstack[ 256 ];
std::string ClientAssert::gFilename; 
std::string ClientAssert::gEmail;
std::string ClientAssert::gMiniDumpFilename = "BaseMiniDumpFileName"; 
int ClientAssert::gLine = 0;
bwAssertFlags_e ClientAssert::gAssertFlags = e_NoAssertFlags;

//__________________________________________________________________________________________________
//  Function: ClientAssert::WriteMiniDump
//  
//  Description: 
//__________________________________________________________________________________________________
#if !defined(__GNUC__) && defined(_WINDOWS)
LONG ClientAssert::WriteMiniDump( PEXCEPTION_POINTERS pEp )  
{
  MINIDUMP_EXCEPTION_INFORMATION  cMDInfo;

  HANDLE cFile;
  U32 nControl;
  U32 nAccess;
  U32 nAttributes = FILE_ATTRIBUTE_NORMAL;
  nAccess = GENERIC_WRITE|GENERIC_READ;
  nControl = CREATE_ALWAYS;

  char aBuf[ 1024 ];
  LogErrorf("ERROR: ClientAssert::WriteMiniDump %s", ClientAssert::gMiniDumpFilename.c_str());
  sprintf(aBuf, "C:\\Temp\\%sMiniDump.dmp", ClientAssert::gMiniDumpFilename.c_str());
  // ClientAssert::gFilename
  U16 aBuf16[ 1024 ];
  StringCopy( aBuf16, 1024, aBuf );
  cFile = CreateFileW(( WCHAR* )aBuf16, nAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, nControl, nAttributes, NULL );
  if( cFile == INVALID_HANDLE_VALUE )
  {
    return EXCEPTION_EXECUTE_HANDLER;
  }

  cMDInfo.ThreadId = GetCurrentThreadId();
  cMDInfo.ClientPointers = true;
  cMDInfo.ExceptionPointers = pEp;

  if( !MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), cFile, MiniDumpNormal, &cMDInfo, NULL, NULL ))
  {
    U32 nError = GetLastError();
    LogErrorf( "MiniDump creation failed.  GetLastError returned 0x%X.\n", nError );
  }

  CloseHandle( cFile );

  return EXCEPTION_EXECUTE_HANDLER;
}
#endif // #if !defined(__GNUC__) && defined(_WINDOWS)


//__________________________________________________________________________________________________
//  Function: ClientAssert::IsIgnored
//  
//  Description: 
//__________________________________________________________________________________________________
bool ClientAssert::IsIgnored( void* pAddr )
{
  return std::find( gIgnored.begin(), gIgnored.end(), pAddr ) != gIgnored.end();
}

//__________________________________________________________________________________________________
//  Function: ClientAssert::GenerateBugReport
//  
//  Description: 
//__________________________________________________________________________________________________
void ClientAssert::GenerateBugReport(bool bInformUser, bool bIsSilent)
{
  int nIdx;

  if( !gNoBugReport && bInformUser && !bIsSilent)
  {
    //cBuf.WriteText( "[File]" STR_NewLine );
    LogError("[File]");
    std::string cFileName(gFilename);
    LogErrorf( "%s (%d)", gFilename.c_str(), gLine );

    LogError("[Details]");
    LogError(gExpression.c_str());

    LogError("[Callstack]");

    for( nIdx = 0; nIdx < 256; nIdx++ )
    {
      const char *pzCurrentFunction = gResolvedCallstack[ nIdx ].c_str();
      if( pzCurrentFunction[0] == 0 )
      {
        break;
      }

      LogErrorf( "[0x%p] %s", gRawCallstack[ nIdx + 3], pzCurrentFunction );  
    } 

    LogError("");
    for( int i = 3; i < gStackCount; i++ )
    {
      LogErrorf("%08x", gRawCallstack[i]);
    }
  }
}

//__________________________________________________________________________________________________
//  Function: ClientAssert::HandleAssert
//  
//  Description: 
//__________________________________________________________________________________________________
bool ClientAssert::HandleAssert( const char* pExp, const char* pFile, int nLine, bwAssertFlags_e eFlags )
{
#ifdef VALGRIND_RUN
  return true;
#endif

  char    aBuffer[ 1024 ];
  int     nIdx;
  char*   pPtr;

  // Check to see if base object is deleted??
  {
    gExpression = pExp ? pExp : "";
    gFilename = pFile ? pFile : "Unknown File";
    gLine = nLine;
    gStackCount = BW_Callstack_n::GetCallStack( gRawCallstack, 256 );
    if( !IsIgnored( gRawCallstack[ 3 ]))
    {
      // Force a SEH exception to get the exception information we
      // need to correctly create a minidump.
#if !defined(__GNUC__) && defined(_WINDOWS)
      __try
      {
        *( int* )0 = 0xDEADBABE;
      }
      __except( ClientAssert::WriteMiniDump(GetExceptionInformation()) )
      { 
      }
#endif

      for( nIdx = 3; nIdx < gStackCount; nIdx++ )
      {
        BW_Callstack_n::MapAddress( gRawCallstack[ nIdx ], aBuffer, sizeof( aBuffer ));
        pPtr = strchr( aBuffer, ' ' );
        gResolvedCallstack[ nIdx - 3 ] = pPtr ? ( pPtr + 1 ) : aBuffer;
      }
      gResolvedCallstack[ nIdx ] = "";
      //gNoBugReport = false;
#if defined( NDEBUG )
      //if this is not a recoverable assert handle it normally
#if 0
      if((eFlags & e_RecoverableAssert) == e_RecoverableAssert)
      {
        gIgnored.push_back( gRawCallstack[ 3 ]);
        GenerateBugReport(false, true);
        return true;
      }
#endif

      GenerateBugReport(true, false);
#else
      GenerateBugReport(true, false);
#endif
    }
  }
  bwSetAssertHandler( (bwAssertHandler_t)*&ClientAssert::HandleAssert );

  return false;
}

bool ClientAssert::BuildCallstack( )
{
  char    aBuffer[ 1024 ];
  int     nIdx;
  char*   pPtr;

  // Check to see if base object is deleted??
  {
    gStackCount = BW_Callstack_n::GetCallStack( gRawCallstack, 256 );
    if( !IsIgnored( gRawCallstack[ 3 ]))
    {
      // Force a SEH exception to get the exception information we
      // need to correctly create a minidump.
      for( nIdx = 3; nIdx < gStackCount; nIdx++ )
      {
        BW_Callstack_n::MapAddress( gRawCallstack[ nIdx ], aBuffer, sizeof( aBuffer ));
        pPtr = strchr( aBuffer, ' ' );
        gResolvedCallstack[ nIdx - 3 ] = pPtr ? ( pPtr + 1 ) : aBuffer;
      }
      gResolvedCallstack[ nIdx ] = "";
      //gNoBugReport = false;
      GenerateBugReport(true, false);
    }
  }

  return false;
}



