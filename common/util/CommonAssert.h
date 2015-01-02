//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  CommonAssert.h
//  
//__________________________________________________________________________________________________

#ifndef common_Assert_H
#define common_Assert_H

#include <string>
#include <sstream>

//__________________________________________________________________________________________________
//  enum commonAssertMode_e
//
// The different assert modes.
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
enum commonAssertMode_e
{
  /*! Exit when an assert fails. Nice if you use atexit() (default). */
  e_AssertModeExit,
  /*! Abort when an assert fails. This is similiar to assert() */
  e_AssertModeAbort,
  /*! Sleep when an assert fails. */
  e_AssertModeSleep,
  /*! Same as e_AssertModeAbort, but doesn't call DebugBreak()*/
  e_AssertModeSilentAbort,
  /*! Do nothing by default. Assert handlers will still be called.*/
  e_AssertModeIgnore,
	/*! Call ServerFatal, which is the default */
	e_AssertModeServerFatal,
  e_AssertModeWaitTill
};

//__________________________________________________________________________________________________
//  enum commonAssertFlags_e
//
// The different assert flags.
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
enum commonAssertFlags_e
{
  //! No Assert flags
  e_NoAssertFlags     = 0,
  //! This is a recoverable assert so we just send a bug and dont bother the player
  e_RecoverableAssert = 1<<0,
  //! Special flag for out-of-memory conditions.
  e_OutOfMemory = 1<<1,
  //! Special flag for graying out the continue button in the assert box.
  e_NoContinueButton = 1<<2
};

//__________________________________________________________________________________________________
//  Typedefs
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef bool ( *commonAssertHandler_t )( const char*, const char*, int, commonAssertFlags_e );

//__________________________________________________________________________________________________
//  Global Functions
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
extern void commonSetAssertHandler( commonAssertHandler_t pHandler );
extern void commonSetAssertMode( commonAssertMode_e eMode );
extern commonAssertMode_e commonGetAssertMode(void);
extern void commonHandleAssert( const char* pExp, const char* pFile, int nLine, commonAssertFlags_e eFlags = e_NoAssertFlags );
extern std::string commonFormatAssert( const char* pFormat, ... );
extern void commonHandleStandardAssert(const wchar_t* pExp, const wchar_t* pFile, int Line);
extern void CrashProgram(void);

#ifdef _UNICODE
#define commonShadowAssertHander commonHandleAssert
#define CAST_BASE (char*)
#else
#define commonShadowAssertHander commonHandleAssert
#define CAST_BASE (char*)
#endif

//__________________________________________________________________________________________________
//  MACROS for debug and live mode
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( NDEBUG ) || !defined( LIVE ) || defined( commonOPTIMISED )
  
#define commonAssert( exp)                                                    \
{                                                                         \
	bool  bValid = ( exp ) ? true : false;                                  \
	if( !bValid )                                                           \
	{                                                                       \
	  std::ostringstream o;                                                 \
	  o << #exp << std::endl;                                               \
	  commonShadowAssertHander( o.str().c_str(), __FILE__, __LINE__ );          \
	}                                                                       \
}

  
#define commonAssertFmt( exp, fmt )                                           \
{                                                                         \
	bool  bValid = ( exp ) ? true : false;                                  \
	if( !bValid )                                                           \
	{                                                                       \
	  std::ostringstream o;                                                 \
	  o << #exp << std::endl << commonFormatAssert fmt << std::endl;            \
	  commonShadowAssertHander( o.str().c_str(), __FILE__, __LINE__ );          \
	}                                                                       \
}

#define commonAssertMsg( exp, msg )                                           \
{                                                                         \
	bool  bValid = ( exp ) ? true : false;                                  \
	if( !bValid )                                                           \
	{                                                                       \
	  std::ostringstream o;                                                 \
	  o << #exp << std::endl << msg << std::endl;                           \
	  commonShadowAssertHander( o.str().c_str(), __FILE__, __LINE__ );          \
	}                                                                       \
}

#else
  // NON DEBUG compiles

  #define commonAssert( exp )
  #define commonAssertFmt( exp, fmt )
  #define commonAssertMsg( exp, msg )

#endif

// hook into standard assert
#ifndef _DLL
#define _wassert commonHandleStandardAssert
#endif

#endif  //common_Assert_H
