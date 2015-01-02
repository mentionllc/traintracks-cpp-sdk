//////////////////////////////////////////////////////////////////////
// SplitPath.cpp : implementation of the CSplitPath class.          //
// (c) 1999, Kevin Lussier                                          //
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSplitPath.h"
#include "util/server_types.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSplitPath::CSplitPath( LPCTSTR lpszPath )
{
    // Initialize our objects
    memset( path_buffer, 0, sizeof( path_buffer ) );
    memset( drive, 0, sizeof( drive ) );
    memset( dir, 0, sizeof( dir ) );
    memset( fname, 0, sizeof( fname ) );
    memset( ext, 0, sizeof( ext ) );

    // If we were given a path, split it
    if ( lpszPath ) {
        Split( lpszPath );
    }
}

BOOL CSplitPath::Split( LPCTSTR lpszPath )
{
    // If we weren't given a path, fail
    if ( lpszPath == NULL ) {
        // Return failure
        return FALSE;
    }

#if !defined(__GNUC__) && defined(_WINDOWS)
    // Copy the path
    _tcsncpy( path_buffer, lpszPath, sizeof( path_buffer ) - 1 );
    // Split the given path
    _tsplitpath( path_buffer, drive, dir, fname, ext );
#endif

    return TRUE;
}


void CSplitPath::GetStringPath(std::wstring& npath)
{
  npath = path_buffer;
}

void CSplitPath::GetStringDrive(std::wstring& ndrive)
{
  ndrive = drive;
}

void CSplitPath::GetStringDirectory(std::wstring& ndir)
{
  ndir = dir;
}

void CSplitPath::GetStringFileName(std::wstring& file_name)
{
  file_name = fname;
}

void CSplitPath::GetStringExtension(std::wstring& exten)
{
  exten = ext;
}

void CSplitPath::GetStringDriveDirectory(std::wstring& drvdir)
{
  drvdir = drive;
  drvdir += dir;
}