//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  CommonLogging.h
//  
//__________________________________________________________________________________________________
#include "stdafx.h"
#include "server_types.h"
#include "CommonLogging.h"
#include "CommonAssert.h"
#include <ace/Date_Time.h>
#include <ace/Time_Value.h>
#include <stdarg.h>

// For logging
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>

#include <iomanip>
#include <sys/stat.h>

#include "StringHelpers.h"

#include "BuildMode.h"

#if !defined(__GNUC__) && defined(_WINDOWS)
#include <tchar.h>
#include <TlHelp32.h>
#endif

using namespace log4cplus;
using namespace log4cplus::helpers;

namespace
{

// helper macro to prevent repeated code
#define COMMON_LOGGING_STDSTR(Macro) \
  std::string cMsg = GetNowStr(); \
  cMsg += ' '; \
  if ( *tag != 0 ) \
{ \
  cMsg += tag; \
  cMsg += ' '; \
} \
  AppendNoBSN(cMsg,str); \
  Macro(logger, cMsg.c_str())


  const int MAX_PRINT_SIZE = 2048;

  // Single instance of logger
  Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("log"));
  bool s_bDisplayDate = false;

  bool s_bLoggingInitialized = false;

  const int MAX_TAG = 256;

#ifdef _MSC_VER
  // TODO: Causes a crash in Win XP. Check for define to skip under XP
  //__declspec(thread)
#endif // _MSC_VER
  char tag[ MAX_TAG ];

  void FormatDate( char *buffer, int bufferSize )
  {
    ACE_Date_Time cNow;
    cNow.update();

    // DATE format: "MM/DD/YYYY HH24:MI:SS.uuuuuu"
    if ( s_bDisplayDate )
    {
      sprintf_s(buffer, bufferSize, "%02ld/%02ld/%04ld %02ld:%02ld:%02ld.%ld", cNow.month(), cNow.day(), cNow.year(), cNow.hour(), cNow.minute(), cNow.second(), cNow.microsec());
    } else {
      sprintf_s(buffer, bufferSize, "%02ld/%02ld/%04ld %02ld:%02ld:%02ld.%ld", cNow.month(), cNow.day(), cNow.year(), cNow.hour(), cNow.minute(), cNow.second(), cNow.microsec());
    }
  }

  // BSN == Backslash 'N' (\n)
  void CleanBSN( char *buffer )
  {
    int length = (int)strlen(buffer);
    while ( length > 0 && buffer[length-1] == '\n' )
      --length;
    buffer[length] = 0;
  }

  void AppendNoBSN( std::string &out, const std::string &src )
  {
    // strip \n's off the end (maybe just one?)
    if ( src.length() > 0 && src[src.length()-1] == '\n' )
    {
      int target = (int)src.length()-1;
      while ( target > 0 && src[target-1] == '\n' ) 
        --target;
      out.append( src.begin(), src.begin() + target );
    } else 
      out += src; 
  }

} // anonymous namespace

extern void DisplayLoggerDate(bool bDisplayDate)
{
  s_bDisplayDate = bDisplayDate;
}

//__________________________________________________________________________________________________
//  Function: MakeNowDateStr
//  
//  Description: Format a Database DATE of Now()
//__________________________________________________________________________________________________
std::string GetNowStr(void)
{
  char sResult[128];
  FormatDate(sResult, sizeof(sResult)/sizeof(sResult[0]));
  return std::string(sResult);
}

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for debug
//__________________________________________________________________________________________________
void LogInfo(const std::string& str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_STDSTR(LOG4CPLUS_INFO);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogDebug
//  
//  Description: Log call for debug
//__________________________________________________________________________________________________
void LogDebug(const std::string& str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_STDSTR(LOG4CPLUS_DEBUG);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for Info
//__________________________________________________________________________________________________
void LogTrace(const std::string& str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_STDSTR(LOG4CPLUS_TRACE);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogWarning
//  
//  Description: Log call for Warning
//__________________________________________________________________________________________________
void LogWarning(const std::string& str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_STDSTR(LOG4CPLUS_WARN);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogError
//  
//  Description: Log call for error
//__________________________________________________________________________________________________
void LogError(const std::string& str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_STDSTR(LOG4CPLUS_ERROR);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}


// helper macro to prevent repeated code
#if defined(__GNUC__) && !defined(_WINDOWS)
#define COMMON_LOGGING_CSTR(Macro) \
  int len = (int)strlen(str) + (int)strlen(tag) + 128; \
  char *temp = (char*)alloca(len); \
  FormatDate(temp, len); \
  int cur = (int)strlen(temp); \
  temp[cur++] = ' '; \
  if ( *tag != 0 ) \
{	\
  strncpy( temp + cur, tag, len - cur);	\
  cur += (int)strlen(tag); \
  temp[cur++] = ' '; \
} \
  strncpy( temp + cur, str,  len - cur); \
  CleanBSN(temp); \
  Macro(logger,temp)
#else
#define COMMON_LOGGING_CSTR(Macro) \
  U32 len = (U32)(strlen(str) + strlen(tag) + 128); \
  char *temp = (char*)alloca(len); \
  FormatDate(temp, len); \
  U32 cur = (U32)strlen(temp); \
  temp[cur++] = ' '; \
  if ( *tag != 0 ) \
{	\
  strcpy_s( temp + cur, len - cur, tag );	\
  cur += (U32)strlen(tag); \
  temp[cur++] = ' '; \
} \
  strcpy_s( temp + cur, len - cur, str ); \
  CleanBSN(temp); \
  Macro(logger,temp)
#endif

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for debug
//__________________________________________________________________________________________________
void LogInfo(const char *str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_CSTR(LOG4CPLUS_INFO);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogDebug
//  
//  Description: Log call for debug
//__________________________________________________________________________________________________
void LogDebug(const char *str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_CSTR(LOG4CPLUS_DEBUG);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for Info
//__________________________________________________________________________________________________
void LogTrace(const char *str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_CSTR(LOG4CPLUS_TRACE);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogWarning
//  
//  Description: Log call for Warning
//__________________________________________________________________________________________________
void LogWarning(const char *str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_CSTR(LOG4CPLUS_WARN);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogError
//  
//  Description: Log call for error
//__________________________________________________________________________________________________
void LogError(const char *str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_CSTR(LOG4CPLUS_ERROR);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogFatal
//  
//  Description: Log call for fatal error
//__________________________________________________________________________________________________
void LogFatal(const char *str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_CSTR(LOG4CPLUS_FATAL);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}


//__________________________________________________________________________________________________
//  Function: LogFatal
//  
//  Description: Log call for fatal error
//__________________________________________________________________________________________________
void LogFatal(const std::string& str)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGING_STDSTR(LOG4CPLUS_FATAL);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

// helper macro to prevent repeated code
#if defined(__GNUC__) && !defined(_WINDOWS)
#define COMMON_LOGGINGV(Macro) \
  char buffer[MAX_PRINT_SIZE]; \
  FormatDate(buffer,sizeof(buffer)/sizeof(buffer[0])); \
  int pos = (int)strlen(buffer); \
  if ( *tag ) \
{	\
  buffer[pos++] = ' '; \
  assert( pos < sizeof(buffer)/sizeof(buffer[0]) );	\
  strncpy( buffer + pos, tag, (int)(sizeof(buffer)/sizeof(buffer[0])) - pos); \
  pos += (int)strlen(tag); \
} \
  assert( pos < sizeof(buffer)/sizeof(buffer[0]) );	\
  buffer[pos++] = ' '; \
  vsprintf( buffer + pos, format, vl ); \
  CleanBSN(buffer); \
  Macro(logger, buffer)

#define COMMON_LOGGINGF(Macro) \
  va_list vl; \
  va_start(vl,format); \
  COMMON_LOGGINGV(Macro);	\
  va_end(vl)
#else
#define COMMON_LOGGINGV(Macro) \
  char buffer[MAX_PRINT_SIZE]; \
  FormatDate(buffer,sizeof(buffer)/sizeof(buffer[0])); \
  U32 pos = (U32)strlen(buffer); \
  if ( *tag ) \
{	\
  buffer[pos++] = ' '; \
  assert( pos < sizeof(buffer)/sizeof(buffer[0]) );	\
  strcpy_s( buffer + pos, (sizeof(buffer)/sizeof(buffer[0])) - pos, tag ); \
  pos += (U32)strlen(tag); \
} \
  assert( pos < sizeof(buffer)/sizeof(buffer[0]) );	\
  buffer[pos++] = ' '; \
  vsprintf_s( buffer + pos, (sizeof(buffer)/sizeof(buffer[0])) - pos, format, vl ); \
  CleanBSN(buffer); \
  Macro(logger, buffer)

#define COMMON_LOGGINGF(Macro) \
  va_list vl; \
  va_start(vl,format); \
  COMMON_LOGGINGV(Macro);	\
  va_end(vl)
#endif

#define DEBUG_ASSERT_LOG(Macro,Expre) \
  COMMON_LOGGINGF(Macro)\
  assert((Expre));

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for debug (with printf-format)
//__________________________________________________________________________________________________
void LogInfo(const char *format, va_list vl)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGV(LOG4CPLUS_INFO);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//________________________________________________________________
//  Function: LogDebugf
//  
//  Description: Log call for debug (with printf-format)
//__________________________________________________________________________________________________
void LogDebug(const char *format, va_list vl)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGV(LOG4CPLUS_DEBUG);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for info (with printf-format)
//__________________________________________________________________________________________________
void LogTrace(const char *format, va_list vl)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGV(LOG4CPLUS_TRACE);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogWarning
//  
//  Description: Log call for warning (with printf-format)
//__________________________________________________________________________________________________
void LogWarning(const char *format, va_list vl)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGV(LOG4CPLUS_WARN);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogErrorf
//  
//  Description: Log call for error (with printf-format)
//__________________________________________________________________________________________________
void LogError(const char *format, va_list vl)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGV(LOG4CPLUS_ERROR);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogFatalf
//  
//  Description: Log call for fatal (with printf-format)
//__________________________________________________________________________________________________
void LogFatal(const char *format, va_list vl)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGV(LOG4CPLUS_FATAL);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogTracef
//  
//  Description: Log call for debug (with printf-format)
//__________________________________________________________________________________________________
void LogInfof(const char *format, ...)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGF(LOG4CPLUS_INFO);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogDebugf
//  
//  Description: Log call for debug (with printf-format)
//__________________________________________________________________________________________________
void LogDebugf(const char *format, ...)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGF(LOG4CPLUS_DEBUG);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogTrace
//  
//  Description: Log call for info (with printf-format)
//__________________________________________________________________________________________________
void LogTracef(const char *format, ...)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGF(LOG4CPLUS_TRACE);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogWarning
//  
//  Description: Log call for warning (with printf-format)
//__________________________________________________________________________________________________
void LogWarningf(const char *format, ...)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGF(LOG4CPLUS_WARN);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogErrorf
//  
//  Description: Log call for error (with printf-format)
//__________________________________________________________________________________________________
void LogErrorf(const char *format, ...)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGF(LOG4CPLUS_ERROR);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}

//__________________________________________________________________________________________________
//  Function: LogFatalf
//  
//  Description: Log call for fatal (with printf-format)
//__________________________________________________________________________________________________
void LogFatalf(const char *format, ...)
{
#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
  COMMON_LOGGINGF(LOG4CPLUS_FATAL);
#endif //#ifdef ENABLE_SERVER_PRINT_FUNCTIONS
}


//__________________________________________________________________________________________________
//  Function: Log
//  
//  Description: Log call for any type
//__________________________________________________________________________________________________
void Log(LogType type, const std::string &str)
{
  switch(type)
  {
  case LogType_Debug:
    LogDebug(str);
    break;
  case LogType_Info:
    LogTrace(str);
    break;
  case LogType_Warning:
    LogWarning(str);
    break;
  case LogType_Error:
    LogError(str);
    break;
  case LogType_Fatal:
    LogFatal(str);
    break;
  default:
    assert(false);
  }
}

//__________________________________________________________________________________________________
//  Function: Log
//  
//  Description: Log call for any type
//__________________________________________________________________________________________________
void Log(LogType type, const char *str)
{
  switch(type)
  {
  case LogType_Debug:
    LogDebug(str);
    break;
  case LogType_Info:
    LogTrace(str);
    break;
  case LogType_Warning:
    LogWarning(str);
    break;
  case LogType_Error:
    LogError(str);
    break;
  case LogType_Fatal:
    LogFatal(str);
    break;
  default:
    assert(false);
  }
}


//__________________________________________________________________________________________________
//  Function: Log
//  
//  Description: Log call for any type
//__________________________________________________________________________________________________
void Log(LogType type, const char *format, va_list vl)
{
  switch(type)
  {
  case LogType_Debug:
    LogDebug(format, vl);
    break;
  case LogType_Info:
    LogTrace(format, vl);
    break;
  case LogType_Warning:
    LogWarning(format, vl);
    break;
  case LogType_Error:
    LogError(format, vl);
    break;
  case LogType_Fatal:
    LogFatal(format, vl);
    break;
  default:
    assert(false);
  }
}

//__________________________________________________________________________________________________
//  Function: Logf
//  
//  Description: Log call for any type
//__________________________________________________________________________________________________
void Logf(LogType type, const char *format, ...)
{
  va_list vl;
  va_start(vl,format);
  switch(type)
  {
  case LogType_Debug:
    LogDebug(format, vl);
    break;
  case LogType_Info:
    LogTrace(format, vl);
    break;
  case LogType_Warning:
    LogWarning(format, vl);
    break;
  case LogType_Error:
    LogError(format, vl);
    break;
  case LogType_Fatal:
    LogFatal(format, vl);
    break;
  default:
    assert(false);
  }
  va_end(vl);
}

//__________________________________________________________________________________________________
//  Function: printDebug
//  
//  Description: Test Debgu out put
//__________________________________________________________________________________________________
void printDebug(void)
{
  LOG4CPLUS_TRACE_METHOD(logger, LOG4CPLUS_TEXT("::printDebug()"));
  LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
  LOG4CPLUS_INFO(logger, "This is a INFO message");
  LOG4CPLUS_WARN(logger, "This is a WARN message");
  LOG4CPLUS_ERROR(logger, "Thisd is a ERROR message");
  LOG4CPLUS_FATAL(logger, "This is a FATAL message");
}


//__________________________________________________________________________________________________
//  Function: InitializeLogging
//  
//  Description: Initialized out logging config
//__________________________________________________________________________________________________
bool InitializeLogging(LogMode logMode, const char *configFile)
{
  if ( !s_bLoggingInitialized ) 
  {
    LogLog::getLogLog()->setInternalDebugging(true);
    Logger root = Logger::getRoot();

    try {
      switch( logMode )
      {
      case LogMode_FileConfig:
        {
          std::tstring configFileObject;
          if ( configFile == NULL )
          {
            configFileObject.assign( _T("log4cplus.properties") );
          }
          else
          {
            commonAssert(configFile);
#if !defined(__GNUC__) && defined(_WINDOWS)
            StringHelpers::BuildTString( configFileObject, configFile );
#else
            configFileObject =  configFile;
#endif
          }

#if !defined(__GNUC__) && defined(_WINDOWS)
          struct _stat stFileInfo;
#else
          struct stat stFileInfo;
#endif

          int intStat = 0;
          // TODO w string issue
#if !defined(__GNUC__) && defined(_WINDOWS)
          intStat = _wstat(configFileObject.c_str(), &stFileInfo);
#else
          intStat = stat(configFileObject.c_str(), &stFileInfo);
#endif
          if(intStat == 0)
          {
            // use the config file 
            PropertyConfigurator::doConfigure(configFileObject.c_str());
          }
          else
          {
            // could not find file default to stdout logging
            helpers::Properties props;
            props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("TRACE, STDOUT"));
            props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
            PropertyConfigurator tempConfigurator(props);
            tempConfigurator.configure();
          }

          break;
        }

      case LogMode_UseStandardConfigAll:
        {
          // default is to just use the text screen
          helpers::Properties props;
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("TRACE, STDOUT"));
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          PropertyConfigurator tempConfigurator(props);
          tempConfigurator.configure();
          break;
        }

      case LogMode_UseStandardConfigDebug:
        {
          // default is to just use the text screen
          helpers::Properties props;
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("DEBUG, STDOUT"));
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          PropertyConfigurator tempConfigurator(props);
          tempConfigurator.configure();
          break;
        }

      case LogMode_UseStandardConfigInfo:
        {
          // default is to just use the text screen
          helpers::Properties props;
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("INFO, STDOUT"));
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          PropertyConfigurator tempConfigurator(props);
          tempConfigurator.configure();
          break;
        }

      case LogMode_UseStandardConfigWarning:
        {
          // default is to just use the text screen
          helpers::Properties props;
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("WARN, STDOUT"));
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          PropertyConfigurator tempConfigurator(props);
          tempConfigurator.configure();
          break;
        }

      case LogMode_UseStandardConfigError:
        {
          // default is to just use the text screen
          helpers::Properties props;
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("ERROR, STDOUT"));
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.STDOUT"), LOG4CPLUS_TEXT("log4cplus::ConsoleAppender"));
          PropertyConfigurator tempConfigurator(props);
          tempConfigurator.configure();
          break;
        }

      case LogMode_UseStandardConfigNone:
        {
          // just use nothing
          helpers::Properties props;
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.rootLogger"), LOG4CPLUS_TEXT("FATAL, DUMMY"));
          props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.DUMMY"), LOG4CPLUS_TEXT("log4cplus::NullAppender"));
          PropertyConfigurator tempConfigurator(props);
          tempConfigurator.configure();
        }
      }

      //LOG4CPLUS_WARN(root, "Logging Initalized....");

#ifdef TEST_LOGGING
      printDebug();
#endif
    }
    catch(...) {
      std::cout << "Exception..." << std::endl;
      LOG4CPLUS_FATAL(root, "Exception occured...");
      return false;
    }
    s_bLoggingInitialized = true;
  }
  return true;
}

void SetLoggingTag( const char *newTag )
{
  assert( newTag == NULL || strlen((char *)newTag) < MAX_TAG );

  if ( newTag == NULL )
    *tag = 0;
  else
    strcpy_s( tag, MAX_TAG, (char *)newTag );
}

//__________________________________________________________________________________________________
//  Function: InitializeLogging
//  
//  Description: Initialized out logging config
//__________________________________________________________________________________________________
bool ChangeLoggingMode(const std::string& new_level)
{
  if ( s_bLoggingInitialized ) 
  {
    LogLog::getLogLog()->setInternalDebugging(true);
    Logger root = Logger::getRoot();
    root.addAppender(SharedAppenderPtr( new log4cplus::ConsoleAppender(new SimpleLayout()) ) );

    int level = atoi(new_level.c_str());

    try {
      //LOG4CPLUS_WARN(root, "Logging Initialized....");

      switch(level)
      {
      case 0:
        root.setLogLevel(FATAL_LOG_LEVEL);
        break;

      case 1:
        root.setLogLevel(ERROR_LOG_LEVEL);
        break;

      case 2:
        root.setLogLevel(WARN_LOG_LEVEL);
        break;

      case 3:
        root.setLogLevel(INFO_LOG_LEVEL);
        break;

      case 4:
        root.setLogLevel(DEBUG_LOG_LEVEL);
        break;

      case 5:
        root.setLogLevel(TRACE_LOG_LEVEL);
        break;

      default:
        root.setLogLevel(TRACE_LOG_LEVEL);
        break;
      }


    }
    catch(...) {
      std::cout << "Exception..." << std::endl;
      LOG4CPLUS_FATAL(root, "Exception occured...");
      return false;
    }
    s_bLoggingInitialized = true;
  }
  return true;
}

