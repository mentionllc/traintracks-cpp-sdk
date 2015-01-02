//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2014 Traintracks.io. All Rights Reserved.
//  
//  CommonLogging.h
//  
//__________________________________________________________________________________________________
#ifndef CommonLogging_H
#define CommonLogging_H 1

#include <string>
#include <iostream>
#include <sstream>

#if defined(__GNUC__) && !defined(_WINDOWS)
#include <stdarg.h>
#endif


extern std::string GetNowStr(void);

enum LogMode
{
	LogMode_FileConfig,
	LogMode_UseStandardConfigAll,
	LogMode_UseStandardConfigDebug,
	LogMode_UseStandardConfigInfo,
	LogMode_UseStandardConfigWarning,
	LogMode_UseStandardConfigError,
	LogMode_UseStandardConfigNone
};

typedef enum _LoggingState
{
  LOG_INFO,
  LOG_DEBUG,
  LOG_TRACE,
  LOG_WARNING,
  LOG_ERROR,
  LOG_FATAL,

  LAST_LOG_STATE
} LoggingState;

extern bool InitializeLogging(LogMode logMode = LogMode_FileConfig, const char *configFile = NULL);
extern void DisplayLoggerDate(bool bDisplayDate);
bool ChangeLoggingMode(const std::string& new_level);

extern void LogInfo(const std::string& str);
extern void LogDebug(const std::string& str);
extern void LogTrace(const std::string& str);
extern void LogWarning(const std::string& str);
extern void LogError(const std::string& str);
extern void LogFatal(const std::string& str);

extern void LogInfo(const char *str);
extern void LogDebug(const char *str);
extern void LogTrace(const char *str);
extern void LogWarning(const char *str);
extern void LogError(const char *str);
extern void LogFatal(const char *str);

extern void LogInfo(const char *format, va_list vl);
extern void LogDebug(const char *format, va_list vl);
extern void LogTrace(const char *format, va_list vl);
extern void LogWarning(const char *format, va_list vl);
extern void LogError(const char *format, va_list vl);
extern void LogFatal(const char *format, va_list vl);

extern void LogInfof(const char *format, ...);
extern void LogDebugf(const char *format, ...);
extern void LogTracef(const char *format, ...);
extern void LogWarningf(const char *format, ...);
extern void LogErrorf(const char *format, ...);
extern void LogFatalf(const char *format, ...);

enum LogType
{
	LogType_Debug,
	LogType_Info,
	LogType_Warning,
	LogType_Error,
	LogType_Fatal,
  LogType_Trace
};

extern void Log(LogType type, const std::string &str);
extern void Log(LogType type, const char *format);
extern void Log(LogType type, const char *format, va_list vl);
extern void Logf(LogType type, const char *format, ...);

extern void SetLoggingTag( const char *tag );


#endif // #ifndef CommonLogging_H