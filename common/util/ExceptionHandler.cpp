//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  ExceptionHandler.cpp
//  
//__________________________________________________________________________________________________

#include "stdafx.h"
#if defined( _WINDOWS )
#include <windows.h>
#endif
#include "ExceptionHandler.h"
#include "util/CommonLogging.h"
#include "util/CommonAssert.h"
#include "DebugHandlers/CallStack.h"
#include <ace/OS.h>

#if !defined(__GNUC__) && defined(_WINDOWS)
#include <DbgHelp.h>
#endif

#if !defined(__GNUC__) && defined(_WINDOWS)

#include "DebugHandlers/ClientAsset.h"

#include <tchar.h>
#include <TlHelp32.h>

#endif

//__________________________________________________________________________________________________
//  
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <float.h>

#pragma fenv_access (on)

#define EXCEPTIONHANDLER_StackDumpSize  30    // Number of DWords displayed.


//__________________________________________________________________________________________________
//  
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ExceptionHandler_n
{
  LPTOP_LEVEL_EXCEPTION_FILTER  gPreviousFilter = NULL;
  CleanupCallBackFunctionPrt cleanup_func = NULL;

  LONG WINAPI                   Process( PEXCEPTION_POINTERS pExceptionInfo );
  void                          CreateCrashLog( PEXCEPTION_POINTERS pInfo );
}


//__________________________________________________________________________________________________
//  Function: ExceptionHandler_n::Register
//  
//  Description: 
//__________________________________________________________________________________________________
bool ExceptionHandler_n::Register(void)
{
  gPreviousFilter = SetUnhandledExceptionFilter( ExceptionHandler_n::Process );
  return true;
}

//__________________________________________________________________________________________________
//  Function: ExceptionHandler_n::Unregister
//  
//  Description: 
//__________________________________________________________________________________________________
void ExceptionHandler_n::Unregister(void)
{
  SetUnhandledExceptionFilter( gPreviousFilter );
}

void SetupCleanupCallback( CleanupCallBackFunctionPrt the_cleanup_func)
{
  ExceptionHandler_n::cleanup_func = the_cleanup_func;
}

//__________________________________________________________________________________________________
//  Function: ExceptionHandler_n::Process
//  
//  Description: 
//__________________________________________________________________________________________________
LONG WINAPI ExceptionHandler_n::Process( PEXCEPTION_POINTERS pInfo )
{
  //CreateCrashLog( pInfo );

  if(ExceptionHandler_n::cleanup_func != NULL)
  {
    ExceptionHandler_n::cleanup_func();
  }

#if !defined(__GNUC__) && defined(_WINDOWS)
  ClientAssert::BuildCallstack( );
  ClientAssert::WriteMiniDump(pInfo);
#endif

#if !defined(__GNUC__) && defined(_WINDOWS)
  ExceptionHandler_n::Unregister();
#endif

  if(IsCallFatal())
    ServerFatal();

  // Server Fatal normally does not return
  ExitProcess( 0 );

  return EXCEPTION_CONTINUE_EXECUTION;
}

//__________________________________________________________________________________________________
//  Function: ExceptionHandler_n::CreateCrashLog
//  
//  Description: 
//__________________________________________________________________________________________________
void ExceptionHandler_n::CreateCrashLog( PEXCEPTION_POINTERS pInfo )
{
#if defined( _XBOX )
#else if defined(_WINDOWS)
  PEXCEPTION_RECORD pRecord = pInfo->ExceptionRecord;
  PCONTEXT pCtx = pInfo->ContextRecord;
  char aBuf[ 65536 ];
  MEMORY_BASIC_INFORMATION cMBI;
  char aModule[ 512 ];
  U32* pStack;
  U32 nCount;

  //std::cout << "TEST 1" << std::endl;
  {
    strcpy( aModule, "<Unknown>" );
    if( VirtualQuery( pRecord->ExceptionAddress, &cMBI, sizeof( cMBI )))
    {
      //std::cout << "TEST 2" << std::endl;
      if( cMBI.AllocationBase )
      {
        GetModuleFileNameA(( HMODULE )cMBI.AllocationBase, aModule, sizeof( aModule ));
      }
    }
    //std::cout << "TEST 3" << std::endl;
    LogErrorf( "%s (0x%08X) in %s", GetExceptionString( pRecord->ExceptionCode ), pRecord->ExceptionCode, aModule );

    if( pRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION )
    {
      switch( pRecord->ExceptionInformation[ 0 ])
      {
      case  0:  sprintf( aBuf, "Access violation reading from memory location 0x%p.", ( void* )pRecord->ExceptionInformation[ 1 ]);           break;
      case  1:  sprintf( aBuf, "Access violation writing to memory location 0x%p.", ( void* )pRecord->ExceptionInformation[ 1 ]);             break;
      case  8:  sprintf( aBuf, "Data execution prevention violation at memory location 0x%p.", ( void* )pRecord->ExceptionInformation[ 1 ]);  break;
      }
      LogError(aBuf);
      //std::cout << "TEST 4" << std::endl;
    }
    else if( pRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR )
    {
      switch( pRecord->ExceptionInformation[ 0 ])
      {
      case  0:  sprintf( aBuf, "In page error reading from memory location 0x%p. NTSTATUS=0x%08X", ( void* )pRecord->ExceptionInformation[ 1 ], ( U32 )pRecord->ExceptionInformation[ 2 ]);              break;
      case  1:  sprintf( aBuf, "In page error writing to memory location 0x%p. NTSTATUS=0x%08X", ( void* )pRecord->ExceptionInformation[ 1 ], ( U32 )pRecord->ExceptionInformation[ 2 ]);                break;
      case  8:  sprintf( aBuf, "Data execution prevention violation at memory location 0x%p. NTSTATUS=0x%08X", ( void* )pRecord->ExceptionInformation[ 1 ], ( U32 )pRecord->ExceptionInformation[ 2 ]);  break;
      }

      LogError(aBuf);
    }
    //std::cout << "TEST 5" << std::endl;

    LogError("Registers");
#if defined( _WIN64 )
    sprintf( aBuf, "RAX:%016X    R8 :%016X" "RBX:%016X    R9 :%016X" "RCX:%016X    R10:%016X" "RDX:%016X    R11:%016X" , pCtx->Rax, pCtx->R8, pCtx->Rbx, pCtx->R9, pCtx->Rcx, pCtx->R10, pCtx->Rdx, pCtx->R11 );
    LogError(aBuf);

    sprintf( aBuf, "ESI:%016X    R12:%016X"  "EDI:%016X    R13:%016X" , pCtx->Rsi, pCtx->R12, pCtx->Rdi, pCtx->R13 );
    LogError(aBuf);

    sprintf( aBuf, "ESP:%016X    R14:%016X"  "EBP:%016X    R15:%016X" "RIP:%016X" , pCtx->Rsp, pCtx->R14, pCtx->Rbp, pCtx->R15, pCtx->Rip );
    LogError(aBuf);

    sprintf( aBuf, "CS:%04X  FS:%04X  GS:%04X" , pCtx->SegCs, pCtx->SegFs, pCtx->SegGs );
    LogError(aBuf);

    sprintf( aBuf,  "Flags:   %016X"  "FPU Ctrl:%08X" , pCtx->EFlags, _control87( 0, 0 ));
    LogError(aBuf);

    pStack = ( U32* )pCtx->Rsp;
#else // Win32...
    sprintf( aBuf, "EAX:%08X"  " EBX:%08X"  " ECX:%08X"  " EDX:%08X" , pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx );
    LogError(aBuf);

    sprintf( aBuf, "ESI:%08X"  " EDI:%08X" , pCtx->Esi, pCtx->Edi );
    LogError(aBuf);

    sprintf( aBuf, "ESP:%08X"  " EBP:%08X"  " EIP:%08X" , pCtx->Esp, pCtx->Ebp, pCtx->Eip );
    LogError(aBuf);

    sprintf( aBuf, "CS:%04X  DS:%04X  ES:%04X" , pCtx->SegCs, pCtx->SegDs, pCtx->SegEs );
    LogError(aBuf);

    sprintf( aBuf, "FS:%04X  GS:%04X  SS:%04X" , pCtx->SegFs, pCtx->SegGs, pCtx->SegSs );
    LogError(aBuf);

    sprintf( aBuf,  "Flags:   %08X"  " FPU Ctrl:%08X" , pCtx->EFlags, _control87( 0, 0 ));
    LogError(aBuf);

    pStack = ( U32* )( ULONG_PTR )pCtx->Esp;
    //std::cout << "TEST 6" << std::endl;
#endif
    if( IsBadReadPtr( pStack, EXCEPTIONHANDLER_StackDumpSize * 4 ))
    {
      LogError("Stack memory not accessible.");
    }
    else
    {
      sprintf( aBuf, "Stack from address 0x%p", pStack );
      LogError(aBuf);

      for( nCount = 0; nCount < EXCEPTIONHANDLER_StackDumpSize; nCount++ )
      {
        if( !( nCount % 10 ))
        {
          LogError("");
        }
        sprintf( aBuf, " %08x ", *pStack++ );
        LogError(aBuf);
      }
    }
    //std::cout << "TEST 7" << std::endl;
    LogError("Callstack");
    memset( aBuf, 0, sizeof( aBuf ));
    LogError(aBuf);

    LogError("Mapped Call Stack");

    // PRINT MAPPED CALL STACK
    {
      void* aCallstack[ 1024 ];
      memset(aCallstack, 0, 1024);
      HANDLE idThread = (HANDLE)GetCurrentThreadId();

      int nStackCount = BW_Callstack_n::GetCallStackMappedWithContext( (char *)aCallstack, 1024, pCtx, false, idThread);
      for( int i = 0; i < nStackCount; i++ )
      {
        sprintf( aBuf, "%p", aCallstack[i] );
        LogError(aBuf);
      }
    }
  }
  //std::cout << "TEST 8" << std::endl;
#endif
}

//__________________________________________________________________________________________________
//  Function: ExceptionHandler_n::GetExceptionString
//  
//  Description: 
//__________________________________________________________________________________________________
const char* ExceptionHandler_n::GetExceptionString( U32 nCode )
{
  switch( nCode )
  {
  case  EXCEPTION_ACCESS_VIOLATION:         return "Access Violation";
  case  EXCEPTION_DATATYPE_MISALIGNMENT:    return "Datatype Misalignment";
  case  EXCEPTION_BREAKPOINT:               return "Breakpoint";
  case  EXCEPTION_SINGLE_STEP:              return "Single Step";
  case  EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "Array Bounds Exceeded";
  case  EXCEPTION_FLT_DENORMAL_OPERAND:     return "Floating Point Denormal Operand";
  case  EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "Floating Point Divide By Zero";
  case  EXCEPTION_FLT_INEXACT_RESULT:       return "Floating Point Inexact Result";
  case  EXCEPTION_FLT_INVALID_OPERATION:    return "Floating Point Invalid Operation";
  case  EXCEPTION_FLT_OVERFLOW:             return "Floating Point Overflow";
  case  EXCEPTION_FLT_STACK_CHECK:          return "Floating Point Stack Check";
  case  EXCEPTION_FLT_UNDERFLOW:            return "Floating Point Underflow";
  case  EXCEPTION_INT_DIVIDE_BY_ZERO:       return "Integer Divide By Zero";
  case  EXCEPTION_INT_OVERFLOW:             return "Integer Overflow";
  case  EXCEPTION_PRIV_INSTRUCTION:         return "Privileged Instruction";
  case  EXCEPTION_IN_PAGE_ERROR:            return "In Page Error";
  case  EXCEPTION_ILLEGAL_INSTRUCTION:      return "Illegal Instruction";
  case  EXCEPTION_NONCONTINUABLE_EXCEPTION: return "Non Continuable Exception";
  case  EXCEPTION_STACK_OVERFLOW:           return "Stack Overflow";
  case  EXCEPTION_INVALID_DISPOSITION:      return "Invalid Disposition";
  case  EXCEPTION_GUARD_PAGE:               return "Guard Page";
  case  EXCEPTION_INVALID_HANDLE:           return "Invalid Handle";
  default:
    {
#if defined( _WINDOWS )
      static char aBuf[ 1024 ];
      FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE, GetModuleHandleA( "NTDLL.DLL" ), nCode, 0, aBuf, sizeof( aBuf ), 0 );
      return aBuf;
#else
      return "";
#endif
    }
  }
}

