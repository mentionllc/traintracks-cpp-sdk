//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  ClientAsset.h
//  
//__________________________________________________________________________________________________

#ifndef CLIENT_ASSERT_H
#define CLIENT_ASSERT_H

#include "Util/fw_server_types.h"
#include "Util/BWAssert.h"

#include <string>
#include <vector>

//__________________________________________________________________________________________________
//  
//  Class: ClientAssert
//  
//  Description: 
//  
//__________________________________________________________________________________________________
class ClientAssert
{
public:
  ClientAssert() {}
  ~ClientAssert() {}

  static bool HandleAssert( const char* pExp, const char* pFile, int nLine, bwAssertFlags_e eFlags );

#if !defined(__GNUC__) && defined(_WINDOWS)
  static LONG WriteMiniDump( PEXCEPTION_POINTERS pEp );
#endif

  static void SetMiniDumpFilename(const std::string& name) { ClientAssert::gMiniDumpFilename = name; }
  static bool BuildCallstack( );


protected:
  static bool IsIgnored( void* pAddr );
  static void GenerateBugReport(bool bInformUser, bool bIsSilent);

  static std::vector< void* > gIgnored;
  static void* gRawCallstack[ 256 ];
  static int gStackCount;

  static bool gNoBugReport;
  static std::string gExpression;
  static std::string gComments;
  static std::string gResolvedCallstack[ 256 ];
  static std::string gFilename; 
  static std::string gEmail;
  static int gLine;
  static bwAssertFlags_e gAssertFlags;
  static std::string gMiniDumpFilename; 
};

#endif  //CLIENT_ASSERT_H
