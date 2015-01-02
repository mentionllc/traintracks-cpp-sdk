//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  ExceptionHandler.h
//  
//__________________________________________________________________________________________________


#ifndef ExceptionHandler_H
#define ExceptionHandler_H

#include "util/server_types.h"

//________________________________________________
//  
//________________________________________________
#define FILE_CrashLog     "CrashLogFile.txt"
#define FILE_MiniDump     "WindowsMiniDump.dmp"
#define FILE_RawCallstack "RawCallStack.txt"

//________________________________________________
//  
//________________________________________________
namespace ExceptionHandler_n
{
  bool Register(void);
  void Unregister(void);

  const char* GetExceptionString( U32 nCode );
}

//________________________________________________
//  
//________________________________________________
extern void SetImmediateFatal(bool bFatalImmediateExit);
extern bool GetImmediateFatal();
extern bool IsCallFatal();
extern void GlobalSetCallFatal(bool bCallServerFatal);
extern void ServerFatal();

typedef void ( *CleanupCallBackFunctionPrt)();
extern void SetupCleanupCallback( CleanupCallBackFunctionPrt the_cleanup_func);


#endif  //BW_ExceptionHandler_H
