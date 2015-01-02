//////////////////////////////////////////////////////////////////////
// SplitPath.h : interface for the CSplitPath class.                //
// (c) 1999, Kevin Lussier                                          //
//////////////////////////////////////////////////////////////////////

#ifndef __SPLITPATH_H__
#define __SPLITPATH_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "server_types.h"
#include <string>


#if !defined(__GNUC__) && defined(_WINDOWS)
#include <stdlib.h>
#include <tchar.h>
#endif

class CSplitPath
{ 
// Construction
public: 
    CSplitPath( LPCTSTR lpszPath = NULL );

// Operations
    BOOL   Split(LPCTSTR lpszPath );
    TCHAR* GetPath( void ) { return path_buffer; }
    TCHAR* GetDrive( void ) { return drive; }
    TCHAR* GetDirectory( void ) { return dir; }
    TCHAR* GetFileName( void ) { return fname; }
    TCHAR* GetExtension( void ) { return ext; }

    void GetStringPath(std::wstring& path);
    void GetStringDrive(std::wstring& drive);
    void GetStringDirectory(std::wstring& dir);
    void GetStringDriveDirectory(std::wstring& drvdir);
    void GetStringFileName(std::wstring& file_name);
    void GetStringExtension(std::wstring& exten);

// Attributes
protected:
    TCHAR path_buffer[_MAX_PATH];
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];
}; 

#endif  // __SPLITPATH_H__
